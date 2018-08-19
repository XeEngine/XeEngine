#pragma once
#include <XeSDK/IDriversDriver.h>

namespace Xe { namespace Graphics {
	struct ContextInitDesc;
	class IContext;
} }

namespace Xe { namespace Drivers { namespace Rendering {
	//! \brief Contract for a Rendering Driver
	struct IRenderingDriver : public IDriver
	{
		virtual bool Factory(Xe::Graphics::IContext** pContext, const Xe::Graphics::ContextInitDesc& properties) = 0;
	};
} } }
