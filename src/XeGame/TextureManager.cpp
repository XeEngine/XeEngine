#include "pch.h"
#include "TextureManager.h"

using namespace Xe::IO;
using namespace Xe::Graphics;
using namespace Xe::Graphics::Imaging;
using namespace Xe::Storage;

namespace Xe { namespace Game {
	TexId ITextureManager::Open(ITextureManager& textureManager, Storage::IStorage& storage, ctstring path)
	{
		TexId texId;
		IFileStream* pFileStream;

		if (storage.FileOpen(&pFileStream, path, Access_Read, Mode_OpenIfExists) == Error::OK)
		{
			texId = Open(textureManager, *pFileStream);
		}
		else
		{
			texId = TexInvalid;
			Logger::DebugError("Unable to open texture %s.\n", path);
		}
		return texId;
	}

	TexId ITextureManager::Open(ITextureManager& textureManager, IO::IFileStream& fileStream)
	{
		return Open(textureManager, fileStream, fileStream.GetFilename());
	}

	TexId ITextureManager::Open(ITextureManager& textureManager, IO::IStream& stream, ctstring name)
	{
		TexId texId;
		IImage* pImage;

		bool r = PngOpen(&pImage, &stream);
		if (r)
		{
			texId = textureManager.Open(*pImage, name);
		}
		else
		{
			texId = TexInvalid;
			Logger::DebugError("Texture %s is not a valid PNG.\n", name);
		}

		return texId;
	}
} }