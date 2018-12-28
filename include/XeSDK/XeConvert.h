#pragma once
#include <XeSDK/XeStringSpan.h>
#include <XeSDK/IIOStream.h>
#include <vector>

namespace Xe { namespace Convert {
	size_t FromBase64String(IO::IStream* pStream, const StringSpan& string);
	size_t FromBase64String(IO::IStream* pOutStream, IO::IStream* pInStream, size_t length);
	size_t ToBase64String(IO::IStream* pOutStream, IO::IStream* pInStream, size_t length);
} }