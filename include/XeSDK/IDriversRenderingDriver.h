#pragma once
#include <XeSDK/IDriversDriver.h>

namespace Xe { namespace Core {
	class IView;
} }

namespace Xe { namespace Graphics {
	struct ContextProperties;
	class IContext;
} }

namespace Xe { namespace Drivers { namespace Rendering {
	//! \brief Contract for a Rendering Driver
	struct IRenderingDriver : public IDriver
	{
		virtual bool Factory(Xe::Graphics::IContext** pContext, Xe::Core::IView *pView, const Xe::Graphics::ContextProperties& properties) = 0;
	};
} } }
