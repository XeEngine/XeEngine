#pragma once
#include <XeSDK/IDriversDriver.h>

namespace Xe { namespace Graphics {
	struct ContextInitDesc;
	interface IContext;
} }

namespace Xe { namespace Drivers { namespace Rendering {
	//! \brief Contract for a Rendering Driver
	interface IRenderingDriver : public IDriver
	{
		virtual bool Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties) = 0;
	};
} } }
