#pragma once
#include <XeSDK/IDriversRenderingDriver.h>

namespace Xe { namespace Drivers {
	struct OpenGlDriver : public IRenderingDriver
	{
		const char* GetDriverName() const;
		DriverType GetDriverType() const;
		bool Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties);
	};
} }
