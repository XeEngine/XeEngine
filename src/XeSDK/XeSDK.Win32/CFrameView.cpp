#include "pch.h"
#include "CFrameView.h"
#include "DummyApplicationHandler.h"
#include "DummyFrameHandler.h"
#include "DummyKeyboardHandler.h"
#include "DummyPointerHandler.h"

#include <Windows.h>
#include <Windowsx.h>

#if WINVER >= 0x0601
#include <ShellScalingAPI.h>
#else

#ifndef DPI_ENUMS_DECLARED // ShellScalingAPI.h

typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE {
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

#define DPI_ENUMS_DECLARED
#endif // (DPI_ENUMS_DECLARED)

#ifndef TWF_FINETOUCH
#define TWF_FINETOUCH       (0x00000001)
#endif
#ifndef TWF_WANTPALM
#define TWF_WANTPALM        (0x00000002)
#endif
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL      0x020E
#endif
#ifndef WM_TOUCH
#define WM_TOUCH            0x0240
#endif

#endif // ShellScalingAPI support

using namespace Xe;
using namespace Xe::Core;
using namespace Xe::Graphics;

typedef void(WINAPI * PFNREGISTERTOUCHWINDOWPROC)(HWND hWnd, ULONG ulFlags);
typedef BOOL(WINAPI * PFNSETPROCESSDPIAWAREPROC)(void);
typedef BOOL(WINAPI * PFNSETPROCESSDPIAWARENESSPROC)(PROCESS_DPI_AWARENESS value);

svar CFrameView::m_classesInstanceCount = 0;
ATOM CFrameView::m_atom = 0;

CFrameView::CFrameView(Xe::Core::IFrameHandler* pFrameHandler) :
	m_isClosed(true),
	m_hWnd(nullptr),
	m_pFrameHandler(pFrameHandler),
	m_pApplicationHandler(new DummyApplicationHandler),
	m_pKeyboardHandler(new DummyKeyboardHandler),
	m_pPointerHandler(new DummyPointerHandler)
{
	m_hInstance = GetModuleHandle(NULL);

	m_pointerEvent.CurrentPointer.IsProcessed = true;
	m_pointerEvent.CurrentPointer.Device.Type = IO::PointerType_Mouse;
	m_pointerEvent.CurrentPointer.Device.Is3dSupported = false;
	m_pointerEvent.CurrentPointer.Position.Set(-1, -1);
	m_pointerEvent.CurrentPointer.Buttons = 0;
	m_pointerEvent.CurrentPointer.Pressure = 0.0f;
	m_pointerEvent.CurrentPointer.Time = Timer::Current();

	pFrameHandler->AddRef();
}
CFrameView::~CFrameView()
{
	if (m_hWnd) CloseWindow(m_hWnd);
	UnregisterWindowClass();

	m_pFrameHandler->Release();
	m_pApplicationHandler->Release();
	m_pKeyboardHandler->Release();
	m_pPointerHandler->Release();
}

bool CFrameView::Initialize(const Xe::Core::FrameViewInitDesc& properties)
{
	m_atom = RegisterWindowClass();
	if (m_atom == 0)
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
	if (m_hWnd == nullptr)
		return false;

	HMODULE hModuleUser32 = LoadLibraryExW(L"USER32.DLL", NULL, 0);
	HMODULE hModuleShcore = LoadLibraryExW(L"USER32.DLL", NULL, 0);
	// Enable DPI support
	PFNSETPROCESSDPIAWARENESSPROC pfnSetProcessDpiAwareness =
		(PFNSETPROCESSDPIAWARENESSPROC)GetProcAddress(hModuleShcore, "SetProcessDpiAwareness");
	if (pfnSetProcessDpiAwareness)
	{
		pfnSetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	}
	else
	{
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

bool CFrameView::Run()
{
	bool result = m_pApplicationHandler->OnInitialize();
	if (result)
	{
		m_pApplicationHandler->OnRun();
	}
	m_pApplicationHandler->OnDestroy();

	return result;
}

void CFrameView::SetApplicationHandler(Xe::Core::IApplicationHandler* pApplicationHandler)
{
	m_pApplicationHandler->Release();
	m_pApplicationHandler = pApplicationHandler;
	if (m_pApplicationHandler != nullptr)
	{
		m_pApplicationHandler->AddRef();
	}
	else
	{
		m_pApplicationHandler = new DummyApplicationHandler;
	}
}

void CFrameView::SetKeyboardHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler)
{
	m_pKeyboardHandler->Release();
	m_pKeyboardHandler = pKeyboardHandler;
	if (m_pKeyboardHandler != nullptr)
	{
		m_pKeyboardHandler->AddRef();
	}
	else
	{
		m_pKeyboardHandler = new DummyKeyboardHandler;
	}
}

void CFrameView::SetPointerHandler(Xe::Core::IPointerHandler* pPointerHandler)
{
	m_pPointerHandler->Release();
	m_pPointerHandler = pPointerHandler;
	if (m_pPointerHandler != nullptr)
	{
		m_pPointerHandler->AddRef();
	}
	else
	{
		m_pPointerHandler = new DummyPointerHandler;
	}
}

bool CFrameView::DispatchEvents(DispatchType type)
{
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

void CFrameView::SetTitle(const String& title)
{
	SetTitleT(title.GetData());
}

Graphics::Size CFrameView::GetSize() const
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return Size(rect.right - rect.left, rect.bottom - rect.top);
}

bool CFrameView::SetSize(const Graphics::Size& size)
{
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
		(DWORD)GetWindowLongPtr(m_hWnd, GWL_EXSTYLE)))
	{
		if (SetWindowPos(m_hWnd, 0, 0, 0, (int)size.x + ptDiff.x, (int)size.y + ptDiff.y,
			SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER))
			return true;
	}
	return false;
}

void CFrameView::SetFullScreen(bool fullScreen)
{
	if (fullScreen)
	{
		GetWindowPlacement(m_hWnd, &m_OriginalWindowPlacement);
		if (m_style == 0)
			m_style = GetWindowLong(m_hWnd, GWL_STYLE);
		if (m_styleEx == 0)
			m_styleEx = GetWindowLong(m_hWnd, GWL_EXSTYLE);

		LONG style = m_style;
		style &= ~WS_BORDER;
		style &= ~WS_DLGFRAME;
		style &= ~WS_THICKFRAME;

		LONG styleEx = m_styleEx;
		styleEx &= ~WS_EX_WINDOWEDGE;

		SetWindowLong(m_hWnd, GWL_STYLE, style | WS_POPUP);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, styleEx | WS_EX_TOPMOST);
		ShowWindow(m_hWnd, SW_SHOWMAXIMIZED);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, m_style);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, m_styleEx);
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		SetWindowPlacement(m_hWnd, &m_OriginalWindowPlacement);
	}
}

Xe::Core::Orientation CFrameView::GetOrientation() const
{
	return Xe::Core::Orientation_Unknown;
}

void CFrameView::SetPreferredOrientation(Xe::Core::Orientation orientation)
{

}

float CFrameView::GetScale() const
{
	HDC hDc = GetDC(m_hWnd);
	int xPixelPerInch = GetDeviceCaps(hDc, LOGPIXELSX);
	ReleaseDC(m_hWnd, hDc);
	return (float)xPixelPerInch / 96.0f;
}

void* CFrameView::GetSystemWindow() const
{
	return (void*)m_hWnd;
}

void CFrameView::SetTitleT(const char* title)
{
	SetWindowTextA(m_hWnd, title);
}

void CFrameView::SetTitleT(const wchar_t* title)
{
	SetWindowTextW(m_hWnd, title);
}

ATOM CFrameView::RegisterWindowClass()
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

void CFrameView::UnregisterWindowClass()
{
	if (--m_classesInstanceCount == 0)
		UnregisterClass(_T("XEWND"), m_hInstance);
}

LRESULT CALLBACK CFrameView::_WndProc(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	CFrameView* pWindow = (CFrameView*)GetWindowLongPtr(hWnd, 0);
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);
		CFrameView* pWindow = (CFrameView*)cs->lpCreateParams;
	}

	return pWindow ? pWindow->WndProc(hWnd, uMsg, wParam, lParam) :
		DefWindowProc(hWnd, uMsg, wParam, lParam);
}