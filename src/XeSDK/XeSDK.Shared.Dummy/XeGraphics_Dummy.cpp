#include "pch.h"
#include <XeSDK/XeGraphics.h>

namespace Xe {
    namespace Graphics {
        bool IsSupported(RenderEngine renderEngine) {
            switch (renderEngine) {
                case RenderEngine_Null:
                    return true;
                default:
                    return false;
            }
        }
        RenderEngine GetDefaultRenderEngine() {
            return RenderEngine_Null;
        }
    }
}