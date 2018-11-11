#include "pch.h"
#include "$ext_safeprojectname$View.h"

using namespace Xe;

namespace Xe { namespace Core {
	// Entry point of our application.
	void Main() {
		// Initialize XeEngine, starting up the drivers and its dependencies
		Initialize();

		ObjPtr<$ext_safeprojectname$::$ext_safeprojectname$View> frameView = new $ext_safeprojectname$::$ext_safeprojectname$View;

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

		// Ask to the core to initialize and show the specified FrameView
		if (Xe::Core::Run(frameView, viewInitDesc) == false)
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
		m_IsFullscreen(false)
	{
	}

	$ext_safeprojectname$View::~$ext_safeprojectname$View()
	{
	}

	bool $ext_safeprojectname$View::OnAttach(Xe::Core::IFrameView* pFrameView)
	{
		// Early call to specify the ApplicationHandler that will initialize the application
		m_pFrameView = pFrameView;
		m_pFrameView->SetApplicationHandler(this);

		return true;
	}

	bool $ext_safeprojectname$View::OnInitialize()
	{
		// Here belong the initialization of our view
		Xe::Graphics::ContextInitDesc contextInitDesc;
		contextInitDesc.FrameView = m_pFrameView;

		// Get the default rendering driver
		auto renderingDrivers = Xe::Drivers::GetDrivers<Xe::Drivers::IRenderingDriver>();
		auto defaultRenderingDriver = *renderingDrivers.begin();
		
		// Initialize the rendering driver
		if (!defaultRenderingDriver->Factory(&m_pContext, contextInitDesc))
		{
			LOGE("Unable to initialize the default rendering driver %s.", defaultRenderingDriver->GetDriverName());
			return false;
		}

		// Set a default color as background of our app.
		m_pContext->SetClearColor(Graphics::Color::SlateGray);

		// Set the other handlers (eg. keyboard, mouse, network, overlays, etc.)
		m_pFrameView->SetKeyboardHandler(this);

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
		// Slow down the app under 10fps
		static const double DELTATIME_TRESHOLD = 1.0 / 10.0;

		// This is the body of our application.
		Timer prev = Timer::Current();
		bool isRunning;

		do
		{
			try
			{
				// Dispatch only one event at the time from the FrameView event dispacher
				while (isRunning = m_pFrameView->DispatchEvents(Core::DispatchType_One))
				{
					// Calculate the raw delta time
					Timer cur = Timer::Current();
					double deltaTime = Timer(cur - prev).AsDouble();
					prev = cur;
					if (deltaTime > DELTATIME_TRESHOLD)
						deltaTime = DELTATIME_TRESHOLD;

					// Draw the scene
					Draw();
					SwapBuffer(m_IsVblankEnabled);
				}
			}
			catch (const std::exception& e)
			{
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

	void $ext_safeprojectname$View::Draw()
	{
		m_pContext->Clear(Graphics::Clear_Color | Graphics::Clear_Depth | Graphics::Clear_Stencil);
	}

	void $ext_safeprojectname$View::SwapBuffer(bool isVblankEnabled)
	{
		m_pContext->SwapBuffers(isVblankEnabled ?
			m_pContext->VBlank_Wait :
			m_pContext->VBlank_DoNotWait);
	}
}