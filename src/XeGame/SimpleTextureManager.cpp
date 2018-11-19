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

		bool m_ClutInvalidated;

		Xe::Graphics::ISurface* m_SurfacePalette;

		// Array of texture entries
		Texture m_Texture[MaxTexturesCount];

		u32 m_PaletteData[MaxClutsCount][ColorsCount];
		TexId m_PaletteSlot[MaxClutsCount];

		SimpleTextureManager(Xe::Graphics::IContext& context) :
			m_Context(context),
			m_TexturesCount(0),
			m_ClutsCount(0),
			m_ClutInvalidated(true),
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

		const Texture& GetInternalTextureConst(TexId texId) const
		{
			ASSERT((int)texId >= 0 || (int)texId < MaxTexturesCount);
			return m_Texture[(int)texId];
		}

		Texture& GetInternalTexture(TexId texId)
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
				if (texture.Palette[i] != ClutInvalid)
				{
					DeletePalette(texture.Palette[i]);
				}
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

				InvalidateClut();
			}
			else
			{
				LOGE("Clut slots full.\n");
			}

			return clutId;
		}

		void DeletePalette(ClutId clutId)
		{
			m_ClutsCount--;
			ResetClutSlotUsed((int)clutId);
		}

		void InvalidateClut()
		{
			m_ClutInvalidated = true;
		}

		void ValidateClut()
		{
			if (m_ClutInvalidated)
			{
				if (m_SurfacePalette == nullptr)
				{
					bool success = m_Context.CreateSurface(
						&m_SurfacePalette,
						Xe::Graphics::SurfaceType_Texture,
						Xe::Graphics::Size(ColorsCount, MaxClutsCount),
						Xe::Graphics::Color::Format_BGRA8888,
						Xe::Graphics::DataDesc(m_PaletteData, ColorsCount * sizeof(u32)));

					if (success)
					{
						m_ClutInvalidated = false;
					}
					else
					{
						LOGE("Unable to create the clut's surface.\n");
					}
				}
				else
				{
					Xe::Graphics::DataDesc desc;
					if (m_SurfacePalette->Lock(desc, Xe::Graphics::Lock_Write))
					{
						Memory::Copy((void*)desc.data, m_PaletteData, sizeof(m_PaletteData));
						m_SurfacePalette->Unlock();
						m_ClutInvalidated = false;
					}
					else
					{
						LOGW("Unable to lock the clut's surface; it will be recreated.\n");

						m_SurfacePalette->Release();
						m_SurfacePalette = nullptr;
						ValidateClut();
					}
				}
			}
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
				Texture& tex = GetInternalTexture(texId);

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
					LOGI("Texture %s opened as %i.\n", name, texId);
				}
				else {
					LOGE("Unable to allocate %s.\n", name);
					tex.Surface = nullptr;
				}
			}
			else {
				LOGW("There's no space to allocate %s.\n", name);
			}

			return texId;
		}

		TexId GetTextureId(ctstring name) const
		{
			ASSERT(name != nullptr);
			return Search(crc.Calculate(name));
		}

		Xe::Graphics::ISurface* GetSurface(TexId texId)
		{
			if (texId != TexInvalid)
			{
				return GetInternalTextureConst(texId).Surface;
			}
			else
			{
				LOGW("An invalid Id has been specified for %s.\n", "GetTexture");
			}

			return nullptr;
		}

		Xe::Graphics::ISurface* GetClutSurface()
		{
			ValidateClut();
			return m_SurfacePalette;
		}

		void SelectTexture(TexId texId, int slot)
		{
			if (texId != TexInvalid)
			{
				m_Context.SelectSurface(GetSurface(texId), slot);
			}
			else
			{
				LOGW("An invalid Id has been specified for %s.\n", "Select");
			}
		}

		void SelectClut(int slot)
		{
			m_Context.SelectSurface(GetClutSurface(), slot);
		}

		void GetUv(TexId texId, Math::Vector2f(uv)[4])
		{
		}

		void GetUvLTRB(TexId texId, Math::Vector2f(uv)[4], u16 left, u16 top, s16 right, s16 bottom)
		{
			if (texId != TexInvalid)
			{
				auto& texture = GetInternalTextureConst(texId);
				const Graphics::Size& size = texture.Surface->GetSize();
				uv[0].x = 1.0f / (float)size.x * (float)left;
				uv[0].y = 1.0f / (float)size.y * (float)top;
				uv[1].x = 1.0f / (float)size.x * (float)right;
				uv[1].y = uv[0].y;
				uv[2].x = uv[0].x;
				uv[2].y = 1.0f / (float)size.y * (float)bottom;
				uv[3].x = uv[1].x;
				uv[3].y = uv[2].y;
			}
			else
			{
				LOGW("An invalid Id has been specified for %s.\n", "GetUvLTRB");
			}
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
				LOGW("An invalid Id has been specified for %s.\n", "AddReference");
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
					return TexInvalid;
				}
			}
			else
			{
				LOGW("An invalid Id has been specified for %s.\n", "RemoveReference");
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
				LOGW("An invalid Id has been specified for %s.\n", "GetReferencesCount");
			}

			return 0;
		}

		ClutId GetDefaultClut(TexId texId) const
		{
			return GetInternalTextureConst(texId).Palette[0];
		}

		ClutId AllocateClut(TexId texId)
		{
			auto& texture = GetInternalTexture(texId);

			if (texture.ClutsCount > 0)
			{
				if (texture.ClutsCount < MaxSubClutsCount)
				{
					if (texture.ClutsCount < MaxSubClutsCount)
					{
						AddReference(texId);

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
						LOGW("The max cluts count %i has been reached.\n", MaxClutsCount);
					}
				}
				else
				{
					LOGW("The max subcluts of %i has been reached for TexId %i.\n", MaxSubClutsCount, texId);
				}
			}
			else
			{
				LOGW("The specified texture %i has no embedded palette.\n", texId);
			}

			return ClutInvalid;
		}

		ClutId FreeClut(ClutId clutId)
		{
			if (clutId != ClutInvalid)
			{
				ASSERT((int)clutId >= 0 || (int)clutId < MaxClutsCount);

				TexId texId = GetTextureId(clutId);
				ASSERT(texId != TexInvalid);

				Texture& texture = GetInternalTexture(texId);
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
				
				DeletePalette(clutId);
				return RemoveReference(texId) != TexInvalid ? texture.Palette[0] : ClutInvalid;
			}
			else
			{
				LOGW("An invalid Id has been specified for %s.\n", "FreeClut");
			}

			return ClutInvalid;
		}

		TexId GetTextureId(ClutId clutId) const
		{
			if (clutId != ClutInvalid)
			{
				ASSERT((int)clutId >= 0 || (int)clutId < MaxClutsCount);
				return m_PaletteSlot[(int)clutId];
			}

			return TexInvalid;
		}

		float GetClutIndexf(ClutId clutId) const
		{
			return (float)(int)clutId / MaxClutsCount + 0.5f / MaxClutsCount;
		}

		void LockClut(ClutId clutId, ClutData& clutData)
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
				LOGE("An invalid Id has been specified for %s.\n", "GetClutData");
			}
		}

		void UnlockClut(ClutId clutId, ClutData& clutData)
		{
			InvalidateClut();
		}

		bool GetProfile(TextureManagerProfile& profile)
		{
#ifdef DEVELOPMENT
			profile.TexturesCount = m_TexturesCount;
			profile.MaxTexturesCount = MaxTexturesCount;
			profile.ClutsCount = m_ClutsCount;
			profile.MaxClutsCount = MaxClutsCount;
			profile.TexturesReferencesCount = 0;
			profile.MaxClutsCountPerTexture = MaxSubClutsCount;

			for (int i = 0; i < MaxTexturesCount; i++)
			{
				Texture& tex = m_Texture[i];
				if (tex.Hash != 0)
				{
					auto& size = tex.Surface->GetSize();
					TextureManagerProfileEntry e;
					e.Name = tex.Name;
					e.TexId = tex.Id;
					e.Width = size.x;
					e.Height = size.y;
					e.ReferencesCount = tex.ReferencesCount;

					for (int j = 0; j < MaxSubClutsCount; j++)
					{
						if (tex.Palette[j] != ClutInvalid)
						{
							e.Cluts.push_back(tex.Palette[j]);
						}
					}

					profile.TexturesReferencesCount += tex.ReferencesCount;
					profile.Textures.push_back(e);
				}
			}

			return true;
#else
			profile.TexturesCount = 0;
			profile.MaxTexturesCount = 0;
			profile.ClutsCount = 0;
			profile.MaxClutsCount = 0;
			profile.TexturesReferencesCount = 0;
			profile.MaxClutsCountPerTexture = 0;
			return false;
#endif
		}
		#pragma endregion
	};

	void TextureManager::FactorySimple(ITextureManager*& pTextureManager, Xe::Graphics::IContext& context)
	{
		pTextureManager = new SimpleTextureManager(context);
	}

	void Factory(ITextureManager** ppTextureManager, Xe::Graphics::IContext* pContext)
	{
		*ppTextureManager = new SimpleTextureManager(*pContext);
	}
} }