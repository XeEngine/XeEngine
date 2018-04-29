#include "pch.h"
#ifdef PLATFORM_WINRT
#include "XeCoreView.h"
#include <XeSDK/XeMath.h>
#pragma comment(lib, "RuntimeObject.lib")

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel;
using namespace ABI::Windows::ApplicationModel::Core;
using namespace ABI::Windows::ApplicationModel::Activation;
using namespace ABI::Windows::Graphics::Display;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Windows::UI::ViewManagement;


typedef IEventHandler<SuspendingEventArgs*> SuspendingEvent;
typedef IEventHandler<IInspectable*> ResumingEvent;
typedef ITypedEventHandler<CoreApplicationView *, IActivatedEventArgs *> IActivatedEventHandler;
typedef ITypedEventHandler<CoreWindow*, WindowActivatedEventArgs*>  ActivatedEvent;
typedef ITypedEventHandler<CoreWindow*, CharacterReceivedEventArgs*> CharacterReceivedEvent;
typedef ITypedEventHandler<CoreWindow*, CoreWindowEventArgs*> ClosedEvent;
typedef ITypedEventHandler<CoreWindow*, WindowSizeChangedEventArgs*> SizeChangedEvent;
typedef ITypedEventHandler<CoreWindow*, VisibilityChangedEventArgs*> VisibilityChangedEvent;
typedef ITypedEventHandler<CoreWindow*, KeyEventArgs*> KeyEvent;
typedef ITypedEventHandler<CoreWindow*, PointerEventArgs*> PointerEvent;
typedef ITypedEventHandler<CoreWindow*, PointerEventArgs*> Display;

typedef ITypedEventHandler<DisplayInformation*, IInspectable*> DisplayEvent;

#define HR

template <typename T, unsigned Count>
auto GetActivationFactory(WCHAR const (&classId)[Count]) -> ComPtr<T>
{
	HSTRING_HEADER header;
	HSTRING string;
	HR(WindowsCreateStringReference(classId,
		Count - 1,
		&header,
		&string));
	ComPtr<T> result;
	HR(RoGetActivationFactory(string,
		__uuidof(T),
		reinterpret_cast<void **>(result.GetAddressOf())));
	return result;
}

namespace Xe {
	namespace Core {
		struct CFrameView :
			public IFrameworkViewSource,
			IFrameworkView,
			IActivatedEventHandler,
			public IFrameView {

			ULONG m_ref;
			ComPtr<ICoreWindow> m_Window;
			ComPtr<IApplicationView> m_ApplicationView;
#ifndef PLATFORM_WINAPP
#ifdef PLATFORM_WINUNIVERSAL
			ComPtr<IApplicationView3> m_ApplicationView3;
#endif
#endif
			ComPtr<IDisplayInformationStatics> m_DisplayInformationStatics;
			ComPtr<ICoreDispatcher> m_Dispatcher;
			bool m_IsClosed;
			Xe::Core::IView *m_pView;
			const IView::InitProperties& m_Properties;
			Xe::Graphics::Size m_Size;
			int m_Width, m_Height;
			Xe::Graphics::Orientation m_Orientation;
			float m_Scale;

			///////////////////////////////////////////////////////////////////////////
			// IUnknown
			auto WINAPI QueryInterface(IID const & id, void ** result) -> HRESULT
			{
				if (result == nullptr) return E_POINTER;
				if (id == __uuidof(IFrameworkViewSource) ||
					id == __uuidof(IInspectable) ||
					id == __uuidof(IUnknown))
				{
					*result = static_cast<IFrameworkViewSource *>(this);
				}
				else if (id == __uuidof(IFrameworkView))
				{
					*result = static_cast<IFrameworkView *>(this);
				}
				else if (id == __uuidof(IActivatedEventHandler))
				{
					*result = static_cast<IActivatedEventHandler *>(this);
				}
				else
				{
					*result = nullptr;
					return E_NOINTERFACE;
				}
				static_cast<IUnknown *>(*result)->AddRef();
				return S_OK;
			}
			auto WINAPI AddRef()  -> ULONG { return ++m_ref; }
			auto WINAPI Release() -> ULONG
			{
				if (--m_ref) return m_ref;
				delete this;
				return 0;
			}
			///////////////////////////////////////////////////////////////////////////
			// IInspectable
			auto WINAPI GetIids(ULONG *, IID **) -> HRESULT
			{
				return E_NOTIMPL;
			}
			auto WINAPI GetRuntimeClassName(HSTRING *) -> HRESULT
			{
				return E_NOTIMPL;
			}
			auto WINAPI GetTrustLevel(TrustLevel *) -> HRESULT
			{
				return E_NOTIMPL;
			}
			///////////////////////////////////////////////////////////////////////////
			// IFrameworkViewSource
			auto WINAPI CreateView(IFrameworkView ** result) -> HRESULT
			{
				*result = this;
				(*result)->AddRef();
				return S_OK;
			}
			///////////////////////////////////////////////////////////////////////////
			// IFrameworkView
			auto WINAPI Initialize(ICoreApplicationView * view) -> HRESULT
			{
				EventRegistrationToken token;
				view->add_Activated(this, &token);
				view->add_Activated(Callback<IActivatedEventHandler>(this, &CFrameView::OnActivated).Get(), &token);

				auto CoreApplication = GetActivationFactory<ICoreApplication>(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);
				CoreApplication->add_Suspending(Callback<SuspendingEvent>(this, &CFrameView::OnSuspending).Get(), &token);
				CoreApplication->add_Resuming(Callback<ResumingEvent>(this, &CFrameView::OnResuming).Get(), &token);

				return S_OK;
			}
			auto WINAPI SetWindow(ICoreWindow * window) -> HRESULT
			{
				EventRegistrationToken token;

				m_Window = window;
				m_Window->add_Closed(Callback<ClosedEvent>(this, &CFrameView::OnClosed).Get(), &token);
				m_Window->add_SizeChanged(Callback<SizeChangedEvent>(this, &CFrameView::OnSizeChanged).Get(), &token);
				m_Window->add_VisibilityChanged(Callback<VisibilityChangedEvent>(this, &CFrameView::OnVisibilityChanged).Get(), &token);

				m_Window->add_CharacterReceived(Callback<CharacterReceivedEvent>(this, &CFrameView::OnCharacterReceived).Get(), &token);
				m_Window->add_KeyDown(Callback<KeyEvent>(this, &CFrameView::OnKeyDown).Get(), &token);
				m_Window->add_KeyUp(Callback<KeyEvent>(this, &CFrameView::OnKeyUp).Get(), &token);

				m_Window->add_PointerCaptureLost(Callback<PointerEvent>(this, &CFrameView::OnPointerCaptureLost).Get(), &token);
				m_Window->add_PointerEntered(Callback<PointerEvent>(this, &CFrameView::OnPointerEntered).Get(), &token);
				m_Window->add_PointerExited(Callback<PointerEvent>(this, &CFrameView::OnPointerExited).Get(), &token);
				m_Window->add_PointerMoved(Callback<PointerEvent>(this, &CFrameView::OnPointerMoved).Get(), &token);
				m_Window->add_PointerPressed(Callback<PointerEvent>(this, &CFrameView::OnPointerPressed).Get(), &token);
				m_Window->add_PointerReleased(Callback<PointerEvent>(this, &CFrameView::OnPointerReleased).Get(), &token);
				m_Window->add_PointerWheelChanged(Callback<PointerEvent>(this, &CFrameView::OnPointerWheelChanged).Get(), &token);

#ifdef PLATFORM_WINUNIVERSAL
				auto ApplicationViewStatic3 = GetActivationFactory<IApplicationViewStatics3>(RuntimeClass_Windows_UI_ViewManagement_ApplicationView);
				if (ApplicationViewStatic3) {
					if (m_Properties.Size.x > 0 && m_Properties.Size.y > 0) {
						ABI::Windows::Foundation::Size size;
						size.Width = (FLOAT)m_Properties.Size.x;
						size.Height = (FLOAT)m_Properties.Size.y;
						ApplicationViewStatic3->put_PreferredLaunchViewSize(size);
					}
					ApplicationViewWindowingMode windowingMode;
					if (m_Properties.IsFullscreen) windowingMode = ApplicationViewWindowingMode_FullScreen;
					else windowingMode = ApplicationViewWindowingMode_PreferredLaunchViewSize;
					ApplicationViewStatic3->put_PreferredLaunchWindowingMode(windowingMode);
				}
#endif
				auto ApplicationViewStatic2 = GetActivationFactory<IApplicationViewStatics2>(RuntimeClass_Windows_UI_ViewManagement_ApplicationView);
				if (ApplicationViewStatic2) ApplicationViewStatic2->GetForCurrentView(&m_ApplicationView);
#ifndef PLATFORM_WINAPP
				if (m_ApplicationView) {
					ComPtr<IApplicationView2> ApplicationView2;
					if (m_ApplicationView.As(&ApplicationView2) == S_OK)
						ApplicationView2->put_SuppressSystemOverlays((boolean)m_Properties.IsFullscreen);
#ifdef PLATFORM_WINUNIVERSAL
					if (m_ApplicationView.As(&m_ApplicationView3) == S_OK) {
						FullScreenSystemOverlayMode overlayMode;
						if (m_Properties.IsFullscreen) overlayMode = FullScreenSystemOverlayMode_Minimal;
						else overlayMode = FullScreenSystemOverlayMode_Standard;
						m_ApplicationView3->put_FullScreenSystemOverlayMode(overlayMode);
						boolean success;
						if (m_Properties.IsFullscreen) m_ApplicationView3->TryEnterFullScreenMode(&success);
					}
#endif
				}
#endif
				
				float dpi;
				IDisplayInformation *pDisplayInformation;
				m_DisplayInformationStatics = GetActivationFactory<IDisplayInformationStatics>(RuntimeClass_Windows_Graphics_Display_DisplayInformation);
				m_DisplayInformationStatics->add_DisplayContentsInvalidated(Callback<DisplayEvent>(this, &CFrameView::OnDisplayContentsInvalidated).Get(), &token);
				m_DisplayInformationStatics->GetForCurrentView(&pDisplayInformation);
				pDisplayInformation->add_DpiChanged(Callback<DisplayEvent>(this, &CFrameView::OnDpiChanged).Get(), &token);
				pDisplayInformation->add_OrientationChanged(Callback<DisplayEvent>(this, &CFrameView::OnOrientationChanged).Get(), &token);
				pDisplayInformation->get_LogicalDpi(&dpi);
				pDisplayInformation->Release();

				m_Scale = dpi / 96.0f;

				Rect rect;
				m_Window->get_Bounds(&rect);
				m_Size.x = (svar)Math::Round(rect.Width * m_Scale);
				m_Size.y = (svar)Math::Round(rect.Height * m_Scale);
				return S_OK;
			}
			auto WINAPI Load(HSTRING) -> HRESULT
			{
				// Load all the shit here, during splashscreen
				return S_OK;
			}
			auto WINAPI Invoke(ICoreApplicationView *, IActivatedEventArgs *) -> HRESULT
			{
				m_Window->Activate();
				return S_OK;
			}
			HRESULT WINAPI Run()
			{
				m_Window->get_Dispatcher(m_Dispatcher.GetAddressOf());
				if (m_pView->OnInitialize())
					m_pView->OnRun();
				m_pView->OnDestroy();
				return S_OK;
			}
			HRESULT WINAPI Uninitialize()
			{
				// Called only if Run exits
				return S_OK;
			}
			///////////////////////////////////////////////////////////////////////////
			// IFrameView implementation
			bool DispatchEvents(DispatchType type) {
				CoreProcessEventsOption option;
				switch (type) {
				case DispatchType_Exit:
					return false;
				case DispatchType_None:
					return !m_IsClosed;
				case DispatchType_One:
					option = CoreProcessEventsOption_ProcessOneIfPresent;
					break;
				case DispatchType_All:
					option = CoreProcessEventsOption_ProcessAllIfPresent;
					break;
				case DispatchType_WaitOnce:
					option = CoreProcessEventsOption_ProcessOneAndAllPending;
					break;
				case DispatchType_WaitUntilQuit:
					option = CoreProcessEventsOption_ProcessUntilQuit;
					break;
				default:
					return !m_IsClosed;
				}
				m_Dispatcher->ProcessEvents(option);
				return !m_IsClosed;
			}
			void SetTitle(const String& title) {
				HSTRING_HEADER header;
				HSTRING string;
				if (WindowsCreateStringReference(title, title.GetLength(), &header, &string) == S_OK)
					m_ApplicationView->put_Title(string);
			}
			Graphics::Size GetSize() const {
				return m_Size;
			}
			bool SetSize(const Graphics::Size& size) {
#ifdef PLATFORM_WINUNIVERSAL
				ABI::Windows::Foundation::Size newsize;
				newsize.Width = (FLOAT)size.x;
				newsize.Height = (FLOAT)size.y;
				boolean success;
				if (m_ApplicationView3->TryResizeView(newsize, &success) == S_OK)
					return success != 0;
#endif
				return false;
			}
			Xe::Graphics::Orientation GetOrientation() const {
				return m_Orientation;
			}
			void SetPreferredOrientation(Xe::Graphics::Orientation orientation) {
				SetPreferredOrientation(m_DisplayInformationStatics.Get(), orientation);
			}
			float GetScale() const {
				return m_Scale;
			}

			///////////////////////////////////////////////////////////////////////////
			// CoreApplication events
			HRESULT OnSuspending(IInspectable*, ISuspendingEventArgs* args) {
				m_pView->OnSuspend();
				return S_OK;
			}
			HRESULT OnResuming(IInspectable*, IInspectable*) {
				m_pView->OnResume();
				return S_OK;
			}
			///////////////////////////////////////////////////////////////////////////
			// IFrameworkView events
			HRESULT OnActivated(ICoreApplicationView*, IActivatedEventArgs* args) {
				// Before this, an useless OnActivated : CodeActivated is sent. We won't this.
				EventRegistrationToken token;
				m_Window->add_Activated(Callback<ActivatedEvent>(this, &CFrameView::OnWindowActivated).Get(), &token);
				return S_OK;
			}
			///////////////////////////////////////////////////////////////////////////
			// ICoreWindow events
			HRESULT OnWindowActivated(ICoreWindow* pCoreWindow, IWindowActivatedEventArgs* args) {
				CoreWindowActivationState state;
				args->get_WindowActivationState(&state);
				switch (state) {
				case CoreWindowActivationState_CodeActivated:
					m_pView->OnFocusGot();
					break;
				case CoreWindowActivationState_Deactivated:
					m_pView->OnFocusLost();
					break;
				case CoreWindowActivationState_PointerActivated:
					m_pView->OnFocusGot();
					break;
				}
				return S_OK;
			}
			HRESULT OnClosed(ICoreWindow* pCoreWindow, ICoreWindowEventArgs* args) {
				return S_OK;
			}
			HRESULT OnSizeChanged(ICoreWindow* pCoreWindow, IWindowSizeChangedEventArgs* args) {
				ABI::Windows::Foundation::Size size;
				args->get_Size(&size);
				m_Size.x = (svar)Math::Round(size.Width * m_Scale);
				m_Size.y = (svar)Math::Round(size.Height * m_Scale);
				m_pView->OnSizeChanged(m_Size);
				return S_OK;
			}
			HRESULT OnVisibilityChanged(ICoreWindow* pCoreWindow, IVisibilityChangedEventArgs* args) {
				boolean visible;
				args->get_Visible(&visible);
				m_pView->OnVisibilityChanged(visible != 0);
				return S_OK;
			}
			HRESULT OnCharacterReceived(ICoreWindow* pCoreWindow, ICharacterReceivedEventArgs* args) {
				UINT32 code;
				CorePhysicalKeyStatus status;
				args->get_KeyCode(&code);
				args->get_KeyStatus(&status);

				Xe::IO::CharacterEvent e;
				e.Character = code;
				m_pView->OnCharacter(e);
				return S_OK;
			}
			HRESULT OnKeyDown(ICoreWindow* pCoreWindow, IKeyEventArgs* args) {
				ABI::Windows::UI::Core::CorePhysicalKeyStatus status;
				ABI::Windows::System::VirtualKey vk;
				args->get_KeyStatus(&status);
				args->get_VirtualKey(&vk);

				IO::KeyboardEvent e;
				e.ScanCode = status.ScanCode;
				e.VirtualCode = vk;
				m_pView->OnKeyPressed(e);
				return S_OK;
			}
			HRESULT OnKeyUp(ICoreWindow* pCoreWindow, IKeyEventArgs* args) {
				ABI::Windows::UI::Core::CorePhysicalKeyStatus status;
				ABI::Windows::System::VirtualKey vk;
				args->get_KeyStatus(&status);
				args->get_VirtualKey(&vk);

				IO::KeyboardEvent e;
				e.ScanCode = status.ScanCode;
				e.VirtualCode = vk;
				m_pView->OnKeyReleased(e);
				return S_OK;
			}
			HRESULT OnPointerCaptureLost(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}
			HRESULT OnPointerEntered(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}
			HRESULT OnPointerExited(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}
			HRESULT OnPointerMoved(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}
			HRESULT OnPointerPressed(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}
			HRESULT OnPointerReleased(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}
			HRESULT OnPointerWheelChanged(ICoreWindow* pCoreWindow, IPointerEventArgs* args) {
				return S_OK;
			}

			///////////////////////////////////////////////////////////////////////////
			// DisplayInformation events
			HRESULT OnDisplayContentsInvalidated(IDisplayInformation* pDisplayInformation, IInspectable* value) {
				m_pView->OnDraw();
				return S_OK;
			}
			HRESULT OnDpiChanged(IDisplayInformation* pDisplayInformation, IInspectable* value) {
				float dpi;
				pDisplayInformation->get_LogicalDpi(&dpi);
				m_Scale = dpi / 96.0f;
				m_pView->OnDpiChanged(m_Scale);
				return S_OK;
			}
			HRESULT OnOrientationChanged(IDisplayInformation* pDisplayInformation, IInspectable* value) {
				m_Orientation = GetOrientation(pDisplayInformation);
				m_pView->OnOrientationChanged(m_Orientation);
				return S_OK;
			}

			///////////////////////////////////////////////////////////////////////////
			// Other stuff
			static Xe::Graphics::Orientation GetOrientation(IDisplayInformation* pDisplayInformation) {
				DisplayOrientations displayOrientation;
				pDisplayInformation->get_CurrentOrientation(&displayOrientation);
				switch (displayOrientation) {
				case DisplayOrientations_None:
					return Xe::Graphics::Orientation_Unknown;
				case DisplayOrientations_Landscape:
					return Xe::Graphics::Orientation_Landscape;
				case DisplayOrientations_Portrait:
					return Xe::Graphics::Orientation_Portrait;
				case DisplayOrientations_LandscapeFlipped:
					return Xe::Graphics::Orientation_LandscapeFlipped;
				case DisplayOrientations_PortraitFlipped:
					return Xe::Graphics::Orientation_PortraitFlipped;
				default:
					return Xe::Graphics::Orientation_Unknown;
				}
			}
			static void SetPreferredOrientation(IDisplayInformationStatics* pDisplayInformation, Xe::Graphics::Orientation orientation) {
				DisplayOrientations displayOrientation;
				switch (orientation) {
				case Xe::Graphics::Orientation_Unknown:
					displayOrientation = DisplayOrientations_None;
					break;
				case Xe::Graphics::Orientation_Landscape:
					displayOrientation = DisplayOrientations_Landscape;
					break;
				case Xe::Graphics::Orientation_Portrait:
					displayOrientation = DisplayOrientations_Portrait;
					break;
				case Xe::Graphics::Orientation_LandscapeFlipped:
					displayOrientation = DisplayOrientations_LandscapeFlipped;
					break;
				case Xe::Graphics::Orientation_PortraitFlipped:
					displayOrientation = DisplayOrientations_PortraitFlipped;
					break;
				default:
					displayOrientation = DisplayOrientations_None;
				}
				pDisplayInformation->put_AutoRotationPreferences(displayOrientation);
			}

		public:
			CFrameView(IView *pView, const IView::InitProperties& properties) :
				m_ref(1),
				m_IsClosed(false),
				m_pView(pView),
				m_Properties(properties) {
			}
			~CFrameView() {
			}
		};
	}
}

namespace Xe {
	namespace Core {
		void IView::Run(IView *pView, const IView::InitProperties& properties) {
			auto CoreApplication = GetActivationFactory<ICoreApplication>(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);
			CFrameView *pFrameView = new CFrameView(pView, properties);
			pView->m_FrameView = pFrameView;
			CoreApplication->Run(pFrameView);
			delete pFrameView;
		}
		void GetViewInternal(IView* pView, ViewInternal& viewInternal) {
			CFrameView *pFrameView = (CFrameView*)pView->m_FrameView;
			ComPtr<IUnknown> unk;
			pFrameView->m_Window.As(&unk);
			viewInternal.Window = unk.Get();
		}
	}
}
#endif