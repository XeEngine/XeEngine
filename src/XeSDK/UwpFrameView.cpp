#include "pch.h"

#if WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE

#include <XeSDK/XeMath.h>
#include "UwpFrameView.h"
#include "WinRtUtilities.h"
#include "DummyApplicationHandler.h"
#include "DummyFrameHandler.h"
#include "DummyKeyboardHandler.h"
#include "DummyPointerHandler.h"
#pragma comment(lib, "RuntimeObject.lib")

#define HR(x) (ASSERT((x) == S_OK))

using namespace Xe;
using namespace Xe::Core;

UwpFrameView::UwpFrameView(IFrameHandler* pFrameHandler) :
	m_Orientation(Orientation_Unknown),
	m_Scale(1.0f),
	m_IsFullscreen(false),
	m_IsClosed(false),
	m_pFrameHandler(pFrameHandler),
	m_pApplicationHandler(new DummyApplicationHandler),
	m_pKeyboardHandler(new DummyKeyboardHandler),
	m_pPointerHandler(new DummyPointerHandler)
{
	m_pFrameHandler->AddRef();
}
UwpFrameView::~UwpFrameView()
{
	m_pFrameHandler->Release();
	m_pApplicationHandler->Release();
	m_pKeyboardHandler->Release();
	m_pPointerHandler->Release();
}

bool UwpFrameView::Initialize(const Xe::Core::FrameViewInitDesc& frameInitDesc)
{
	m_Size = frameInitDesc.Size;
	m_IsFullscreen = frameInitDesc.IsFullscreen;

	return true;
}

#pragma region IFrameView

void UwpFrameView::SetApplicationHandler(Xe::Core::IApplicationHandler* pApplicationHandler)
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

void UwpFrameView::SetKeyboardHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler)
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

void UwpFrameView::SetPointerHandler(Xe::Core::IPointerHandler* pPointerHandler)
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

bool UwpFrameView::DispatchEvents(Xe::Core::DispatchType type)
{
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

void UwpFrameView::SetTitle(const Xe::String& title)
{
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
	HSTRING_HEADER header;
	HSTRING string;

	int strLength = Xe::String::GetLength(title);
	wchar_t* strW = new wchar_t[strLength + 1];
	mbtowc(strW, title, strLength + 1);

	if (WindowsCreateStringReference(strW, title.GetLength(), &header, &string) == S_OK)
	{
		m_ApplicationView->put_Title(string);
	}

	delete[] strW;
#endif
}

Xe::Graphics::Size UwpFrameView::GetSize() const
{
	return m_Size;
}

bool UwpFrameView::SetSize(const Xe::Graphics::Size& size)
{
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
	ABI::Windows::Foundation::Size newsize;
	newsize.Width = (FLOAT)size.x;
	newsize.Height = (FLOAT)size.y;
	boolean success;
	return m_ApplicationView3->TryResizeView(newsize, &success) == S_OK;
#else
	return false;
#endif
}

void UwpFrameView::SetFullScreen(bool fullScreen)
{
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
	boolean success;
	m_ApplicationView3->TryEnterFullScreenMode(&success);
#endif
}

Xe::Core::Orientation UwpFrameView::GetOrientation() const
{
	return m_Orientation;
}

void UwpFrameView::SetPreferredOrientation(Xe::Core::Orientation orientation)
{
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
	SetPreferredOrientation(m_DisplayInformationStatics.Get(), orientation);
#endif
}

float UwpFrameView::GetScale() const
{
	return m_Scale;
}

void* UwpFrameView::GetSystemWindow() const
{
	ComPtr<IUnknown> unk;
	while (m_Window == nullptr)
	{
		SleepEx(1, TRUE);
	}

	m_Window.As(&unk);
	return unk.Get();
}

#pragma endregion

#pragma region IUnknown

HRESULT WINAPI UwpFrameView::QueryInterface(IID const & id, void ** result)
{
	if (result == nullptr) return E_POINTER;
	if (id == __uuidof(IInspectable) ||
		id == __uuidof(IUnknown))
	{
		*result = static_cast<IInspectable *>(this);
	}
	else if (id == __uuidof(IFrameworkView))
	{
		*result = static_cast<IFrameworkView *>(this);
	}
	else
	{
		*result = nullptr;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown *>(*result)->AddRef();
	return S_OK;
}

ULONG WINAPI UwpFrameView::AddRef()
{
	return ++m_ref;
}

ULONG WINAPI UwpFrameView::Release()
{
	if (--m_ref) return m_ref;
	delete this;
	return 0;
}


#pragma endregion

#pragma region IInspectable

HRESULT WINAPI UwpFrameView::GetIids(ULONG *, IID **)
{
	return E_NOTIMPL;
}

HRESULT WINAPI UwpFrameView::GetRuntimeClassName(HSTRING *)
{
	return E_NOTIMPL;
}

HRESULT WINAPI UwpFrameView::GetTrustLevel(TrustLevel *)
{
	return E_NOTIMPL;
}

#pragma endregion

#pragma region IFrameworkview

HRESULT WINAPI UwpFrameView::Initialize(ICoreApplicationView * view)
{
	EventRegistrationToken token;
	view->add_Activated(this, &token);
	view->add_Activated(Callback<IActivatedEventHandler>(this, &UwpFrameView::OnActivated).Get(), &token);

	auto CoreApplication = GetActivationFactory<ICoreApplication>(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);
	CoreApplication->add_Suspending(Callback<SuspendingEvent>(this, &UwpFrameView::OnSuspending).Get(), &token);
	CoreApplication->add_Resuming(Callback<ResumingEvent>(this, &UwpFrameView::OnResuming).Get(), &token);
	auto asd = Callback<SuspendingEvent>(this, &UwpFrameView::OnSuspending);

	return S_OK;
}
HRESULT WINAPI UwpFrameView::SetWindow(ICoreWindow * window)
{
#if _XBOX_ONE
	SetThreadAffinityMask(GetCurrentThread(), 0x1);
#endif

	EventRegistrationToken token;
	m_Window = window;
	m_Window->add_Closed(Callback<ClosedEvent>(this, &UwpFrameView::OnClosed).Get(), &token);
	m_Window->add_SizeChanged(Callback<SizeChangedEvent>(this, &UwpFrameView::OnSizeChanged).Get(), &token);
	m_Window->add_VisibilityChanged(Callback<VisibilityChangedEvent>(this, &UwpFrameView::OnVisibilityChanged).Get(), &token);

	m_Window->add_CharacterReceived(Callback<CharacterReceivedEvent>(this, &UwpFrameView::OnCharacterReceived).Get(), &token);
	m_Window->add_KeyDown(Callback<KeyEvent>(this, &UwpFrameView::OnKeyDown).Get(), &token);
	m_Window->add_KeyUp(Callback<KeyEvent>(this, &UwpFrameView::OnKeyUp).Get(), &token);

#if WINAPI_FAMILY == WINAPI_FAMILY_APP
	m_Window->add_PointerCaptureLost(Callback<PointerEvent>(this, &UwpFrameView::OnPointerCaptureLost).Get(), &token);
#endif
	m_Window->add_PointerEntered(Callback<PointerEvent>(this, &UwpFrameView::OnPointerEntered).Get(), &token);
	m_Window->add_PointerExited(Callback<PointerEvent>(this, &UwpFrameView::OnPointerExited).Get(), &token);
	m_Window->add_PointerMoved(Callback<PointerEvent>(this, &UwpFrameView::OnPointerMoved).Get(), &token);
	m_Window->add_PointerPressed(Callback<PointerEvent>(this, &UwpFrameView::OnPointerPressed).Get(), &token);
	m_Window->add_PointerReleased(Callback<PointerEvent>(this, &UwpFrameView::OnPointerReleased).Get(), &token);
	m_Window->add_PointerWheelChanged(Callback<PointerEvent>(this, &UwpFrameView::OnPointerWheelChanged).Get(), &token);
	
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
	auto ApplicationViewStatic3 = GetActivationFactory<IApplicationViewStatics3>(RuntimeClass_Windows_UI_ViewManagement_ApplicationView);
	if (ApplicationViewStatic3)
	{
		if (m_Size.x > 0 && m_Size.y > 0) {
			ABI::Windows::Foundation::Size size;
			size.Width = (FLOAT)m_Size.x;
			size.Height = (FLOAT)m_Size.y;
			ApplicationViewStatic3->put_PreferredLaunchViewSize(size);
		}

		ApplicationViewWindowingMode windowingMode;
		if (m_IsFullscreen) windowingMode = ApplicationViewWindowingMode_FullScreen;
		else windowingMode = ApplicationViewWindowingMode_PreferredLaunchViewSize;
		ApplicationViewStatic3->put_PreferredLaunchWindowingMode(windowingMode);
	}

	auto ApplicationViewStatic2 = GetActivationFactory<IApplicationViewStatics2>(RuntimeClass_Windows_UI_ViewManagement_ApplicationView);
	if (ApplicationViewStatic2)
	{
		ApplicationViewStatic2->GetForCurrentView(&m_ApplicationView);
	}

	if (m_ApplicationView)
	{
		if (m_ApplicationView.As(&m_ApplicationView3) == S_OK)
		{
			FullScreenSystemOverlayMode overlayMode = m_IsFullscreen ?
				FullScreenSystemOverlayMode_Minimal : FullScreenSystemOverlayMode_Standard;
			m_ApplicationView3->put_FullScreenSystemOverlayMode(overlayMode);

			if (m_IsFullscreen)
			{
				SetFullScreen(true);
			}
		}
	}

	float dpi;
	IDisplayInformation *pDisplayInformation;
	m_DisplayInformationStatics = GetActivationFactory<IDisplayInformationStatics>(RuntimeClass_Windows_Graphics_Display_DisplayInformation);
	m_DisplayInformationStatics->add_DisplayContentsInvalidated(Callback<DisplayEvent>(this, &UwpFrameView::OnDisplayContentsInvalidated).Get(), &token);
	m_DisplayInformationStatics->GetForCurrentView(&pDisplayInformation);
	pDisplayInformation->add_DpiChanged(Callback<DisplayEvent>(this, &UwpFrameView::OnDpiChanged).Get(), &token);
	pDisplayInformation->add_OrientationChanged(Callback<DisplayEvent>(this, &UwpFrameView::OnOrientationChanged).Get(), &token);
	pDisplayInformation->get_LogicalDpi(&dpi);
	pDisplayInformation->Release();

	m_Scale = dpi / 96.0f;

#endif

	Rect rect;
	m_Window->get_Bounds(&rect);
	m_Size.x = (svar)Math::Round(rect.Width * m_Scale);
	m_Size.y = (svar)Math::Round(rect.Height * m_Scale);
	return S_OK;
}
HRESULT WINAPI UwpFrameView::Load(HSTRING)
{
	// Load all the shit here, during splashscreen
	return S_OK;
}
HRESULT WINAPI UwpFrameView::Invoke(ICoreApplicationView *, IActivatedEventArgs *)
{
	m_Window->Activate();
	return S_OK;
}
HRESULT WINAPI UwpFrameView::Run()
{
	HRESULT hr;

	HR(m_Window->get_Dispatcher(m_Dispatcher.GetAddressOf()));
	if (m_pApplicationHandler->OnInitialize())
	{
		m_pApplicationHandler->OnRun();
		hr = S_OK;
	}
	else
	{
		hr = E_FAIL;
	}
	m_pApplicationHandler->OnDestroy();

	return hr;
}
HRESULT WINAPI UwpFrameView::Uninitialize()
{
	// Called only if Run exits
	return S_OK;
}

#pragma endregion

#pragma region CoreApplication

HRESULT UwpFrameView::OnSuspending(IInspectable*, ISuspendingEventArgs* args)
{
	m_pApplicationHandler->OnSuspend();
	return S_OK;
}
HRESULT UwpFrameView::OnResuming(IInspectable*, IInspectable*)
{
	m_pApplicationHandler->OnResume();
	return S_OK;
}

#pragma endregion

#pragma region IFrameworkView

HRESULT UwpFrameView::OnActivated(ICoreApplicationView*, IActivatedEventArgs* args)
{
	// Before this, an useless OnActivated : CodeActivated is sent. We won't this.
	EventRegistrationToken token;
	m_Window->add_Activated(Callback<ActivatedEvent>(this, &UwpFrameView::OnWindowActivated).Get(), &token);
	return S_OK;
}

#pragma endregion

#pragma region ICoreWindow

HRESULT UwpFrameView::OnWindowActivated(ICoreWindow* pCoreWindow, IWindowActivatedEventArgs* args)
{
	CoreWindowActivationState state;
	args->get_WindowActivationState(&state);
	switch (state) {
	case CoreWindowActivationState_CodeActivated:
		m_pFrameHandler->OnFocusGot();
		break;
	case CoreWindowActivationState_Deactivated:
		m_pFrameHandler->OnFocusLost();
		break;
	case CoreWindowActivationState_PointerActivated:
		m_pFrameHandler->OnFocusGot();
		break;
	}
	return S_OK;
}
HRESULT UwpFrameView::OnClosed(ICoreWindow* pCoreWindow, ICoreWindowEventArgs* args)
{
	return S_OK;
}
HRESULT UwpFrameView::OnSizeChanged(ICoreWindow* pCoreWindow, IWindowSizeChangedEventArgs* args)
{
	ABI::Windows::Foundation::Size size;
	args->get_Size(&size);
	m_Size.x = (svar)Math::Round(size.Width * m_Scale);
	m_Size.y = (svar)Math::Round(size.Height * m_Scale);
	m_pFrameHandler->OnSizeChanged(m_Size);
	return S_OK;
}
HRESULT UwpFrameView::OnVisibilityChanged(ICoreWindow* pCoreWindow, IVisibilityChangedEventArgs* args)
{
	boolean visible;
	args->get_Visible(&visible);
	m_pFrameHandler->OnVisibilityChanged(visible != 0);
	return S_OK;
}
HRESULT UwpFrameView::OnCharacterReceived(ICoreWindow* pCoreWindow, ICharacterReceivedEventArgs* args)
{
	UINT32 code;
	CorePhysicalKeyStatus status;
	args->get_KeyCode(&code);
	args->get_KeyStatus(&status);

	Xe::IO::CharacterEvent e;
	e.Character = code;
	m_pKeyboardHandler->OnCharacter(e);
	return S_OK;
}
HRESULT UwpFrameView::OnKeyDown(ICoreWindow* pCoreWindow, IKeyEventArgs* args)
{
	ABI::Windows::UI::Core::CorePhysicalKeyStatus status;
	ABI::Windows::System::VirtualKey vk;
	args->get_KeyStatus(&status);
	args->get_VirtualKey(&vk);

	IO::KeyboardEvent e;
	e.ScanCode = status.ScanCode;
	e.VirtualCode = vk;
	m_pKeyboardHandler->OnKeyPressed(e);
	return S_OK;
}
HRESULT UwpFrameView::OnKeyUp(ICoreWindow* pCoreWindow, IKeyEventArgs* args)
{
	ABI::Windows::UI::Core::CorePhysicalKeyStatus status;
	ABI::Windows::System::VirtualKey vk;
	args->get_KeyStatus(&status);
	args->get_VirtualKey(&vk);

	IO::KeyboardEvent e;
	e.ScanCode = status.ScanCode;
	e.VirtualCode = vk;
	m_pKeyboardHandler->OnKeyReleased(e);
	return S_OK;
}

#if WINAPI_FAMILY == WINAPI_FAMILY_APP
HRESULT UwpFrameView::OnPointerCaptureLost(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
#endif
HRESULT UwpFrameView::OnPointerEntered(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT UwpFrameView::OnPointerExited(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT UwpFrameView::OnPointerMoved(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT UwpFrameView::OnPointerPressed(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT UwpFrameView::OnPointerReleased(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT UwpFrameView::OnPointerWheelChanged(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
#pragma endregion

#pragma region DisplayInformation

#if WINAPI_FAMILY == WINAPI_FAMILY_APP
HRESULT UwpFrameView::OnDisplayContentsInvalidated(IDisplayInformation* pDisplayInformation, IInspectable* value)
{
	m_pApplicationHandler->OnDraw();
	return S_OK;
}

HRESULT UwpFrameView::OnDpiChanged(IDisplayInformation* pDisplayInformation, IInspectable* value)
{
	float dpi;
	pDisplayInformation->get_LogicalDpi(&dpi);
	m_Scale = dpi / 96.0f;
	m_pFrameHandler->OnDpiChanged(m_Scale);
	return S_OK;
}

HRESULT UwpFrameView::OnOrientationChanged(IDisplayInformation* pDisplayInformation, IInspectable* value)
{
	m_Orientation = GetOrientation(pDisplayInformation);
	m_pFrameHandler->OnOrientationChanged(m_Orientation);
	return S_OK;
}
#endif

#pragma endregion

#pragma region Utilities

#if WINAPI_FAMILY == WINAPI_FAMILY_APP
Xe::Core::Orientation UwpFrameView::GetOrientation(IDisplayInformation* pDisplayInformation)
{
	DisplayOrientations displayOrientation;
	pDisplayInformation->get_CurrentOrientation(&displayOrientation);
	switch (displayOrientation) {
	case DisplayOrientations_None:
		return Xe::Core::Orientation_Unknown;
	case DisplayOrientations_Landscape:
		return Xe::Core::Orientation_Landscape;
	case DisplayOrientations_Portrait:
		return Xe::Core::Orientation_Portrait;
	case DisplayOrientations_LandscapeFlipped:
		return Xe::Core::Orientation_LandscapeFlipped;
	case DisplayOrientations_PortraitFlipped:
		return Xe::Core::Orientation_PortraitFlipped;
	default:
		return Xe::Core::Orientation_Unknown;
	}
}

void UwpFrameView::SetPreferredOrientation(IDisplayInformationStatics* pDisplayInformation, Xe::Core::Orientation orientation)
{
	DisplayOrientations displayOrientation;
	switch (orientation) {
	case Xe::Core::Orientation_Unknown:
		displayOrientation = DisplayOrientations_None;
		break;
	case Xe::Core::Orientation_Landscape:
		displayOrientation = DisplayOrientations_Landscape;
		break;
	case Xe::Core::Orientation_Portrait:
		displayOrientation = DisplayOrientations_Portrait;
		break;
	case Xe::Core::Orientation_LandscapeFlipped:
		displayOrientation = DisplayOrientations_LandscapeFlipped;
		break;
	case Xe::Core::Orientation_PortraitFlipped:
		displayOrientation = DisplayOrientations_PortraitFlipped;
		break;
	default:
		displayOrientation = DisplayOrientations_None;
	}
	pDisplayInformation->put_AutoRotationPreferences(displayOrientation);
}
#endif

#pragma endregion

#endif