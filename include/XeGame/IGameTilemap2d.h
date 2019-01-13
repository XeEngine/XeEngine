#pragma once
#include <XeSDK/IDelegate.h>
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

	enum TilemapBufferSizeType
	{
		TilemapBuffer_16 = 16,
		TilemapBuffer_32 = 32,
		TilemapBuffer_64 = 64,
		TilemapBuffer_128 = 128,
		TilemapBuffer_256 = 256,
	};

	enum TilemapTileSizeType
	{
		TilemapTile_8 = 8,
		TilemapTile_16 = 16,
		TilemapTile_24 = 24,
		TilemapTile_32 = 32,
		TilemapTile_48 = 48,
		TilemapTile_64 = 64,
	};

	typedef Xe::Math::Vector2<TilemapBufferSizeType> TilemapBufferSize;
	typedef Xe::Math::Vector2<TilemapTileSizeType> TilemapTileSize;

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

	struct TilemapRequestTilesArgs
	{
		TilemapData Destination;
		Math::Vector2i Position;
	};

	struct TilemapDrawVertex
	{
		float x, y, u, v;
	};

	struct TilemapDrawIndex
	{
		u16 VertexIndex;
	};

	struct TilemapDrawList
	{
		TilemapDrawVertex* VerticesData;
		TilemapDrawIndex* IndicesData;
		size_t VerticesCount;
		size_t IndicesCount;
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

		virtual const Xe::Math::Vector2i& GetCameraSize() = 0;
		virtual void SetCameraSize(const Xe::Math::Vector2i& cameraSize) = 0;

		virtual const Xe::Math::Vector2i& GetCameraPosition() = 0;
		virtual void SetCameraPosition(const Xe::Math::Vector2i& cameraPosition) = 0;

		virtual const TilemapTileSize& GetTileSize() = 0;
		virtual void SetTileSize(const TilemapTileSize& tileSize) = 0;

		virtual const TilemapBufferSize& GetBufferSize() = 0;
		virtual void SetBufferSize(const TilemapBufferSize& bufferSize) = 0;

		virtual bool GetBuffer(TilemapData* layer) = 0;

		virtual void SetTileset(const TilesetProperties& tileset) = 0;

		virtual void Update(double deltaTime) = 0;
		virtual void Flush() = 0;
		virtual void Draw(int flags) = 0;
	};
} }