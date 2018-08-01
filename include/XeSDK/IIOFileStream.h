#pragma once
#include <XeSDK/IIOStream.h>

namespace Xe { namespace IO {
	struct IFileStream : public IStream
	{
		static const UID ID = 0x19176c1c0b1f4f7cULL;

		//! \brief obtain the name of the file
		//! \return file name
		virtual ctstring GetFilename() const = 0;
	};
} }