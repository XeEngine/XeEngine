#include "pch.h"
#include "XeDriversRenderingNullDriver.h"
#include "ContextNull.h"

namespace Xe { namespace Drivers {
	const char* NullRenderingDriver::GetDriverName() const
	{
		return "Null render";
	}

	DriverType NullRenderingDriver::GetDriverType() const
	{
		return DriverType_Rendering;
	}

	bool NullRenderingDriver::Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties)
	{
		ASSERT(pContext != nullptr);
		auto driver = new Xe::Graphics::CContextNull;

		ASSERT(driver != nullptr);
		bool result = driver->Initialize(properties);
		if (result)
		{
			*pContext = driver;
		}

		return result;
	}
} }
