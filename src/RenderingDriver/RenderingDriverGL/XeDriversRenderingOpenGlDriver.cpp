#include "pch.h"
#include "XeDriversRenderingOpenGlDriver.h"
#include "ContextGL.h"

#ifdef PLATFORM_WIN32

#include "ContextGL.Win32.h"
#define TYPECONTEXT Xe::Graphics::ContextGL_Win32
#define CREATECONTEXT (new TYPECONTEXT(pView))

#else

#define CREATECONTEXT nullptr

#endif

namespace Xe { namespace Drivers { namespace Rendering {
	const char* OpenGlDriver::GetDriverName() const
	{
		return "OpenGL";
	}

	DriverType OpenGlDriver::GetDriverType() const
	{
		return DriverType_Rendering;
	}

	bool OpenGlDriver::Factory(Xe::Graphics::IContext** pContext, Xe::Core::IView *pView, const Xe::Graphics::ContextProperties& properties)
	{
		ASSERT(pContext != nullptr);
		auto driver = CREATECONTEXT;

		ASSERT(driver != nullptr);
		bool result = driver->Initialize(properties);
		if (result)
		{
			*pContext = driver;
		}

		return result;
	}
} } }
