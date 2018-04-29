#pragma once

namespace Xe {
	namespace Graphics {
		struct ContextProperties;
		class IContext;
		bool CreateContextD3D11(IContext **context, const ContextProperties& properties);
	}
}