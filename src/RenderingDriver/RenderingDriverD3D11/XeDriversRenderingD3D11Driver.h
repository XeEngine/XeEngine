#include "pch.h"
#include <XeSDK/IDriversRenderingDriver.h>

namespace Xe { namespace Drivers { namespace Rendering {
	struct D3D11Driver : public IRenderingDriver
	{
		const char* GetDriverName() const;
		DriverType GetDriverType() const;
		bool Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties);
	};
} } }
