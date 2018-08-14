#include "pch.h"
#include "XeDriversRenderingD3D11Driver.h"
#include "ContextD3D11.h"

namespace Xe { namespace Drivers { namespace Rendering {
	const char* D3D11Driver::GetDriverName() const
	{
		return "Direct3D 11";
	}

	DriverType D3D11Driver::GetDriverType() const
	{
		return DriverType_Rendering;
	}

	bool D3D11Driver::Factory(Xe::Graphics::IContext** pContext, Xe::Core::IView *pView, const Xe::Graphics::ContextProperties& properties)
	{
		ASSERT(pContext != nullptr);
		auto driver = new Xe::Graphics::CContextD3D11(pView);

		ASSERT(driver != nullptr);
		bool result = driver->Initialize(properties);
		if (result)
		{
			*pContext = driver;
		}

		return result;
	}
} } }
