#include "pch.h"
#include <XeSDK/IDriversRenderingDriver.h>

namespace Xe { namespace Drivers {
	struct NullRenderingDriver : public IRenderingDriver
	{
		const char* GetDriverName() const;
		DriverType GetDriverType() const;
		bool Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties);
	};
} }
