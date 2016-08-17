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
				static const UID ID = 0x3355c4cc25f549d1ULL;

				static bool Open(Xefs **xfs, IO::IStream *stream);
			};
		}
	}
}