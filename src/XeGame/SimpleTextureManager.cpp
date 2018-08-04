#include "pch.h"
#include <XeGame/XeGameTextureManager.h>
#include "XeSDK/XeCore.h"
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/XeCryptoCrc32.h>
#include <XeSDK/XeCryptoCrc64.h>

namespace Xe { namespace Game {

	struct SimpleTextureManager : ITextureManager
	{
		static const svar MaxTexturesCount = 256;
		static const svar MaxClutsCount = 256;
		static const svar MaxSubClutsCount = 16;
		static const int ColorsCount = 0x100;

#ifdef PLATFORM_64
		typedef u64 Hash;
		Xe::Crypto::Crc64 crc;
#else
		typedef u32 Hash;
		Xe::Crypto::Crc32 crc;
#endif

		struct Texture
		{
			Hash Hash;
			TexId Id;
			ClutId Palette[MaxSubClutsCount];
			int ReferencesCount;
			int ClutsCount;
			Graphics::ISurface* Surface;
#ifdef DEVELOPMENT
			std::string Name;
#endif
		};

		Graphics::IContext& m_Context;

		// Tiene il conto di sole le texture con riferimenti attivi.
		int m_TexturesCount;

		// Takes count of the used cluts.
		int m_ClutsCount;

		Xe::Graphics::ISurface* m_SurfacePalette;

		// Array of texture entries
		Texture m_Texture[MaxTexturesCount];

		u32 m_PaletteData[MaxClutsCount][ColorsCount];
		TexId m_PaletteSlot[MaxClutsCount];

		SimpleTextureManager(Xe::Graphics::IContext& context) :
			m_Context(context),
			m_TexturesCount(0),
			m_SurfacePalette(nullptr)
		{
			for (int i = 0; i < lengthof(m_Texture); i++)
			{
				Texture& tex = m_Texture[i];
				tex.Hash = 0;
				tex.Id = 0;

				for (int j = 0; j < MaxSubClutsCount; j++)
				{
					tex.Palette[j] = ClutInvalid;
				}
			}

			for (int i = 0; i < lengthof(m_PaletteSlot); i++)
			{
				m_PaletteSlot[i] = TexInvalid;
			}

			Xe::Memory::Fill(m_PaletteData, 0, sizeof(m_PaletteData));
		}

		~SimpleTextureManager()
		{
			for (int i = 0; i < lengthof(m_Texture) && m_TexturesCount > 0; i++)
			{
				if (m_Texture[i].Hash != 0)
				{
					DeleteTexture(m_Texture[i]);
					m_TexturesCount--;
				}
			}

			if (m_SurfacePalette)
			{
				m_SurfacePalette->Release();
			}

			ASSERT(m_TexturesCount == 0);
		}

		TexId Search(Hash hash) const
		{
			for (int i = 0; i < lengthof(m_Texture); i++)
			{
				if (m_Texture[i].Hash == hash)
				{
					return (TexId)i;
				}
			}

			return TexInvalid;
		}

		TexId GetFreeTextureSlot() const
		{
			return Search((Hash)0);
		}

		ClutId GetFreeClutSlot() const
		{
			for (int i = 0; i < lengthof(m_PaletteSlot); i++)
			{
				if (!IsClutSlotUsed(i))
				{
					return (ClutId)i;
				}
			}

			return ClutInvalid;
		}

		bool IsClutSlotUsed(int clutIndex) const
		{
			ASSERT(clutIndex >= 0 && clutIndex < MaxClutsCount);
			return m_PaletteSlot[clutIndex] != TexInvalid;
		}

		void SetClutSlotUsed(TexId texId, ClutId clutId)
		{
			SetClutSlotUsed(texId, (int)clutId);
		}

		void SetClutSlotUsed(TexId texId, int clutIndex)
		{
			ASSERT(clutIndex >= 0 && clutIndex < MaxClutsCount);
			m_PaletteSlot[clutIndex] = texId;
		}

		void ResetClutSlotUsed(ClutId clutId)
		{
			ResetClutSlotUsed((int)clutId);
		}

		void ResetClutSlotUsed(int clutIndex)
		{
			ASSERT(clutIndex >= 0 && clutIndex < MaxClutsCount);
			m_PaletteSlot[clutIndex] = TexInvalid;
		}

		const Texture& GetTextureConst(TexId texId) const
		{
			ASSERT((int)texId >= 0 || (int)texId < MaxTexturesCount);
			return m_Texture[(int)texId];
		}

		Texture& GetTexture(TexId texId)
		{
			ASSERT((int)texId >= 0 || (int)texId < MaxTexturesCount);
			return m_Texture[(int)texId];
		}

		u32* GetInternalClut(ClutId clutId)
		{
			ASSERT((int)clutId >= 0 || (int)clutId < MaxClutsCount);
			return m_PaletteData[(int)clutId];
		}

		bool AllocateTexture(Xe::Graphics::Imaging::IImage& image, Xe::Graphics::ISurface*& pSurface)
		{
			ASSERT(pSurface != nullptr);
			return Xe::Graphics::Imaging::CreateSurface(&m_Context, &pSurface,
				Xe::Graphics::SurfaceType_Texture, &image);
		}

		void DeleteTexture(Texture& texture)
		{
			texture.Hash = 0;
			texture.Id = 0;
			texture.ReferencesCount = 0;
			texture.ClutsCount = 0;

			for (int i = 0; i < MaxSubClutsCount; i++)
			{
				texture.Palette[i] = ClutInvalid;
			}

			if (texture.Surface) { texture.Surface->Release(); texture.Surface = nullptr; }

			ASSERT(m_TexturesCount > 0);
			m_TexturesCount--;
		}

		ClutId CreateClut(TexId texId, Xe::Graphics::Imaging::IImage& image)
		{
			ClutId clutId = GetFreeClutSlot();
			if (clutId != ClutInvalid)
			{
				m_ClutsCount++;
				SetClutSlotUsed(texId, clutId);

				auto paletteData = GetInternalClut(clutId);
				image.ReadPalette(paletteData, ColorsCount);

				for (int i = 1; i < MaxClutsCount - 1; i++)
				{
					u32 data = paletteData[i];
					u8 r = data >> 0;
					u8 g = data >> 8;
					u8 b = data >> 16;
					u8 a = data >> 24;
					data = b | (g << 8) | (r << 16) | (a << 24);
					paletteData[i] = data;
				}

				if (m_SurfacePalette)
					m_SurfacePalette->Release();

				bool success = m_Context.CreateSurface(
					&m_SurfacePalette,
					Xe::Graphics::SurfaceType_Texture,
					Xe::Graphics::Size(ColorsCount, MaxClutsCount),
					Xe::Graphics::Color::Format_BGRA8888,
					Xe::Graphics::DataDesc(m_PaletteData, ColorsCount * sizeof(u32)));

				if (!success)
				{
					clutId = ClutInvalid;
					Logger::DebugError("Unable to create the clut's texture.");
				}
			}
			else
			{
				Logger::DebugError("Clut slots full.");
			}

			return clutId;
		}

		void DeletePalette(ClutId clutId)
		{
			ResetClutSlotUsed((int)clutId);
		}

		#pragma region Inherited via ITextureManager
		TexId Create(Graphics::Imaging::IImage& image, ctstring name)
		{
			ASSERT(name != nullptr);

			// Has the texture previously loaded?
			Hash hash = crc.Calculate(name);
			TexId texId = Search(hash);
			if (texId != TexInvalid)
			{
				AddReference(texId);
				return texId;
			}

			texId = GetFreeTextureSlot();
			if (texId != TexInvalid)
			{
				Texture& tex = GetTexture(texId);

				// Check if it is possible to allocate the texture.
				if (AllocateTexture(image, tex.Surface))
				{
					ClutId paletteSlot;
					if (Xe::Graphics::Color::IsIndexed(image.GetFormat()))
					{
						paletteSlot = CreateClut(texId, image);
					}
					else
					{
						paletteSlot = ClutInvalid;
					}

					// Texture loaded!
					tex.Hash = hash;
					tex.Id = texId;
					tex.Palette[0] = paletteSlot;
					tex.ReferencesCount = 1;
					tex.ClutsCount = paletteSlot != ClutInvalid ? 1 : 0;
#ifdef DEVELOPMENT
					tex.Name = name;
#endif
					m_TexturesCount++;
					Logger::Info("Texture %s opened as %i.\n", name, texId);
				}
				else {
					Logger::Error("Unable to allocate %s.\n", name);
					tex.Surface = nullptr;
				}
			}
			else {
				Logger::Warning("There's no space to allocate %s.\n", name);
			}

			return texId;
		}

		TexId Get(ctstring name) const
		{
			ASSERT(name != nullptr);
			return Search(crc.Calculate(name));
		}

		TexId AddReference(TexId texId)
		{
			if (texId != TexInvalid)
			{
				ASSERT((int)texId >= 0 || (int)texId < MaxTexturesCount);
				m_Texture[(int)texId].ReferencesCount++;
			}
			else
			{
				Logger::DebugWarning("An invalid Id has been specified for %s.\n", "AddReference");
			}

			return texId;
		}

		TexId RemoveReference(TexId texId)
		{
			if (texId != TexInvalid)
			{
				ASSERT((int)texId >= 0 || (int)texId < MaxTexturesCount);
				if (--m_Texture[(int)texId].ReferencesCount == 0)
				{
					DeleteTexture(m_Texture[(int)texId]);
				}
			}
			else
			{
				Logger::DebugWarning("An invalid Id has been specified for %s.\n", "RemoveReference");
			}

			return texId;
		}

		int GetReferencesCount(TexId texId) const
		{
			if (texId != TexInvalid)
			{
				ASSERT((int)texId >= 0 || (int)texId < MaxTexturesCount);
				return m_Texture[(int)texId].ReferencesCount;
			}
			else
			{
				Logger::DebugWarning("An invalid Id has been specified for %s.\n", "GetReferencesCount");
			}

			return 0;
		}

		ClutId GetDefaultClut(TexId texId) const
		{
			return GetTextureConst(texId).Palette[0];
		}

		ClutId AllocateClut(TexId texId)
		{
			auto& texture = GetTexture(texId);

			if (texture.ClutsCount > 0)
			{
				if (texture.ClutsCount < MaxSubClutsCount)
				{
					if (texture.ClutsCount < MaxSubClutsCount)
					{
						ClutId clutId = GetFreeClutSlot();
						ASSERT(clutId != ClutInvalid);

						m_ClutsCount++;
						SetClutSlotUsed(texId, clutId);

						texture.Palette[texture.ClutsCount] = clutId;
						Xe::Memory::Copy(
							GetInternalClut(texture.Palette[texture.ClutsCount]),
							GetInternalClut(texture.Palette[0]),
							ColorsCount * sizeof(u32));

						texture.ClutsCount++;
						return clutId;
					}
					else
					{
						Logger::DebugWarning("The max cluts count %i has been reached.\n", MaxClutsCount);
					}
				}
				else
				{
					Logger::DebugWarning("The max subcluts of %i has been reached for TexId %i.\n", MaxSubClutsCount, texId);
				}
			}
			else
			{
				Logger::DebugWarning("The specified texture %i has no embedded palette.\n", texId);
			}

			return ClutInvalid;
		}

		ClutId FreeClut(ClutId clutId)
		{
			if (clutId != ClutInvalid)
			{
				ASSERT((int)clutId >= 0 || (int)clutId < MaxClutsCount);

				TexId texId = GetTexture(clutId);
				ASSERT(texId != TexInvalid);

				Texture& texture = GetTexture(texId);
				bool clutsToFreeFound = false;
				for (int i = 1; i < texture.ClutsCount; i++)
				{
					if (texture.Palette[i] == clutId)
					{
						clutsToFreeFound = true;
						for (int j = i; j < MaxSubClutsCount - 1; j++)
						{
							texture.Palette[j] = texture.Palette[j + 1];
						}
						texture.Palette[MaxSubClutsCount - 1] = ClutInvalid;

						break;
					}
				}
				ASSERT(clutsToFreeFound == true);
				
				m_PaletteSlot[(int)clutId] = TexInvalid;
				return texture.Palette[0];
			}
			else
			{
				Logger::DebugWarning("An invalid Id has been specified for %s.\n", "FreeClut");
			}

			return ClutInvalid;
		}

		TexId GetTexture(ClutId clutId) const
		{
			if (clutId != ClutInvalid)
			{
				ASSERT((int)clutId >= 0 || (int)clutId < MaxClutsCount);
				return m_PaletteSlot[(int)clutId];
			}

			return TexInvalid;

		}

		void GetClutData(ClutId clutId, ClutData& clutData)
		{
			ASSERT((int)clutId >= 0 || (int)clutId < MaxClutsCount);
			if (clutId != ClutInvalid)
			{
				clutData.Data = GetInternalClut(clutId);
				clutData.ColorsCount = ColorsCount;
				clutData.Pitch = ColorsCount * sizeof(u32);
			}
			else
			{
				Logger::DebugError("An invalid Id has been specified for %s.\n", "GetClutData");
			}
		}

		bool GetProfile(TextureManagerProfile& profile, int maxEntriesCount)
		{
#ifdef DEVELOPMENT
			profile.TexturesCount = m_TexturesCount;
			profile.MaxTexturesCount = MaxTexturesCount;
			profile.ClutsCount = m_ClutsCount;
			profile.MaxClutsCount = MaxClutsCount;
			profile.TexturesReferencesCount = 0;
			profile.MaxClutsCountPerTexture = MaxSubClutsCount;

			int entries = Math::Min(m_TexturesCount, maxEntriesCount);
			for (int i = 0, j = 0; i < MaxTexturesCount && j < entries; i++)
			{
				Texture& tex = m_Texture[i];
				if (tex.Hash != 0)
				{
					auto& size = tex.Surface->GetSize();
					auto& e = profile.Textures[j];
					e.Name = tex.Name;
					e.Width = size.x;
					e.Height = size.y;
					e.ReferencesCount = tex.ReferencesCount;
					e.ClutsCount = tex.ClutsCount;

					profile.TexturesReferencesCount += tex.ReferencesCount;
					j++;
				}
			}

			return true;
#else
			return false;
#endif
		}
		#pragma endregion
	};

	void TextureManager::FactorySimple(ITextureManager*& pTextureManager, Xe::Graphics::IContext& context)
	{
		pTextureManager = new SimpleTextureManager(context);
	}
} }