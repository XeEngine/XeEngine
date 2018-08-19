#include "pch.h"
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeDrivers.h>
#include <XeSDK/IDriversRenderingDriver.h>

using namespace Xe::Debug;

#define DRIVER_NULL "Null render"
#define DRIVER_D3D9 "Direct3D 9"
#define DRIVER_D3D10 "Direct3D 10"
#define DRIVER_D3D11 "Direct3D 11"
#define DRIVER_OPENGL "OpenGL"

namespace Xe { namespace Graphics {

	bool Create(IContext **context, RenderEngine renderEngine, const ContextInitDesc& properties)
	{
		ctstring DriverNames[]
		{
			DRIVER_D3D11,
			DRIVER_NULL,
			DRIVER_D3D9,
			DRIVER_D3D10,
			DRIVER_D3D11,
			DRIVER_OPENGL
		};

		ASSERT((unsigned int)renderEngine > lengthof(DriverNames));

		return Create(context, DriverNames[renderEngine], properties);
	}

    bool Create(IContext **context, ctstring driverName, const ContextInitDesc& properties)
	{
		LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Checking for '%s' rendering driver..."), driverName);

		Xe::Drivers::Rendering::IRenderingDriver* renderingDriver = nullptr;
		auto drivers = Xe::Drivers::GetDrivers(Xe::Drivers::DriverTypeFilter_Rendering);
		for (auto it = drivers.begin(); it != drivers.end(); it++)
		{
			auto currentDriverName = (*it)->GetDriverName();
			if (strcmp(currentDriverName, driverName) == 0)
			{
				renderingDriver = (Xe::Drivers::Rendering::IRenderingDriver*)*it;
			}
		}

		if (renderingDriver != nullptr)
		{
			LOG(Log::Priority_Info, Log::Type_Graphics, _T("Render driver %s found!"), driverName);
			if (renderingDriver->Factory(context, properties))
			{
				LOG(Log::Priority_Info, Log::Type_Graphics, _T("Render driver %s initialized with success!"), driverName);
				return true;
			}
			else
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to initialize the rendering driver %s."), driverName);
			}
		}
		else
		{
			LOG(Log::Priority_Error, Log::Type_Graphics, _T("Render driver %s not found."), driverName);
		}

		*context = nullptr;
		return false;
    }
} }