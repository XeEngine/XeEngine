#pragma once
#include <XeSDK/IDriversRenderingDriver.h>

namespace Xe { namespace Drivers { namespace Rendering {
	struct OpenGlDriver : public IRenderingDriver
	{
		const char* GetDriverName() const;
		DriverType GetDriverType() const;
		bool Factory(Xe::Graphics::IContext** pContext, Xe::Core::IView *pView, const Xe::Graphics::ContextInitDesc& properties);
	};
} } }
