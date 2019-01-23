#pragma once
#include <XeGame/IGameTilemapLayer.h>

namespace Xe { namespace Game {
	class CTilemapLayer : public ITilemapLayer
	{
		struct Flags
		{
			u32 Visible : 1;
			u32 Locked : 1;
		};

		String m_Name;
		Xe::Math::Vector2u m_BufferSize;
		Xe::Math::Vector2f m_ScrollOffset;
		Xe::Math::Vector2f m_ScrollMultiplier;
		TexId m_TextureId;
		ClutId m_ClutId;
		size_t m_TilesPerRow;
		Flags m_Flags;

		LockType m_LockType;
		TileData* m_LockedData;

	public:
		TileData* m_Data;

		CTilemapLayer();
		~CTilemapLayer();

		const String& GetName() const;
		void SetName(const StringSpan& name);

		const Xe::Math::Vector2u& GetBufferSize() const;
		void SetBufferSize(const Xe::Math::Vector2u& size);

		const Xe::Math::Vector2f& GetScrollOffset() const;
		void SetScrollOffset(const Xe::Math::Vector2f& offset);

		const Xe::Math::Vector2f& GetScrollMultiplier() const;
		void SetScrollMultiplier(const Xe::Math::Vector2f& multiplier);

		Xe::Math::Vector2f ProcessPosition(const Xe::Math::Vector2f& cameraPosition) const;

		bool IsVisible() const;
		void SetVisible(bool visibility);

		TexId GetTexture() const;
		size_t GetTilesPerRow() const;
		void SetTexture(TexId textureId, size_t tilesPerRow);

		ClutId GetPalette();
		void SetPalette(ClutId clutId);

		bool IsLocked() const;
		bool Lock(TilemapData& tilemapData, LockType lockType);
		void Unlock();
	};
} }