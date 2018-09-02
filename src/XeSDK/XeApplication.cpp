#include "pch.h"
#include <XeSDK/XeApplication.h>
#include <XeSDK/XeTimer.h>
#include <XeSDK/IGraphicsContext.h>

using namespace Xe::Graphics;

namespace Xe {
    bool AppLoop::Create(AppLoop **ppAppLoop, Application* pApp, const AppProperties& properties) {
        class CAppLife : public AppLoop {
            Application* m_App;
            AppProperties m_Properties;
            Timer m_TLast;
            bool m_Initialized;

        public:
            CAppLife(Application* app, const AppProperties& properties) :
                    m_App(app),
                    m_Properties(properties),
                    m_Initialized(false) {
                m_App->AddRef();
            }
            ~CAppLife() {
                if (m_Initialized)
                    m_App->OnDestroy();
                if (m_App->m_pContext)
                    m_App->m_pContext->Release();
                m_App->Release();
            }
            bool Initialize() {
                Graphics::ContextInitDesc p;
                m_App->OnContextInitialize(p);
                if (Xe::Graphics::Create(&m_App->m_pContext, Xe::Graphics::RenderEngine_Default, p) != Error::OK)
                    return false;
                m_Initialized = true;
                if (!m_App->OnInitialize())
                    return false;
                m_TLast = Timer::Current();
                return true;
            }
            bool Process(const AppProperties &properties) {
                if (!m_App->IsValidated()) {

                    if (!m_App->IsValidated()) {
                        m_App->Flush();
                        m_App->OnDraw();
                    }
                    Timer tCur = Timer::Current();
                    double deltaTime = Timer(tCur - m_TLast).AsDouble();
                    m_TLast = tCur;
                    if (deltaTime > properties.MaxDeltaTime)
                        deltaTime = properties.MaxDeltaTime;

                    bool r;
                    switch (properties.TimestepType) {
                        case AppProperties::TimestepType_Dynamic:
                            r = m_App->OnUpdate(deltaTime);
                            break;
                        case AppProperties::TimestepType_SemiFixed:
                            while (deltaTime > 0) {
                                double dt = deltaTime > properties.Timestep ? properties.Timestep : deltaTime;
                                deltaTime -= properties.Timestep;
                                r = m_App->OnUpdate(dt);
                            }
                            break;
                        case AppProperties::TimestepType_Fixed:
                            while (deltaTime >= properties.Timestep) {
                                deltaTime -= properties.Timestep;
                                r = m_App->OnUpdate(properties.Timestep);
                            }
                            break;
                    }
                    return r;
                }
                else return true;
            }
        };

        CAppLife *pAppLoop = new CAppLife(pApp, properties);
        if (!pAppLoop->Initialize()) {
            pAppLoop->Release();
            return false;
        }
        *ppAppLoop = pAppLoop;
        return true;
    }

    Application::Application() :
            m_Invalidated(false) {
    }
    bool Application::IsValidated() const {
        return m_Invalidated;
    }
    void Application::Invalidate() {
        m_Invalidated = true;
    }
    void Application::Flush() {
        m_Invalidated = false;
    }

    void Application::OnKeybDown(const Key &key) {}
    void Application::OnKeybUp(const Key &key) {}
    void Application::OnKeybChar(const Key &key, uint ch) {}

    void Application::OnPointerMove(const IO::PointerEvent& e) {}
    void Application::OnPointerPressed(const IO::PointerEvent& e) {}
    void Application::OnPointerReleased(const IO::PointerEvent& e) {}
    void Application::OnPointerEnter() {}
    void Application::OnPointerLeave() {}
    void Application::OnPointerWheel(const Math::Vector2<float>& wheel) {}

    void Application::OnSizeChanged(const Math::Vector2<svar> &size) {}
    void Application::OnLostFocus() {}
    void Application::OnGotFocus() {}
    void Application::OnSuspend() {}
    void Application::OnResume() {}
    bool Application::OnClosing(bool forced) {
        return true;
    }
}

bool Xe::Run(Application *app, const AppProperties &properties) {
    AppLoop *loop;
    if (AppLoop::Create(&loop, app, properties)) {
        while (loop->Process(properties));
        loop->Release();
        return true;
    }
    return false;
}