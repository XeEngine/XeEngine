#include "pch.h"
#include <XeSDK/XeGraphics.h>

namespace Xe {
	namespace Graphics {
		bool IsSupported(RenderEngine renderEngine) {
			switch (renderEngine) {
			case RenderEngine_D3D9:
				return FreeLibrary(LoadLibraryEx(_T("D3D9.DLL"), NULL, DONT_RESOLVE_DLL_REFERENCES)) != 0;
			case RenderEngine_D3D11:
				return FreeLibrary(LoadLibraryEx(_T("D3D11.DLL"), NULL, DONT_RESOLVE_DLL_REFERENCES)) != 0;
			case RenderEngine_D3D12:
				return FreeLibrary(LoadLibraryEx(_T("D3D12.DLL"), NULL, DONT_RESOLVE_DLL_REFERENCES)) != 0;
			case RenderEngine_OpenGL:
				return true;
			default:
				return false;
			}
		}
		RenderEngine GetDefaultRenderEngine() {
			return RenderEngine_OpenGL;
		}
	}
}