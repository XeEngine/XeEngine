#pragma once
#include <XeSDK/IIoStream.h>
#include <XeSDK/IGraphicsImagingImage.h>

namespace Xe {
	namespace Graphics {
		namespace Imaging {
			//! \brief open a PNG image
			//! \param[out] image opened
			//! \param[in] stream data source
			//! \return false if an error occurred
			bool PngOpen(IImage **image, IO::IStream *stream);
			//! \brief save to a PNG image
			//! \param[in] stream where destination data will be written
			//! \param[in] image already opened
			//! \return false if an error occurred
			bool PngSave(IO::IStream *stream, IImage *image);
		}
	}
}