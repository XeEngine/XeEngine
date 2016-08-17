#pragma once
#include <XeSDK/IIoStream.h>

namespace Xe
{
	namespace IO
	{
		class IFileStream : public IStream
		{
		public:
			static const UID ID = 0x19176c1c0b1f4f7cULL;

			//! \brief obtain the name of the file
			//! \return file name
			virtual ctstring GetFilename() const = 0;
		};
	}
}