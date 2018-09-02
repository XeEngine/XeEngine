#pragma once
#include <XeSDK/IIOFileStream.h>
#include <XeSDK/IStorage.h>

namespace Xe
{
	namespace Storage
	{
		namespace Container
		{
			class Xefs : public IStorage
			{
				class XfsPrivate *xfs;
			public:
				static bool Open(Xefs **xfs, IO::IStream *stream);
			};
		}
	}
}