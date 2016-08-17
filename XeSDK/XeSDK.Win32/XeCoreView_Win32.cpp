#include "pch.h"
#include <XeSDK/ICoreView.h>
#include "XeCoreView.h"

#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeString.h>
#include <Windows.h>
#include <Windowsx.h>
#include <ShellScalingAPI.h>

using namespace Xe::Graphics;

typedef void(WINAPI * PFNREGISTERTOUCHWINDOWPROC)(HWND hWnd, ULONG ulFlags);
typedef BOOL(WINAPI * PFNSETPROCESSDPIAWAREPROC)(void);
typedef BOOL(WINAPI * PFNSETPROCESSDPIAWARENESSPROC)(PROCESS_DPI_AWARENESS value);

namespace Xe {
	namespace Core {
		class CFrameView : public IFrameView {
			IView* m_pView;
			IO::PointerEvent m_pointerEvent;
			bool m_IsViewInitialized;
		public:
			CFrameView(IView *pView) :
				m_pView(pView),
				m_IsViewInitialized(false),
				m_hWnd(nullptr),
				m_isClosed(true) {
				m_hInstance = GetModuleHandle(NULL);

				m_pointerEvent.CurrentPointer.IsProcessed = true;
				m_pointerEvent.CurrentPointer.Device.Type = IO::PointerType_Mouse;
				m_pointerEvent.CurrentPointer.Device.Is3dSupported = false;
				m_pointerEvent.CurrentPointer.Position.Set(-1, -1);
				m_pointerEvent.CurrentPointer.Buttons = 0;
				m_pointerEvent.CurrentPointer.Pressure = 0.0f;
				m_pointerEvent.CurrentPointer.Time = Timer::Current();
			}
			~CFrameView() {
				if (m_hWnd) CloseWindow(m_hWnd);
				UnregisterWindowClass();
			}

			void SetViewInitializeState(bool isInitialized) {
				m_IsViewInitialized = isInitialized;
			}
			bool Initialize(const IView::InitProperties& properties) {
				m_atom = RegisterWindowClass();
				if (m_atom == NULL)
					return false;
				m_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
				m_styleEx = WS_EX_APPWINDOW;

				UINT flags = 0;
				if (flags & 1)
				{
					m_style |= WS_POPUP;
				}
				else
				{
					m_style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
				}
				if (properties.IsResizable)
				{
					m_style |= WS_MAXIMIZEBOX | WS_SIZEBOX;
					m_styleEx |= WS_EX_WINDOWEDGE;
				}
				m_isFullscreenSwitchEnabled = (flags & 2) != 0;

				const Size &size = properties.Size;
				int width = (int)(size.x >= 0 ? size.x : 800);
				int height = (int)(size.y >= 0 ? size.y : 480);
				m_hWnd = CreateWindowEx(m_styleEx, _T("XEWND"), properties.Title, m_style,
					CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hInstance, this);
				if (m_hWnd == nullptr) return false;

				HMODULE hModuleUser32 = LoadLibraryExW(L"USER32.DLL", NULL, 0);
				HMODULE hModuleShcore = LoadLibraryExW(L"USER32.DLL", NULL, 0);
				// Enable DPI support
				PFNSETPROCESSDPIAWARENESSPROC pfnSetProcessDpiAwareness =
					(PFNSETPROCESSDPIAWARENESSPROC)GetProcAddress(hModuleShcore, "SetProcessDpiAwareness");
				if (pfnSetProcessDpiAwareness) {
					pfnSetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
				}
				else {
					PFNSETPROCESSDPIAWAREPROC pfnSetProcessDPIAware =
						(PFNSETPROCESSDPIAWAREPROC)GetProcAddress(hModuleUser32, "SetProcessDPIAware");
					if (pfnSetProcessDPIAware)
						pfnSetProcessDPIAware();
				}
				// Enable touch
				PFNREGISTERTOUCHWINDOWPROC pfnRegisterTouchWindow =
					(PFNREGISTERTOUCHWINDOWPROC)GetProcAddress(hModuleUser32, "RegisterTouchWindow");
				if (pfnRegisterTouchWindow)
					pfnRegisterTouchWindow(m_hWnd, TWF_WANTPALM);

				FreeLibrary(hModuleShcore);
				FreeLibrary(hModuleUser32);

				SetSize(Size(width, height));
				ShowWindow(m_hWnd, true);
				m_isClosed = false;
				m_isWindowMinimized = false;
				return true;
			}

			void SetTitleT(const char* title) {
				SetWindowTextA(m_hWnd, title);
			}
			void SetTitleT(const wchar_t* title) {
				SetWindowTextW(m_hWnd, title);
			}
			void SetTitle(const String& title) {
				SetTitleT(title.GetData());
			}
			float GetScale() const {
				HDC hDc = GetDC(m_hWnd);
				int xPixelPerInch = GetDeviceCaps(hDc, LOGPIXELSX);
				ReleaseDC(m_hWnd, hDc);
				return (float)xPixelPerInch / 96.0f;
			}
			Graphics::Size GetSize() const {
				RECT rect;
				GetClientRect(m_hWnd, &rect);
				return Size(rect.right - rect.left, rect.bottom - rect.top);
			}
			bool SetSize(const Graphics::Size& size) {
				if (size.x <= 0 || size.y <= 0)
					return false;
				RECT rcWindow;
				RECT rcClient;
				POINT ptDiff;

				GetWindowRect(m_hWnd, &rcWindow);
				GetClientRect(m_hWnd, &rcClient);

				ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
				ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
				rcClient.right = (LONG)(rcClient.left + size.x);
				rcClient.bottom = (LONG)(rcClient.top + size.y);

				if (AdjustWindowRectEx(&rcClient, (DWORD)GetWindowLongPtr(m_hWnd, GWL_STYLE), FALSE,
					(DWORD)GetWindowLongPtr(m_hWnd, GWL_EXSTYLE))) {
					if (SetWindowPos(m_hWnd, 0, 0, 0, (int)size.x + ptDiff.x, (int)size.y + ptDiff.y,
						SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER))
						return true;
				}
				return false;
			}
			Graphics::Orientation GetOrientation() const {
				return Graphics::Orientation_Unknown;
			}
			void SetPreferredOrientation(Graphics::Orientation orientation) {

			}

			HWND GetHwnd() const {
				return m_hWnd;
			}
			LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
				switch (uMsg) {
				case WM_NCDESTROY:
					m_isClosed = true;
					break;
				case WM_SIZE:
					switch (wParam) {
					case SIZE_RESTORED:
					case SIZE_MAXIMIZED:
					case SIZE_MAXSHOW:
					case SIZE_MAXHIDE:
						if (m_isWindowMinimized) {
							m_isWindowMinimized = false;
							if (m_IsViewInitialized)
								m_pView->OnVisibilityChanged(true);
						}
						m_size.x = LOWORD(lParam);
						m_size.y = HIWORD(lParam);
						m_pointerEvent.CurrentPointer.Device.Rectangle.left = 0;
						m_pointerEvent.CurrentPointer.Device.Rectangle.top = 0;
						m_pointerEvent.CurrentPointer.Device.Rectangle.right = m_size.x;
						m_pointerEvent.CurrentPointer.Device.Rectangle.bottom = m_size.y;
						if (m_IsViewInitialized)
							m_pView->OnSizeChanged(GetSize());
						break;
					case SIZE_MINIMIZED:
						m_isWindowMinimized = true;
						if (m_IsViewInitialized)
							m_pView->OnVisibilityChanged(false);
						break;
					}
					break;
				case WM_SETFOCUS:
					if (m_IsViewInitialized)
						m_pView->OnFocusGot();
					break;
				case WM_KILLFOCUS:
					if (m_IsViewInitialized)
						m_pView->OnFocusLost();
					break;
				case WM_PAINT:
					break;
				case WM_CLOSE:
					if (m_IsViewInitialized)
						if (m_pView->OnClosing(false))
							DestroyWindow(hWnd);
					return 0;
				case WM_ERASEBKGND:
					return 0;
				case WM_KEYDOWN:
					if (m_IsViewInitialized)
						OnKey(wParam, lParam, false);
					break;
				case WM_KEYUP:
					if (m_IsViewInitialized)
						OnKey(wParam, lParam, true);
					break;
				case WM_CHAR:
					if (m_IsViewInitialized)
						OnChar(wParam, lParam);
					break;
				case WM_MOUSEMOVE:
					if (m_IsViewInitialized)
						OnMouseMove(wParam, lParam);
					break;
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_XBUTTONDOWN:
					if (m_IsViewInitialized)
						OnMouseButton(wParam, lParam, false);
					break;
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_MBUTTONUP:
				case WM_XBUTTONUP:
					if (m_IsViewInitialized)
						OnMouseButton(wParam, lParam, true);
					break;
				case WM_MOUSEWHEEL:
					if (m_IsViewInitialized)
						OnMouseWheel(wParam, lParam, false);
					break;
				case WM_MOUSEHWHEEL:
					if (m_IsViewInitialized)
						OnMouseWheel(wParam, lParam, true);
					break;
				case WM_TOUCH:
					if (m_IsViewInitialized)
						OnTouch(hWnd, wParam, lParam);
					break;
				case WM_MOUSELEAVE:
					break;
				}
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		private:
			static svar m_classesInstanceCount;
			static ATOM m_atom;

			HINSTANCE m_hInstance;
			DWORD m_style;
			DWORD m_styleEx;
			HWND m_hWnd;
			bool m_isClosed;
			bool m_isFullscreenSwitchEnabled;
			bool m_isWindowMinimized;
			Size m_size;

			static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg,
				WPARAM wParam, LPARAM lParam)
			{
				CFrameView* pWindow = (CFrameView*)GetWindowLongPtr(hWnd, 0);
				if (uMsg == WM_CREATE)
				{
					CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
					SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);
				}
				if (pWindow != nullptr)
					return pWindow->WndProc(hWnd, uMsg, wParam, lParam);
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			ATOM RegisterWindowClass()
			{
				if (m_classesInstanceCount++ > 0)
					return m_atom;
				WNDCLASS wc;
				ATOM classAtom;

				wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				wc.lpfnWndProc = (WNDPROC)_WndProc;
				wc.cbClsExtra = 0;
				wc.cbWndExtra = sizeof(void*);
				wc.hInstance = m_hInstance;
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				wc.hbrBackground = nullptr;
				wc.lpszMenuName = nullptr;
				wc.lpszClassName = _T("XEWND");

				wc.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(101));
				if (wc.hIcon == nullptr)
					wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

				classAtom = RegisterClass(&wc);
				return classAtom;
			}
			void UnregisterWindowClass() {
				if (--m_classesInstanceCount == 0)
					UnregisterClass(_T("XEWND"), m_hInstance);
			}

			void OnChar(WPARAM wParam, LPARAM lParam) {
				IO::CharacterEvent e;
				e.ScanCode = (u32)((wParam >> 16) & 8);
				e.Character = (u32)wParam;
				m_pView->OnCharacter(e);
			}
			void OnKey(WPARAM wParam, LPARAM lParam, bool isReleased) {
				union Param {
					LPARAM Data;
					struct {
						u32 RepeatCount : 16;
						u32 ScanCode : 8;
						u32 IsExtended : 1;
						u32 Reserved : 4;
						u32 ContextCode : 1;
						u32 PreviousState : 1;
						u32 TransitionState : 1;
					};
				};

				IO::KeyboardEvent e;
				Param param; param.Data = lParam;
				e.ScanCode = param.ScanCode;
				e.VirtualCode = (u32)wParam;
				isReleased ? m_pView->OnKeyReleased(e) :
					m_pView->OnKeyPressed(e);
			}
			void OnMouseWheel(WPARAM wParam, LPARAM lParam, bool isHorizontal) {
				float value = (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
				if (isHorizontal) m_pView->OnPointerWheel(Math::Vector2f(value, 0.0f));
				else m_pView->OnPointerWheel(Math::Vector2f(0.0f, value));
			}
			void OnTouch(HWND hWnd, WPARAM wParam, LPARAM lParam) {
				UINT cInputs = LOWORD(wParam);
				PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
				if (pInputs != NULL)
				{
					if (GetTouchInputInfo((HTOUCHINPUT)lParam,
						cInputs, pInputs, sizeof(TOUCHINPUT)))
					{
						UINT type = 0;
						RECT rect;
						GetClientRect(m_hWnd, &rect);
						/*UINT count = Math::Min<UINT>(cInputs, pointerEvent.MaxPointersCount - 1);
						for (UINT i = 0; i < count; i++) {
							TOUCHINPUT& ti = pInputs[i];
							IO::Pointer& p = pointerEvent.Pointer[i + 1];
							if (p.IsProcessed == false) {
								p.IsProcessed = true;
								p.Time = Timer::Current();
							}
							POINT touchPoint;
							touchPoint.x = TOUCH_COORD_TO_PIXEL(ti.x);
							touchPoint.y = TOUCH_COORD_TO_PIXEL(ti.y);
							ScreenToClient(m_hWnd, &touchPoint);

							p.Device.Type = IO::PointerType_Touch;
							p.Device.Rectangle.left = rect.left;
							p.Device.Rectangle.top = rect.top;
							p.Device.Rectangle.right = rect.right;
							p.Device.Rectangle.bottom = rect.bottom;
							p.Device.Is3dSupported = false;
							if (ti.dwFlags & TOUCHEVENTF_MOVE) {
								p.Position.x = (svar)touchPoint.x;
								p.Position.y = (svar)touchPoint.y;
								p.Buttons = 0;
								p.Pressure = 1.0f;
								pointerEvent.PointerIndex = i + 1;
								type = 1;
							}
							else if (ti.dwFlags & TOUCHEVENTF_DOWN) {
								p.Position.x = (svar)touchPoint.x;
								p.Position.y = (svar)touchPoint.y;
								p.Buttons = 0;
								p.Pressure = 1.0f;
								pointerEvent.PointerIndex = i + 1;
								type = 2;
							}
							else if (ti.dwFlags & TOUCHEVENTF_UP) {
								p.Position.x = -1;
								p.Position.y = -1;
								p.Buttons = 0;
								p.Pressure = 0.0f;
								pointerEvent.PointerIndex = i + 1;
								type = 3;
							}
						}
						for (UINT i = count; i < pointerEvent.MaxPointersCount; i++)
							pointerEvent.Pointer[i].IsProcessed = false;*/
						if (!CloseTouchInputHandle((HTOUCHINPUT)lParam))
						{
							// error handling
						}
						switch (type) {
						case 1: //m_pCallback->OnPointerMove(pointerEvent);
							break;
						case 2: //m_pCallback->OnPointerPressed(pointerEvent);
							break;
						case 3: //m_pCallback->OnPointerReleased(pointerEvent);
							break;
						}
					}
					else
					{
						// GetLastError() and error handling
					}
					delete[] pInputs;
				}
				else
				{
					// error handling, presumably out of memory
				}
			}
			uvar GetMouseButtonFromWParam(WPARAM wParam) {
				return ((wParam & MK_LBUTTON) ? IO::PointerButton_Left : 0) |
					((wParam & MK_RBUTTON) ? IO::PointerButton_Right : 0) |
					((wParam & MK_MBUTTON) ? IO::PointerButton_Center : 0) |
					((wParam & MK_XBUTTON1) ? IO::PointerButton_XButton1 : 0) |
					((wParam & MK_XBUTTON2) ? IO::PointerButton_XButton2 : 0);
			}
			void OnMouseMove(WPARAM wParam, LPARAM lParam) {
				m_pointerEvent.CurrentPointer.Time = Timer();
				m_pointerEvent.CurrentPointer.Position.x = GET_X_LPARAM(lParam);
				m_pointerEvent.CurrentPointer.Position.y = GET_Y_LPARAM(lParam);
				if (wParam != 0) {
					m_pointerEvent.CurrentPointer.Buttons = GetMouseButtonFromWParam(wParam);
					m_pointerEvent.CurrentPointer.Pressure = 1.0f;
					m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Moved;
				}
				else {
					m_pointerEvent.CurrentPointer.Buttons = 0;
					m_pointerEvent.CurrentPointer.Pressure = 0.0f;
					m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Moved;
				}
				m_pView->OnPointerMoved(m_pointerEvent);
			}
			void OnMouseButton(WPARAM wParam, LPARAM lParam, bool isReleased) {
				m_pointerEvent.CurrentPointer.Time = Timer();
				m_pointerEvent.CurrentPointer.Position.x = GET_X_LPARAM(lParam);
				m_pointerEvent.CurrentPointer.Position.y = GET_Y_LPARAM(lParam);
				if (wParam != 0) {
					m_pointerEvent.CurrentPointer.Buttons = GetMouseButtonFromWParam(wParam);
					m_pointerEvent.CurrentPointer.Pressure = 1.0f;
				}
				else {
					m_pointerEvent.CurrentPointer.Buttons = 0;
					m_pointerEvent.CurrentPointer.Pressure = 0.0f;
				}
				if (isReleased) {
					m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Released;
					m_pView->OnPointerReleased(m_pointerEvent);
				}
				else {
					m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Pressed;
					m_pView->OnPointerPressed(m_pointerEvent);
				}
			}
			bool DispatchEvents(DispatchType type) {
				MSG msg;
				switch (type) {
					case DispatchType_Exit:
						return false;
					case DispatchType_None:
						return true;
					case DispatchType_One:
						PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE);
						TranslateMessage(&msg);
						DispatchMessage(&msg);
						break;
					case DispatchType_All:
						while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						break;
					case DispatchType_WaitOnce:
						while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						GetMessage(&msg, m_hWnd, 0, 0);
						break;
					case DispatchType_WaitUntilQuit:
						while (GetMessage(&msg, m_hWnd, 0, 0))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						break;
				}
				return !m_isClosed;
			}
		};
		svar CFrameView::m_classesInstanceCount = 0;
		ATOM CFrameView::m_atom = NULL;

		void IView::Run(IView* pView, const InitProperties& properties) {
			CFrameView *pFrameView = new CFrameView(pView);
			pView->m_FrameView = pFrameView;

			if (pFrameView->Initialize(properties)) {
				if (pView->OnInitialize()) {
					pFrameView->SetViewInitializeState(true);
					pView->OnRun();
					pFrameView->SetViewInitializeState(false);
				}
			}
			pView->OnDestroy();
			delete pFrameView;
		}
		void GetViewInternal(IView* pView, ViewInternal& viewInternal) {
			CFrameView *pFrameView = (CFrameView*)pView->m_FrameView;
			viewInternal.Window = pFrameView->GetHwnd();
		}
	}
}