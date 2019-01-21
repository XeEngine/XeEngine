#pragma once
#include <XeSDK/IDelegate.h>
#include <XeSDK/XeCollectionsArray.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeMathRectangle.h>
#include <XeSDK/XeGraphicsColor.h>

namespace Xe { namespace Game {
	enum TilemapDrawFlags
	{
		TilemapDraw_Simple = 0,
		TilemapDraw_Flip = 1,
		TilemapDraw_Palette = 2,
		TilemapDraw_Parallax = 4,
		TilemapDraw_Color = 8,
	};

	struct TilesetProperties
	{
		int TilesPerRow;
	};

	struct TileData
	{
		union
		{
			unsigned int Data;
			struct
			{
				unsigned int Tile : 29;
				unsigned int Rotate : 1;
				unsigned int Flip : 1;
				unsigned int Mirror : 1;

			};
		};

		operator int() const { return Data; }
		int operator = (int x) { Data = x; return x; }
	};

	static_assert(sizeof(TileData) == sizeof(TileData::Data), "TileData size is different than expected");

	struct TilemapData
	{
		TileData* Tilemap;
		Math::Vector2i Size;
		size_t Stride;

		inline void SetTile(unsigned x, unsigned y, TileData tile)
		{
			((TileData*)((u8*)Tilemap + Stride * y))[x] = tile;
		}
	};

	struct TileFrame
	{
		TileData Tile;
		float DelayMs;
	};

	struct TilemapRequestTilesArgs
	{
		TilemapData Destination;
		Math::Vector2i Position;
	};

	struct TilemapDrawVertex
	{
		float x, y, u, v;
		u16 ColorIndex, TextureModeIndex;
	};

	struct TilemapDrawIndex
	{
		u16 VertexIndex;
	};

	struct TilemapDrawList
	{
		TilemapDrawVertex* VerticesData;
		TilemapDrawIndex* IndicesData;
		Xe::Graphics::Color* ColorsData;
		float* TextureModeData;

		size_t VerticesCount;
		size_t IndicesCount;
		size_t ColorsCount;
		size_t TextureModeCount;
	};

	struct TilemapDrawArgs
	{
		std::vector<TilemapDrawList> Draws;
		Math::Vector2f Position;
		Math::Vector2f Size;
	};

	interface ITilemap2d;

	template <class T>
	struct TilemapArgs : public EventArgs<ITilemap2d*, T*> {};

	template <class T>
	interface TilemapDelegate :
		public Xe::IDelegate<const TilemapArgs<T>&> {};

	typedef TilemapDelegate<TilemapRequestTilesArgs> TilemapRequestTilesDelegate;

	typedef TilemapDelegate<TilemapDrawArgs> TilemapDrawDelegate;

	interface ITilemap2d : public IObject
	{
		virtual ~ITilemap2d() {};

		virtual void SetRequestTilesCallback(TilemapRequestTilesDelegate* delegate) = 0;
		virtual void SetDrawCallback(TilemapDrawDelegate* delegate) = 0;

		virtual const Xe::Graphics::Color& GetBackgroundColor() const = 0;
		virtual void SetBackgroundColor(const Xe::Graphics::Color& color) = 0;

		virtual const Xe::Math::Vector2i& GetCameraSize() const = 0;
		virtual void SetCameraSize(const Xe::Math::Vector2i& cameraSize) = 0;

		virtual const Xe::Math::Vector2f& GetCameraPosition() const = 0;
		virtual void SetCameraPosition(const Xe::Math::Vector2f& cameraPosition) = 0;

		virtual const Xe::Math::Vector2i& GetTileSize() const = 0;
		virtual void SetTileSize(const Xe::Math::Vector2i& tileSize) = 0;

		virtual bool GetTileSequence(TileData tile, std::vector<TileFrame>& frames) const = 0;
		virtual void AddTileSequence(TileData tile, const Xe::Collections::Array<TileFrame>& frames) = 0;
		virtual void RemoveTileSequence(TileData tile) = 0;

		virtual const Xe::Math::Vector2i& GetBufferSize() const = 0;
		virtual void SetBufferSize(const Xe::Math::Vector2i& bufferSize) = 0;

		virtual bool IsLayerVisible() const = 0;
		virtual void SetLayerVisible(bool visible) = 0;

		virtual bool GetBuffer(TilemapData* layer) = 0;

		virtual void SetTileset(const TilesetProperties& tileset) = 0;

		virtual void Update(double deltaTime) = 0;
		virtual void Flush() = 0;
		virtual void Draw(int flags) = 0;
	};
} }