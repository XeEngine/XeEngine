#include "pch.h"
#include "XeDriversRenderingD3D11Driver.h"
#include "ContextD3D11.h"

namespace Xe { namespace Drivers {
	const char* D3D11Driver::GetDriverName() const
	{
		return "Direct3D 11";
	}

	DriverType D3D11Driver::GetDriverType() const
	{
		return DriverType_Rendering;
	}

	bool D3D11Driver::Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties)
	{
		ASSERT(pContext != nullptr);
		auto driver = new Xe::Graphics::CContextD3D11;

		ASSERT(driver != nullptr);
		bool result = driver->Initialize(properties);
		if (result)
		{
			*pContext = driver;
			return true;
		}
		
		delete driver;
		*pContext = nullptr;
		return false;
	}
} }
