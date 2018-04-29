#include "pch.h"
#include <XeSDK/ICoreView.h>
#include "XeCoreView.h"

#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeString.h>

using namespace Xe::Graphics;

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
                    m_isClosed(true) {
                m_pointerEvent.CurrentPointer.IsProcessed = true;
                m_pointerEvent.CurrentPointer.Device.Type = IO::PointerType_Mouse;
                m_pointerEvent.CurrentPointer.Device.Is3dSupported = false;
                m_pointerEvent.CurrentPointer.Position.Set(-1, -1);
                m_pointerEvent.CurrentPointer.Buttons = 0;
                m_pointerEvent.CurrentPointer.Pressure = 0.0f;
                m_pointerEvent.CurrentPointer.Time = Timer::Current();
            }
            ~CFrameView() {
            }

            void SetViewInitializeState(bool isInitialized) {
                m_IsViewInitialized = isInitialized;
            }
            bool Initialize(const IView::InitProperties& properties) {
                m_isFullScreenSwitchEnabled = false;

                const Size &size = properties.Size;
                int width = (int)(size.x >= 0 ? size.x : 800);
                int height = (int)(size.y >= 0 ? size.y : 480);

                SetSize(Size(width, height));
                m_isClosed = false;
                m_isWindowMinimized = false;
                return true;
            }

            bool DispatchEvents(DispatchType type) {

            }

            void SetTitle(const String& title) {
            }

            float GetScale() const {
                return (float)1.0f;
            }

            Graphics::Size GetSize() const {
                return Size();
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
                return nullptr;
            }

        private:
            bool m_isClosed;
            bool m_isFullScreenSwitchEnabled;
            bool m_isWindowMinimized;
            Size m_Size;
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