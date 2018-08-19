#pragma once
#include <XeSDK/XeDef.h>

namespace Xe {
	namespace Graphics {
		interface IContext;
		struct ContextInitDesc;
		bool CreateContextGLES(IContext **context, const ContextInitDesc& properties);
	}
}