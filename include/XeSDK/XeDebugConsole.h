#pragma once
#include <XeSDK/IDebugConsole.h>
#include <XeSDK/IGraphicsContext.h>

namespace Xe {
	namespace Graphics {
		namespace Font {
			class IFont;
		}
	}
	namespace Debug {
		bool CreateNullcon(IConsole **console);
		bool CreateGraphicon(IConsole **console, Graphics::IContext *context, Graphics::Font::IFont *pFont);
	}
}