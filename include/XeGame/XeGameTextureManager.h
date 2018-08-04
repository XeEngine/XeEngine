#pragma once
#include "ITextureManager.h"

namespace Xe { namespace IO {
	struct IStream;
	struct IFileStream;
} }

namespace Xe { namespace Storage {
	class IStorage;
} }

namespace Xe { namespace Game {
	struct TextureManager
	{
		static void FactorySimple(ITextureManager*& pTextureManager, Xe::Graphics::IContext& context);

		static TexId Open(ITextureManager& textureManager, Storage::IStorage& storage, ctstring path);
		static TexId Open(ITextureManager& textureManager, IO::IFileStream& fileStream);
		static TexId Open(ITextureManager& textureManager, IO::IStream& stream, ctstring name);
	};
} }