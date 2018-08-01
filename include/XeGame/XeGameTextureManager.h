#pragma once
#include "ITextureManager.h"

namespace Xe { namespace IO {
	struct IStream;
	struct IFileStream;
} }

namespace Xe { namespace Storage {
	struct IStorage;
} }

namespace Xe { namespace Game {
	class TextureManager
	{
		static void FactorySimple(ITextureManager*& pTextureManager);

		static TexId Open(ITextureManager& textureManager, Storage::IStorage& storage, ctstring path);
		static TexId Open(ITextureManager& textureManager, IO::IFileStream& fileStream);
		static TexId Open(ITextureManager& textureManager, IO::IStream& stream, ctstring name);
	};
} }