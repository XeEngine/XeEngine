#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsImagingImage.h>
#include <XeSDK/XeIO.h>
#include "IStorage.h"

namespace Xe { namespace Graphics { namespace Imaging {
	//! \brief Try to open the most appropiate image from the file extension
	bool OpenImage(IImage** ppImage, Xe::IO::IFileStream* pStream);

	//! \brief Try to open the most appropiate image from the file extension
	bool OpenImage(IImage** ppImage, Xe::Storage::IStorage* pStorage, const char* fileName);
	
	bool CreateSurface(IContext *context, ISurface **surface, SurfaceType type, IImage *image);
} } }
