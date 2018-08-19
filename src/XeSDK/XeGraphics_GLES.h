#pragma once

namespace Xe {
	namespace Graphics {
		class IContext;
		struct ContextInitDesc;
		bool CreateContextGLES(IContext **context, const ContextInitDesc& properties);
	}
}