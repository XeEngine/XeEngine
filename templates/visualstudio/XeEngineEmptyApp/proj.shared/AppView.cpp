#include "pch.h"
#include "$ext_safeprojectname$View.h"

using namespace Xe;

namespace Xe { namespace Core {
	// Entry point of our application.
	void Main() {
		Initialize();

		ObjPtr<$ext_safeprojectname$::$ext_safeprojectname$View> pCoreView = new $ext_safeprojectname$::$ext_safeprojectname$View;

		Xe::Core::FrameViewInitDesc viewInitDesc;
#ifdef DEVELOPMENT
		viewInitDesc.Title = "$ext_projectname$ (dev)";
		viewInitDesc.Size.Set(1280, 720);
		viewInitDesc.IsResizable = true;
		viewInitDesc.IsFullscreen = false;
#else
		viewInitDesc.Title = "$ext_projectname$";
		viewInitDesc.Size.Set(1280, 720);
		viewInitDesc.IsResizable = true;
		viewInitDesc.IsFullscreen = true;
#endif

		if (Xe::Core::Run(pCoreView.Get(), viewInitDesc) == false)
		{
			LOGE("Xe::Core::Factory failed.\n");
		}
	}
} }

namespace $ext_safeprojectname$
{
	$ext_safeprojectname$View::$ext_safeprojectname$View() :
		// Assign a default value to all members as pre-initialization.
		m_pFrameView(nullptr),
		m_pContext(nullptr),
		m_IsVblankEnabled(true),
		m_IsFullscreen(false),
		m_IsOverlayActivated(false)
	{
		// Use destructor to deallocate everything not destroyed by OnDestroy.
		if (m_pFrameView) m_pFrameView->Release();
	}

	$ext_safeprojectname$View::~$ext_safeprojectname$View()
	{
	}

	bool $ext_safeprojectname$View::OnAttach(Xe::Core::IFrameView* pFrameView)
	{
		m_pFrameView = pFrameView;
		m_pFrameView->AddRef();
		m_pFrameView->SetApplicationHandler(this);

		return true;
	}
	bool $ext_safeprojectname$View::OnInitialize()
	{
		// Here belong the initialization of our view
		Xe::Graphics::ContextInitDesc contextInitDesc;
		contextInitDesc.FrameView = m_pFrameView;

		auto renderingDrivers = Xe::Drivers::GetDrivers(Xe::Drivers::DriverTypeFilter_Rendering);
		auto defaultRenderingDriver = (Xe::Drivers::Rendering::IRenderingDriver*)*renderingDrivers.begin();
		
		if (!defaultRenderingDriver->Factory(&m_pContext, contextInitDesc))
		{
			LOGE("Unable to initialize the default rendering driver %s.", defaultRenderingDriver->GetDriverName());
			return false;
		}

		// Set a default color as background of our app.
		m_pContext->SetClearColor(Graphics::Color::SlateGray);

		// Set the handlers
		m_pFrameView->SetApplicationHandler(this);
		m_pFrameView->SetKeyboardHandler(this);
		m_pFrameView->SetPointerHandler(this);

		return true;
	}
	void $ext_safeprojectname$View::OnDestroy()
	{
		// This is called if OnInitialize was previously called, even if it
		// failed. Destroy here all allocated stuff.
		if (m_pContext) m_pContext->Release();
	}
	void $ext_safeprojectname$View::OnRun()
	{
		static const double DELTATIME_TRESHOLD = 1.0 / 10.0;

		// This is the body of our application.
		Timer prev = Timer::Current();
		bool isRunning;

		do {
			try {
				while (isRunning = m_pFrameView->DispatchEvents(Core::DispatchType_One)) {
					Timer cur = Timer::Current();
					double deltaTime = Timer(cur - prev).AsDouble();
					prev = cur;
					if (deltaTime > DELTATIME_TRESHOLD)
						deltaTime = DELTATIME_TRESHOLD;

					if (!m_IsOverlayActivated)
					{
						// Process the logic
					}

					Draw();
					SwapBuffer(m_IsVblankEnabled);
				}
			}
			catch (const std::exception& e) {
				LOGF("OnRun exception: %s", e.what());
			}
		} while (isRunning);
	}

	void $ext_safeprojectname$View::OnDraw()
	{
		// Called when window needs to be redrawn
		Draw();
		SwapBuffer(m_IsVblankEnabled);
	}

	void $ext_safeprojectname$View::OnSuspend()
	{
	}

	void $ext_safeprojectname$View::OnResume()
	{
		// Check for the pad
	}

	void $ext_safeprojectname$View::OnDevice(const Xe::Core::DeviceArgs& args)
	{
		// Check for the pad
	}

	bool $ext_safeprojectname$View::OnClosing(bool forced)
	{
		// Can ask for a confirmation message before to acutally close
		return true;
	}

	void $ext_safeprojectname$View::OnFocusGot()
	{
	}

	void $ext_safeprojectname$View::OnFocusLost()
	{
	}

	void $ext_safeprojectname$View::OnVisibilityChanged(bool visibility)
	{
	}

	void $ext_safeprojectname$View::OnSizeChanged(const Graphics::Size& size)
	{
	}

	void $ext_safeprojectname$View::OnOrientationChanged(Xe::Core::Orientation orientation)
	{
	}

	void $ext_safeprojectname$View::OnDpiChanged(float dpi)
	{
	}

	void $ext_safeprojectname$View::OnCharacter(const IO::CharacterEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnKeyPressed(const IO::KeyboardEvent& e)
	{
		switch (e.VirtualCode) {
		case 0x73: // F4
			m_IsFullscreen = !m_IsFullscreen;
			m_pFrameView->SetFullScreen(m_IsFullscreen);
			break;
		default:
			break;
		}

	}
	void $ext_safeprojectname$View::OnKeyReleased(const IO::KeyboardEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnPointerMoved(const IO::PointerEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnPointerPressed(const IO::PointerEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnPointerReleased(const IO::PointerEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnPointerEnter(const IO::PointerEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnPointerLeave(const IO::PointerEvent& e)
	{
	}

	void $ext_safeprojectname$View::OnPointerWheel(const Math::Vector2<float>& wheel)
	{
	}

	void $ext_safeprojectname$View::OnGameOverlayActivated(bool activated)
	{
		m_IsOverlayActivated = activated;
	}

	void $ext_safeprojectname$View::Draw() {
		m_pContext->Clear(Graphics::Clear_Color | Graphics::Clear_Depth | Graphics::Clear_Stencil);
	}

	void $ext_safeprojectname$View::SwapBuffer(bool isVblankEnabled)
	{
		m_pContext->SwapBuffers(isVblankEnabled ?
			m_pContext->VBlank_Wait :
			m_pContext->VBlank_DoNotWait);
	}
}