#include "pch.h"
#include <XeSDK/XeMath.h>
#include "CFrameView.h"
#include "DummyApplicationHandler.h"
#include "DummyFrameHandler.h"
#include "DummyKeyboardHandler.h"
#include "DummyPointerHandler.h"
#pragma comment(lib, "RuntimeObject.lib")

#define HR(x) (ASSERT((x) == S_OK))

using namespace Xe;
using namespace Xe::Core;

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

CFrameView::CFrameView() :
	m_pApplicationHandler(new DummyApplicationHandler),
	m_pFrameHandler(new DummyFrameHandler),
	m_pKeyboardHandler(new DummyKeyboardHandler),
	m_pPointerHandler(new DummyPointerHandler)
{
}
CFrameView::~CFrameView()
{
	m_pApplicationHandler->Release();
	m_pFrameHandler->Release();
	m_pKeyboardHandler->Release();
	m_pPointerHandler->Release();
}

bool CFrameView::Initialize(const Xe::Core::FrameViewInitDesc& frameInitDesc)
{
	m_Size = frameInitDesc.Size;
	m_IsFullscreen = frameInitDesc.IsFullscreen;

	return true;
}

#pragma region IFrameView

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

void CFrameView::SetFrameHandler(Xe::Core::IFrameHandler* pFrameHandler)
{
	m_pFrameHandler->Release();
	m_pFrameHandler = m_pFrameHandler;
	if (m_pFrameHandler != nullptr)
	{
		m_pFrameHandler->AddRef();
	}
	else
	{
		m_pFrameHandler = new DummyFrameHandler;
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

bool CFrameView::DispatchEvents(Xe::Core::DispatchType type)
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

void CFrameView::SetTitle(const Xe::String& title)
{
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
}

Xe::Graphics::Size CFrameView::GetSize() const
{
	return m_Size;
}

bool CFrameView::SetSize(const Xe::Graphics::Size& size)
{
	ABI::Windows::Foundation::Size newsize;
	newsize.Width = (FLOAT)size.x;
	newsize.Height = (FLOAT)size.y;
	boolean success;
	return m_ApplicationView3->TryResizeView(newsize, &success) == S_OK;
}

void CFrameView::SetFullScreen(bool fullScreen)
{
	boolean success;
	m_ApplicationView3->TryEnterFullScreenMode(&success);
}

Xe::Core::Orientation CFrameView::GetOrientation() const
{
	return m_Orientation;
}

void CFrameView::SetPreferredOrientation(Xe::Core::Orientation orientation)
{
	SetPreferredOrientation(m_DisplayInformationStatics.Get(), orientation);
}

float CFrameView::GetScale() const
{
	return m_Scale;
}

void* CFrameView::GetSystemWindow() const
{
	ComPtr<IUnknown> unk;
	m_Window.As(&unk);
	return unk.Get();
}

#pragma endregion

#pragma region IUnknown

HRESULT WINAPI CFrameView::QueryInterface(IID const & id, void ** result)
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
	else
	{
		*result = nullptr;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown *>(*result)->AddRef();
	return S_OK;
}

ULONG WINAPI CFrameView::AddRef()
{
	return ++m_ref;
}

ULONG WINAPI CFrameView::Release()
{
	if (--m_ref) return m_ref;
	delete this;
	return 0;
}


#pragma endregion

#pragma region IInspectable

HRESULT WINAPI CFrameView::GetIids(ULONG *, IID **)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CFrameView::GetRuntimeClassName(HSTRING *)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CFrameView::GetTrustLevel(TrustLevel *)
{
	return E_NOTIMPL;
}

#pragma endregion

#pragma region IFrameworkViewSource

HRESULT WINAPI CFrameView::CreateView(IFrameworkView ** result)
{
	*result = this;
	(*result)->AddRef();
	return S_OK;
}

#pragma endregion

#pragma region IFrameworkview

HRESULT WINAPI CFrameView::Initialize(ICoreApplicationView * view)
{
	EventRegistrationToken token;
	view->add_Activated(Callback<IActivatedEventHandler>(this, &CFrameView::OnActivated).Get(), &token);

	auto CoreApplication = GetActivationFactory<ICoreApplication>(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);
	CoreApplication->add_Suspending(Callback<SuspendingEvent>(this, &CFrameView::OnSuspending).Get(), &token);
	CoreApplication->add_Resuming(Callback<ResumingEvent>(this, &CFrameView::OnResuming).Get(), &token);

	return S_OK;
}
HRESULT WINAPI CFrameView::SetWindow(ICoreWindow * window)
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
auto WINAPI CFrameView::Load(HSTRING) -> HRESULT
{
	// Load all the shit here, during splashscreen
	return S_OK;
}
auto WINAPI CFrameView::Invoke(ICoreApplicationView *, IActivatedEventArgs *) -> HRESULT
{
	m_Window->Activate();
	return S_OK;
}
HRESULT WINAPI CFrameView::Run()
{
	m_Window->get_Dispatcher(m_Dispatcher.GetAddressOf());
	if (m_pApplicationHandler->OnInitialize())
		m_pApplicationHandler->OnRun();
	m_pApplicationHandler->OnDestroy();
	return S_OK;
}
HRESULT WINAPI CFrameView::Uninitialize()
{
	// Called only if Run exits
	return S_OK;
}

#pragma endregion

#pragma region CoreApplication

HRESULT CFrameView::OnSuspending(IInspectable*, ISuspendingEventArgs* args)
{
	m_pApplicationHandler->OnSuspend();
	return S_OK;
}
HRESULT CFrameView::OnResuming(IInspectable*, IInspectable*)
{
	m_pApplicationHandler->OnResume();
	return S_OK;
}

#pragma endregion

#pragma region IFrameworkView

HRESULT CFrameView::OnActivated(ICoreApplicationView*, IActivatedEventArgs* args)
{
	// Before this, an useless OnActivated : CodeActivated is sent. We won't this.
	EventRegistrationToken token;
	m_Window->add_Activated(Callback<ActivatedEvent>(this, &CFrameView::OnWindowActivated).Get(), &token);
	return S_OK;
}

#pragma endregion

#pragma region ICoreWindow

HRESULT CFrameView::OnWindowActivated(ICoreWindow* pCoreWindow, IWindowActivatedEventArgs* args)
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
HRESULT CFrameView::OnClosed(ICoreWindow* pCoreWindow, ICoreWindowEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnSizeChanged(ICoreWindow* pCoreWindow, IWindowSizeChangedEventArgs* args)
{
	ABI::Windows::Foundation::Size size;
	args->get_Size(&size);
	m_Size.x = (svar)Math::Round(size.Width * m_Scale);
	m_Size.y = (svar)Math::Round(size.Height * m_Scale);
	m_pFrameHandler->OnSizeChanged(m_Size);
	return S_OK;
}
HRESULT CFrameView::OnVisibilityChanged(ICoreWindow* pCoreWindow, IVisibilityChangedEventArgs* args)
{
	boolean visible;
	args->get_Visible(&visible);
	m_pFrameHandler->OnVisibilityChanged(visible != 0);
	return S_OK;
}
HRESULT CFrameView::OnCharacterReceived(ICoreWindow* pCoreWindow, ICharacterReceivedEventArgs* args)
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
HRESULT CFrameView::OnKeyDown(ICoreWindow* pCoreWindow, IKeyEventArgs* args)
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
HRESULT CFrameView::OnKeyUp(ICoreWindow* pCoreWindow, IKeyEventArgs* args)
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
HRESULT CFrameView::OnPointerCaptureLost(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnPointerEntered(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnPointerExited(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnPointerMoved(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnPointerPressed(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnPointerReleased(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
HRESULT CFrameView::OnPointerWheelChanged(ICoreWindow* pCoreWindow, IPointerEventArgs* args)
{
	return S_OK;
}
#pragma endregion

#pragma region DisplayInformation

HRESULT CFrameView::OnDisplayContentsInvalidated(IDisplayInformation* pDisplayInformation, IInspectable* value)
{
	m_pApplicationHandler->OnDraw();
	return S_OK;
}

HRESULT CFrameView::OnDpiChanged(IDisplayInformation* pDisplayInformation, IInspectable* value)
{
	float dpi;
	pDisplayInformation->get_LogicalDpi(&dpi);
	m_Scale = dpi / 96.0f;
	m_pFrameHandler->OnDpiChanged(m_Scale);
	return S_OK;
}

HRESULT CFrameView::OnOrientationChanged(IDisplayInformation* pDisplayInformation, IInspectable* value)
{
	m_Orientation = GetOrientation(pDisplayInformation);
	m_pFrameHandler->OnOrientationChanged(m_Orientation);
	return S_OK;
}

#pragma endregion

#pragma region Utilities

static Xe::Core::Orientation GetOrientation(IDisplayInformation* pDisplayInformation)
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
static void SetPreferredOrientation(IDisplayInformationStatics* pDisplayInformation, Xe::Core::Orientation orientation)
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

#pragma endregion