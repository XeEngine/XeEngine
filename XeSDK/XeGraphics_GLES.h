#pragma once

namespace Xe {
	namespace Graphics {
		class IContext;
		struct ContextProperties;
		bool CreateContextGLES(IContext **context, const ContextProperties& properties);
	}
}