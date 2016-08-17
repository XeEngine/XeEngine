#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsImagingImage.h>

namespace Xe {
	namespace Graphics {
		//! \brief Image format management
		namespace Imaging {
			bool CreateSurface(IContext *context, ISurface **surface, SurfaceType type, IImage *image);
		}
	}
}