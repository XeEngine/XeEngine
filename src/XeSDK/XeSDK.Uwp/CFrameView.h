#pragma once
#include <XeSDK/ICoreView.h>
#include <XeSDK/XeCoreFrameView.h>
#include <XeSDK/XeIOPointer.h>
#include <XeSDK/XeString.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::ApplicationModel::Core;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Windows::UI::ViewManagement;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel;
using namespace ABI::Windows::ApplicationModel::Activation;
using namespace ABI::Windows::Graphics::Display;

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

class CFrameView :
	public Xe::Core::IFrameView,
	ABI::Windows::ApplicationModel::Core::IFrameworkViewSource,
	ABI::Windows::ApplicationModel::Core::IFrameworkView
{
	Xe::Graphics::Size m_Size;
	Xe::Core::Orientation m_Orientation;
	float m_Scale;
	bool m_IsFullscreen;
	bool m_IsClosed;

	// XeEngine stuff
	Xe::Core::IApplicationHandler* m_pApplicationHandler;
	Xe::Core::IFrameHandler* m_pFrameHandler;
	Xe::Core::IKeyboardHandler* m_pKeyboardHandler;
	Xe::Core::IPointerHandler* m_pPointerHandler;

	// Windows Runtime stuff
	ULONG m_ref;
	ComPtr<ICoreWindow> m_Window;
	ComPtr<ICoreDispatcher> m_Dispatcher;
	ComPtr<IApplicationView> m_ApplicationView;
	ComPtr<IApplicationView3> m_ApplicationView3;
	ComPtr<IDisplayInformationStatics> m_DisplayInformationStatics;

	// Inherted from IFrameView
	void SetApplicationHandler(Xe::Core::IApplicationHandler* pApplicationHandler);
	void SetFrameHandler(Xe::Core::IFrameHandler* pFrameHandler);
	void SetKeyboardHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler);
	void SetPointerHandler(Xe::Core::IPointerHandler* pPointerHandler);
	bool DispatchEvents(Xe::Core::DispatchType type);
	void SetTitle(const Xe::String& title);
	Xe::Graphics::Size GetSize() const;
	bool SetSize(const Xe::Graphics::Size& size);
	void SetFullScreen(bool fullScreen);
	Xe::Core::Orientation GetOrientation() const;
	void SetPreferredOrientation(Xe::Core::Orientation orientation);
	float GetScale() const;
	void* GetSystemWindow() const;

	// Inherted from IUnknown
	HRESULT WINAPI QueryInterface(IID const & id, void ** result);
	ULONG WINAPI AddRef();
	ULONG WINAPI Release();

	// Inherted from IInspectable
	HRESULT WINAPI GetIids(ULONG *, IID **);
	HRESULT WINAPI GetRuntimeClassName(HSTRING *);
	HRESULT WINAPI GetTrustLevel(TrustLevel *);

	// Inherted from IFrameworkViewSource
	HRESULT WINAPI CreateView(IFrameworkView ** result);

	// Inherted from IFrameworkView
	HRESULT WINAPI Initialize(ICoreApplicationView * view);
	HRESULT WINAPI SetWindow(ICoreWindow * window);
	HRESULT WINAPI Load(HSTRING);
	HRESULT WINAPI Invoke(ICoreApplicationView *, IActivatedEventArgs *);
	HRESULT WINAPI Run();
	HRESULT WINAPI Uninitialize();

	// Events from CoreApplication
	HRESULT OnSuspending(IInspectable*, ISuspendingEventArgs* args);
	HRESULT OnResuming(IInspectable*, IInspectable*);

	// Events from IFrameworkView
	HRESULT OnActivated(ICoreApplicationView*, IActivatedEventArgs* args);

	// Events from ICoreWindow
	HRESULT OnWindowActivated(ICoreWindow* pCoreWindow, IWindowActivatedEventArgs* args);
	HRESULT OnClosed(ICoreWindow* pCoreWindow, ICoreWindowEventArgs* args);
	HRESULT OnSizeChanged(ICoreWindow* pCoreWindow, IWindowSizeChangedEventArgs* args);
	HRESULT OnVisibilityChanged(ICoreWindow* pCoreWindow, IVisibilityChangedEventArgs* args);
	HRESULT OnCharacterReceived(ICoreWindow* pCoreWindow, ICharacterReceivedEventArgs* args);
	HRESULT OnKeyDown(ICoreWindow* pCoreWindow, IKeyEventArgs* args);
	HRESULT OnKeyUp(ICoreWindow* pCoreWindow, IKeyEventArgs* args);
	HRESULT OnPointerCaptureLost(ICoreWindow* pCoreWindow, IPointerEventArgs* args);
	HRESULT OnPointerEntered(ICoreWindow* pCoreWindow, IPointerEventArgs* args);
	HRESULT OnPointerExited(ICoreWindow* pCoreWindow, IPointerEventArgs* args);
	HRESULT OnPointerMoved(ICoreWindow* pCoreWindow, IPointerEventArgs* args);
	HRESULT OnPointerPressed(ICoreWindow* pCoreWindow, IPointerEventArgs* args);
	HRESULT OnPointerReleased(ICoreWindow* pCoreWindow, IPointerEventArgs* args);
	HRESULT OnPointerWheelChanged(ICoreWindow* pCoreWindow, IPointerEventArgs* args);

	// Events from DisplayInformation
	HRESULT OnDisplayContentsInvalidated(IDisplayInformation* pDisplayInformation, IInspectable* value);
	HRESULT OnDpiChanged(IDisplayInformation* pDisplayInformation, IInspectable* value);
	HRESULT OnOrientationChanged(IDisplayInformation* pDisplayInformation, IInspectable* value);

	// Utilities
	static Xe::Core::Orientation GetOrientation(IDisplayInformation* pDisplayInformation);
	static void SetPreferredOrientation(IDisplayInformationStatics* pDisplayInformation, Xe::Core::Orientation orientation);

public:
	CFrameView();
	~CFrameView();

	bool Initialize(const Xe::Core::FrameViewInitDesc& desc);
};