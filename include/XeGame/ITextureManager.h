#pragma once
#include <XeSDK/IStorage.h>

namespace Xe { namespace Game {
	struct _TexId;
	typedef _TexId* TexId;

	struct _PalId;
	typedef _PalId* PalId;

	const TexId TexInvalid = nullptr;
	const PalId PalInvalid = nullptr;

	struct ITextureManager
	{
		virtual TexId Open(Graphics::Imaging::IImage& image, ctstring name) = 0;
		virtual TexId Get(ctstring name) = 0;

		virtual TexId AddRefence(TexId) = 0;
		virtual TexId RemoveRefence(TexId) = 0;
		virtual TexId GetReferencesCount(TexId) = 0;

		static void FactorySimple(ITextureManager*& ppTextureManager);

		static TexId Open(ITextureManager& textureManager, Storage::IStorage& storage, ctstring path); 
		static TexId Open(ITextureManager& textureManager, IO::IFileStream& fileStream);
		static TexId Open(ITextureManager& textureManager, IO::IStream& stream, ctstring name);
	};
} }
