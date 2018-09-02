#pragma once
#include <XeSDK/IIOStream.h>

namespace Xe { namespace IO {
	interface IFileStream : public IStream
	{
		//! \brief obtain the name of the file
		//! \return file name
		virtual ctstring GetFilename() const = 0;
	};
} }