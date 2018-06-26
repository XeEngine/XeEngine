#include "pch.h"
#include "TextureManager.h"

namespace Xe { namespace Game {

	struct Texture {
		//! \brief Cifratura del nome per velocizzare la ricerca.
		u64 Hash;
		//! \brief Identificativo della texture.
		TexId Id;
		//! \brief Palette slot
		PalId Palette;
		u32 Dummy;
		//! \brief Numero di riferimenti.
		s32 ReferencesCount;
		//! \brief Percorso+Nome della texture caricata.
		String Name;
		//! \brief Storage usato per caricare la texture.
		Storage::IStorage* Storage;
		//! \brief Immagine a cui è fatta riferimento la texture
		/** \details L'elemento può essere nullptr; in tal caso può essere
		* riaperto grazie a Name.
		*/
		Graphics::Imaging::IImage* Image;
		//! \brief Superfice contenente la texture.
		Graphics::ISurface* Surface;
		//! \brief Locazione della texture all'interno della superfice
		Math::Rectangle<u16> Rect;
	};

	//struct SimpleTextureManager : ITextureManager
	//{
	//	static const svar MaxTexturesCount = 256;
	//	static const svar MaxPalettesCount = 256;

	//	Graphics::IContext* m_pContext;
	//	// Tiene il conto di tutte le texture mantenute in cache.
	//	svar m_UsedTextureCount;
	//	// Tiene il conto di sole le texture con riferimenti attivi.
	//	svar m_ActivedTextureCount;
	//	// Array contentene la robaccia
	//	Texture* m_Texture;

	//	u32 m_PaletteData[MaxPalettesCount][0x100];
	//	u8 m_PaletteSlot[MaxPalettesCount];
	//	Xe::Graphics::ISurface* m_SurfacePalette;

	//	SimpleTextureManager()
	//	{

	//	}

	//	~SimpleTextureManager()
	//	{

	//	}

	//	#pragma region Inherited via ITextureManager
	//	TexId Open(Graphics::Imaging::IImage& image, ctstring name)
	//	{
	//		// La texture è già stata caricata in precedenza?
	//		TexId texId = Search(path);
	//		if (texId != TexInvalid)
	//		{
	//			AddReference(texId);
	//			return texId;
	//		}

	//		TexId texId = CheckSpace(pImage);
	//		if (texId >= 0) {
	//			Texture& tex = m_Texture[texId];
	//			// Riusciamo ad allocare la texture?
	//			if (AllocateTexture(pImage, &tex.Surface, tex.Rect)) {
	//				PalId paletteSlot;
	//				if (Color::IsIndexed(pImage->GetFormat())) {
	//					paletteSlot = AllocatePalette(pImage);
	//					SetPaletteSlot(paletteSlot, true);
	//				}
	//				else {
	//					paletteSlot = PalInvalid;
	//				}

	//				// Tutto è andato a buon fine, texture caricata!
	//				tex.Hash = String::GetHashFNV1a(name, String::GetLength(name));
	//				tex.Id = texId;
	//				tex.Palette = paletteSlot;
	//				tex.ReferencesCount = 1;
	//				tex.Name = name;
	//				tex.Storage = nullptr;
	//				tex.Image = nullptr;
	//				pImage->Release();
	//				m_ActivedTextureCount++;
	//				m_UsedTextureCount++;
	//				Logger::Info("Texture %s opened as %i.\n", name texId);
	//			}
	//			else {
	//				Logger::Error("Unable to allocate %s.\n", name);
	//				tex.Surface = nullptr;
	//			}
	//		}
	//		else {
	//			Logger::Warning("There's no space to allocate %s.\n", name);
	//		}

	//		return texId;
	//	}

	//	TexId Get(ctstring name)
	//	{
	//		return TexId();
	//	}

	//	TexId AddRefence(TexId texId)
	//	{
	//		if (texId != TexInvalid)
	//		{
	//			m_Texture[texId].ReferencesCount++;
	//		}
	//	}

	//	TexId RemoveRefence(TexId texId)
	//	{
	//		if (texId != TexInvalid)
	//		{

	//		}
	//	}

	//	TexId GetReferencesCount(TexId texId)
	//	{
	//		if (texId != TexInvalid)
	//		{

	//		}
	//	}
	//	#pragma endregion
	//};

	void FactorySimple(ITextureManager*& ppTextureManager)
	{
		//ppTextureManager = new SimpleTextureManager;
	}
} }