#include <XeSDK/XeGraphics.h>
#include "pch.h"

namespace Xe {
	namespace Graphics {
		bool IsSupported(RenderEngine renderEngine) {
			switch (renderEngine) {
			case RenderEngine_GLES20:
				return true;
			default:
				return false;
			}
		}
		RenderEngine GetDefaultRenderEngine() {
			return RenderEngine_GLES20;
		}
	}
}