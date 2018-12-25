#pragma once
#include <XeSDK/IIOStream.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace IO {
	interface IFileStream : public IStream
	{
		//! \brief obtain the name of the file
		//! \return file name
		virtual const String& GetFileName() const = 0;
	};
} }