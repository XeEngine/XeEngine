#pragma once
#include <XeSDK/IIoStream.h>
#include <XeSDK/IGraphicsFont.h>

namespace Xe {
	namespace Graphics {
		namespace Font {
			//! \brief Open Microsoft FNT file
			bool OpenFnt(IFont **font, IO::IStream *stream);
		}
	}
}