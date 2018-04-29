#include "pch.h"
#include <XeSDK/ICoreView.h>
#include "XeCoreView.h"

#include <XeSDK/XeLogger.h>
#include "XeCoreView_X11.h"
#include <GL/glx.h>
#include <XeSDK/XeCore.h>
#include <XeSDK/XeLogger.h>
#include <X11/Xlib.h>

using namespace Xe::Graphics;

namespace Xe {
    namespace Core {
        class CFrameView : public IFrameView {
            IView* m_pView;
            IO::PointerEvent m_pointerEvent;
            IO::KeyboardEvent m_keyboardEvent;
            bool m_IsViewInitialized;

            FrameView_X11_SystemWindow m_SystemWindow = { 0 };
            int m_Screen;
            GC m_Gc;
            Colormap m_Colormap;
            XSetWindowAttributes m_WindowAttributes;
            int grabMouse;
            Atom m_WmDeleteMessage;

            bool m_isClosed;
            bool m_isFullScreenSwitchEnabled;
            bool m_isWindowMinimized;
            Size m_Size;

        public:
            CFrameView(IView *pView) :
                    m_pView(pView),
                    m_IsViewInitialized(false),
                    m_Gc(nullptr),
                    m_isClosed(true),
                    m_isFullScreenSwitchEnabled(false),
                    m_isWindowMinimized(false) {
                m_pointerEvent.CurrentPointer.IsProcessed = true;
                m_pointerEvent.CurrentPointer.Device.Type = IO::PointerType_Mouse;
                m_pointerEvent.CurrentPointer.Device.Is3dSupported = false;
                m_pointerEvent.CurrentPointer.Position.Set(-1, -1);
                m_pointerEvent.CurrentPointer.Buttons = 0;
                m_pointerEvent.CurrentPointer.Pressure = 0.0f;
                m_pointerEvent.CurrentPointer.Time = Timer::Current();
            }
            ~CFrameView() {
                if (m_SystemWindow.window != 0)
                {
                    XDestroyWindow(m_SystemWindow.display, m_SystemWindow.window);
                    m_SystemWindow.window = 0;
                }
                if (m_Gc != nullptr)
                {
                    XFreeGC(m_SystemWindow.display, m_Gc);
                    m_Gc = nullptr;
                }
                if (m_SystemWindow.display != nullptr) {
                    XCloseDisplay(m_SystemWindow.display);
                    m_SystemWindow.display = nullptr;
                }
                m_isClosed = true;
                m_isFullScreenSwitchEnabled = false;
                m_isWindowMinimized = false;
            }

            void SetViewInitializeState(bool isInitialized) {
                m_IsViewInitialized = isInitialized;
            }
            bool Initialize(const IView::InitProperties& properties) {
                bool result = false;

                m_SystemWindow.display = XOpenDisplay(nullptr);
                if (m_SystemWindow.display != nullptr) {
                    int att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, None };

                    m_Screen = XDefaultScreen(m_SystemWindow.display);
                    ::Window root = XRootWindow(m_SystemWindow.display, m_Screen);
                    m_SystemWindow.visualInfo = glXChooseVisual(m_SystemWindow.display, 0, att);
                    if (m_SystemWindow.visualInfo != nullptr) {
                        m_Colormap = XCreateColormap(m_SystemWindow.display, root, m_SystemWindow.visualInfo->visual, AllocNone);

                        m_WindowAttributes.colormap = m_Colormap;
                        m_WindowAttributes.event_mask = ExposureMask | KeyPressMask;

                        m_Size = properties.Size;
                        m_Size.x = m_Size.x >= 0 ? m_Size.x : 800;
                        m_Size.y = m_Size.y >= 0 ? m_Size.y : 480;

                        m_SystemWindow.window = XCreateWindow(m_SystemWindow.display, root, 0, 0, m_Size.x, m_Size.y, 0, m_SystemWindow.visualInfo->depth, InputOutput,
                                                              m_SystemWindow.visualInfo->visual, CWColormap | CWEventMask, &m_WindowAttributes);
                        if (m_SystemWindow.window != 0) {
                            XSelectInput(m_SystemWindow.display, m_SystemWindow.window,
                                         KeyPressMask |
                                         KeyReleaseMask |
                                         ButtonMotionMask |
                                         ButtonPressMask |
                                         ButtonReleaseMask |
                                         EnterWindowMask |
                                         LeaveWindowMask |
                                         PointerMotionMask |
                                         VisibilityChangeMask |
                                         FocusChangeMask |
                                         ExposureMask
                            );
                            SetTitle(properties.Title);
                            SetVisible(true);

                            m_WmDeleteMessage = XInternAtom(m_SystemWindow.display, "WM_DELETE_WINDOW", False);
                            XSetWMProtocols(m_SystemWindow.display, m_SystemWindow.window, &m_WmDeleteMessage, 1);

                            m_isClosed = false;
                            m_isWindowMinimized = false;
                            m_isFullScreenSwitchEnabled = false;
                            result = true;
                        }
                        else {
                            Logger::Error("XCreateWindow failed.");
                        }
                    }
                    else {
                        Logger::Error("glXChooseVisual failed.");
                    }
                }
                else {
                    Logger::Error("XOpenDisplay failed.");
                }
                return result;
            }

            bool DispatchEvents(DispatchType type) {
                int queued;
                switch (type) {
                    case DispatchType_Exit:
                        return false;
                    case DispatchType_None:
                        return true;
                    case DispatchType_One:
                        if (XPending(m_SystemWindow.display) > 0) {
                            DispatchMessage();
                        }
                        break;
                    case DispatchType_All:
                        while (XPending(m_SystemWindow.display) > 0)
                        {
                            DispatchMessage();
                        }
                        break;
                    case DispatchType_WaitOnce:
                        while (XPending(m_SystemWindow.display) > 0)
                        {
                            DispatchMessage();
                        }
                        DispatchMessage();
                        break;
                    case DispatchType_WaitUntilQuit:
                        while (!m_isClosed)
                        {
                            while (XPending(m_SystemWindow.display) > 0)
                            {
                                DispatchMessage();
                            }
                        }
                }
                return !m_isClosed;
            }

            void SetTitle(const String& title) {
                XStoreName(m_SystemWindow.display, m_SystemWindow.window, title);
            }

            void SetVisible(bool visible) {
                if (visible == true) {
                    XMapWindow(m_SystemWindow.display, m_SystemWindow.window);
                }
                else {
                    XUnmapWindow(m_SystemWindow.display, m_SystemWindow.window);
                }
            }

            float GetScale() const {
                return (float)1.0f;
            }

            Graphics::Size GetSize() const {
                return m_Size;
            }

            bool SetSize(const Graphics::Size& size) {
                if (size.x <= 0 || size.y <= 0)
                    return false;
                return false;
            }

            void SetFullScreen(bool fullScreen) {
                if (fullScreen)
                {
                }
                else
                {
                }
            }

            Graphics::Orientation GetOrientation() const {
                return Graphics::Orientation_Unknown;
            }

            void SetPreferredOrientation(Graphics::Orientation orientation) {

            }

            void* GetSystemWindow() const {
                return (void*)&m_SystemWindow;
            }

            void DispatchMessage() {
                XEvent event;
                XNextEvent(m_SystemWindow.display, &event);
                switch(event.type)
                {
                    case KeyPress:
                        OnKeyDown(event.xkey);
                        break;
                    case KeyRelease:
                        OnKeyUp(event.xkey);
                        break;
                    case ButtonPress:
                        switch (event.xbutton.button) {
                            case Button1:
                                OnMouseButton(event.xbutton.x, event.xbutton.y, 0, true);
                                break;
                            case Button2:
                                OnMouseButton(event.xbutton.x, event.xbutton.y, 1, true);
                                break;
                            case Button3:
                                OnMouseButton(event.xbutton.x, event.xbutton.y, 2, true);
                                break;
                            case Button4:
                                OnMouseWheel(Math::Vector2f(0, +1));
                                break;
                            case Button5:
                                OnMouseWheel(Math::Vector2f(0, -1));
                                break;
                            case 6:
                                OnMouseWheel(Math::Vector2f(-1, 0));
                                break;
                            case 7:
                                OnMouseWheel(Math::Vector2f(+1, 0));
                                break;
                        }
                        break;
                    case ButtonRelease:
                        switch (event.xbutton.button) {
                            case Button1:
                                OnMouseButton(event.xbutton.x, event.xbutton.y, 0, true);
                                break;
                            case Button2:
                                OnMouseButton(event.xbutton.x, event.xbutton.y, 1, true);
                                break;
                            case Button3:
                                OnMouseButton(event.xbutton.x, event.xbutton.y, 2, true);
                                break;
                        }
                        break;
                    case MotionNotify:
                        OnMouseMove(event.xmotion.x, event.xmotion.y);
                        break;
                    case EnterNotify:
                        m_pView->OnPointerEnter(m_pointerEvent);
                        break;
                    case LeaveNotify:
                        m_pView->OnPointerLeave(m_pointerEvent);
                        break;
                    case FocusIn:
                        m_pView->OnFocusGot();
                        break;
                    case FocusOut:
                        m_pView->OnFocusLost();
                        break;
                    case Expose:
                        XWindowAttributes gwa;
                        XGetWindowAttributes(m_SystemWindow.display, m_SystemWindow.window, &gwa);
                        if (m_Size.x != gwa.width || m_Size.y != gwa.height) {
                            m_Size = Size(gwa.width, gwa.height);
                            m_pView->OnSizeChanged(m_Size);
                        }
                        m_pView->OnDraw();
                        break;
                    case KeymapNotify:
                        XRefreshKeyboardMapping(&event.xmapping);
                        break;
                    case ClientMessage:
                        if ( (Atom)event.xclient.data.l[0] == m_WmDeleteMessage )
                        {
                            if (m_pView->OnClosing(false)) {
                                m_isClosed = true;
                                m_pView->OnDestroy();
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            void OnKeyDown(XKeyEvent& e) {
                char buffer = 0;
                KeySym keySym = 0;
                XLookupString(&e, &buffer, 1, &keySym, nullptr);

                m_keyboardEvent.ScanCode = e.keycode;
                m_keyboardEvent.VirtualCode = keySym;
                m_pView->OnKeyPressed(m_keyboardEvent);
            }
            void OnKeyUp(XKeyEvent& e) {
                char buffer = 0;
                KeySym keySym = 0;
                XLookupString(&e, &buffer, 1, &keySym, nullptr);

                m_keyboardEvent.ScanCode = e.keycode;
                m_keyboardEvent.VirtualCode = keySym;
                m_pView->OnKeyReleased(m_keyboardEvent);
            }
            void OnMouseMove(int x, int y) {
                m_pointerEvent.CurrentPointer.Time = Timer();
                m_pointerEvent.CurrentPointer.Position.x = x;
                m_pointerEvent.CurrentPointer.Position.y = y;
                m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Moved;
                m_pView->OnPointerMoved(m_pointerEvent);
            }
            void OnMouseButton(int x, int y, int buttonFlag, bool pressed) {
                m_pointerEvent.CurrentPointer.Time = Timer();
                m_pointerEvent.CurrentPointer.Position.x = x;
                m_pointerEvent.CurrentPointer.Position.y = y;
                m_pointerEvent.CurrentPointer.Pressure = 1.0f;
                if (pressed) {
                    m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Pressed;
                    m_pointerEvent.CurrentPointer.Buttons |= 1 << buttonFlag;
                    m_pView->OnPointerPressed(m_pointerEvent);
                }
                else {
                    m_pointerEvent.CurrentPointer.Action = IO::PointerAction_Released;
                    m_pointerEvent.CurrentPointer.Buttons &= ~(1 << buttonFlag);
                    m_pView->OnPointerReleased(m_pointerEvent);
                }
            }
            void OnMouseWheel(Math::Vector2f wheel) {
                m_pView->OnPointerWheel(wheel);
            }
        };

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
        }
    }
}