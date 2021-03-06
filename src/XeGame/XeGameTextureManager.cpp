#include "pch.h"
#include <XeGame/XeGameTextureManager.h>
#include "XeGameTextureManagerDef.h"
#include <XeGame/ITextureManager.h>

using namespace Xe::IO;
using namespace Xe::Graphics;
using namespace Xe::Graphics::Imaging;
using namespace Xe::Storage;

namespace Xe { namespace Game {
	TexId TextureManager::Open(ITextureManager& textureManager, Storage::IStorage& storage, ctstring path)
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
			LOGE("Unable to open texture %s.\n", path);
		}
		return texId;
	}

	TexId TextureManager::Open(ITextureManager& textureManager, IO::IFileStream& fileStream)
	{
		return Open(textureManager, fileStream, fileStream.GetFileName());
	}

	TexId TextureManager::Open(ITextureManager& textureManager, IO::IStream& stream, ctstring name)
	{
		TexId texId;
		IImage* pImage;

		bool r = PngOpen(&pImage, &stream);
		if (r)
		{
			texId = textureManager.Create(pImage, name);
		}
		else
		{
			texId = TexInvalid;
			LOGE("Texture %s is not a valid PNG.\n", name);
		}

		return texId;
	}
} }