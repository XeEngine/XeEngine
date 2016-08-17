#include "pch.h"
#include <XeSDK/XeGraphics.h>

namespace Xe {
	namespace Graphics {
		bool IsSupported(RenderEngine renderEngine) {
			switch (renderEngine) {
			case RenderEngine_D3D11:
			case RenderEngine_D3D12:
				return true;
			default:
				return false;
			}
		}
		RenderEngine GetDefaultRenderEngine() {
			return RenderEngine_D3D11;
		}
	}
}