#include "pch.h"
#include "XeDriversRenderingOpenGlDriver.h"
#include "ContextGL.h"

#ifdef PLATFORM_WIN32

#include "ContextGL.Win32.h"
#define TYPECONTEXT Xe::Graphics::ContextGL_Win32
#define CREATECONTEXT (new TYPECONTEXT)

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

	bool OpenGlDriver::Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties)
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
