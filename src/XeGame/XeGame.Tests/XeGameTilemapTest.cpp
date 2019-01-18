#include "pch.h"
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeGame/XeGameTiledDocument.h>
#include <XeGame/XeGameTilemap2d.h>
#include <XeGame/IGameTilemap2d.h>

using namespace Xe;
using namespace Xe::Math;
using namespace Xe::Game;
using namespace Xe::Game::Tiled;

union MetaTile
{
	u32 Data;
	struct
	{
		u16 X, Y;
	};
};

template <class T>
struct TilemapGenericDelegateTest : public TilemapDelegate<T>
{
	T args;

	void operator()(const TilemapArgs<T>& args)
	{
		this->args = *args.Arguments;
	}
};

template <>
struct TilemapGenericDelegateTest<TilemapRequestTilesArgs>
	: public TilemapDelegate<TilemapRequestTilesArgs>
{
	TilemapRequestTilesArgs args;
	int tilesWritten;

	void operator()(const TilemapArgs<TilemapRequestTilesArgs>& args)
	{
		this->args = *args.Arguments;

		auto dst = args.Arguments->Destination;
		auto tilemapOffset = (u8*)dst.Tilemap;
		auto stride = args.Arguments->Destination.Stride;

		tilesWritten = 0;
		for (int x = 0; x < args.Arguments->Destination.Size.x; x++)
		{
			for (int y = 0; y < args.Arguments->Destination.Size.y; y++)
			{
				MetaTile tile;
				tile.X = x;
				tile.Y = y;
				dst.SetTile(x, y, TileData{ tile.Data });
				tilesWritten++;
			}
		}
	}
};

struct TilemapRequestTilesDelegateTest :
	public TilemapGenericDelegateTest<TilemapRequestTilesArgs> {};

struct TilemapDrawDelegateTest :
	public TilemapGenericDelegateTest<TilemapDrawArgs> {};

TEST(XeGameTilemapTest, SetPropertiesTest)
{
	ObjPtr<ITilemap2d> tilemap;
	Vector2i cameraSize(123, 456);
	Vector2i cameraPos(222, 444);
	TilemapBufferSize bufferSize;

	Factory(&tilemap, nullptr);
	EXPECT_NE(nullptr, tilemap.Get());

	tilemap->SetCameraSize(cameraSize);
	tilemap->SetCameraPosition(cameraPos);
	tilemap->SetBufferSize(bufferSize);

	EXPECT_EQ(cameraSize, tilemap->GetCameraSize());
	EXPECT_EQ(cameraPos, tilemap->GetCameraPosition());
	EXPECT_EQ(bufferSize, tilemap->GetBufferSize());
}

TEST(XeGameTilemapTest, DontPerformCallbackTest)
{
	const int ExpectedSizeX = 123;
	const int ExpectedSizeY = 456;
	const int ExpectedStride = 789;
	Xe::Game::TileData* const ExpectedTilemap = (Xe::Game::TileData*)0xDEADB33F;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	tilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;
	args.Destination.Size.x = ExpectedSizeX;
	args.Destination.Size.y = ExpectedSizeY;
	args.Destination.Stride = ExpectedStride;
	args.Destination.Tilemap = ExpectedTilemap;
	tilemap->Flush();

	EXPECT_EQ(Vector2i(0, 0), tilemap->GetCameraSize());
	EXPECT_EQ(ExpectedSizeX, args.Destination.Size.x);
	EXPECT_EQ(ExpectedSizeY, args.Destination.Size.y);
	EXPECT_EQ(ExpectedStride, args.Destination.Stride);
	EXPECT_EQ(ExpectedTilemap, args.Destination.Tilemap);
}

TEST(XeGameTilemapTest, PerformRequestTilesCallbackBasicTest)
{
	const int CameraSizeX = 4;
	const int CameraSizeY = 2;
	const int UnexpectedStride = 789;
	Xe::Game::TileData* const UnexpectedTilemap = (Xe::Game::TileData*)0xDEADB33F;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	tilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;
	args.Destination.Size.x = 111111;
	args.Destination.Size.y = 222222;
	args.Destination.Stride = UnexpectedStride;
	args.Destination.Tilemap = UnexpectedTilemap;

	tilemap->SetTileSize({ TilemapTile_16, TilemapTile_16 });
	tilemap->SetBufferSize(TilemapBufferSize(TilemapBuffer_32, TilemapBuffer_16));
	tilemap->SetCameraSize(Math::Vector2i(CameraSizeX, CameraSizeY));

	TilemapData layer;
	layer.Stride = 0;
	EXPECT_TRUE(tilemap->GetBuffer(&layer));
	EXPECT_NE(nullptr, layer.Tilemap);
	EXPECT_NE(0, layer.Stride);
	EXPECT_NE(0, layer.Size.x);
	EXPECT_NE(0, layer.Size.y);
	Memory::Fill(layer.Tilemap, 0xCC, layer.Stride * layer.Size.y);

	tilemap->Flush();

	EXPECT_EQ(CameraSizeX, args.Destination.Size.x);
	EXPECT_EQ(CameraSizeY, args.Destination.Size.y);
	EXPECT_NE(UnexpectedStride, args.Destination.Stride);
	EXPECT_NE(UnexpectedTilemap, args.Destination.Tilemap);
	EXPECT_NE(nullptr, args.Destination.Tilemap);
	EXPECT_EQ(CameraSizeX * CameraSizeY, delegateTest.tilesWritten);

	EXPECT_TRUE(tilemap->GetBuffer(&layer));
	for (int x = 0; x < CameraSizeX; x++)
	{
		for (int y = 0; y < CameraSizeY; y++)
		{
			auto pTile = (MetaTile*)&layer.Tilemap[x + y * layer.Size.x];
			EXPECT_EQ(x, pTile->X);
			EXPECT_EQ(y, pTile->Y);
		}
	}
}

TEST(XeGameTilemapTest, CheckBoundariesOnRequestTilesCallbackTest)
{
	const int CameraSizeX = 99;
	const int CameraSizeY = 99;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	tilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;

	tilemap->SetTileSize({ TilemapTile_16, TilemapTile_16 });
	tilemap->SetBufferSize(TilemapBufferSize(TilemapBuffer_32, TilemapBuffer_16));
	tilemap->SetCameraSize(Math::Vector2i(CameraSizeX, CameraSizeY));

	tilemap->Flush();

	EXPECT_EQ(32, args.Destination.Size.x);
	EXPECT_EQ(16, args.Destination.Size.y);
	EXPECT_EQ(32 * sizeof(TileData), args.Destination.Stride);
}

TEST(XeGameTilemapTest, AddAndRemoveTileSequenceTest)
{
	const TileData tile{ 123 };
	TileData tile2{ tile.Data };
	tile2.Flip = tile2.Mirror = tile2.Rotate = 1;
	TileFrame Sequence[] =
	{
		{ 10, 1.0 },
		{ 20, 2.0 },
		{ 30, 3.0 },
	};
	std::vector<TileFrame> seq;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);

	EXPECT_FALSE(tilemap->GetTileSequence(tile, seq));
	EXPECT_FALSE(tilemap->GetTileSequence(tile2, seq));
	tilemap->AddTileSequence(tile, Collections::Array<TileFrame>(Sequence));
	EXPECT_TRUE(tilemap->GetTileSequence(tile, seq));
	EXPECT_TRUE(tilemap->GetTileSequence(tile2, seq));

	EXPECT_EQ(lengthof(Sequence), seq.size());
	for (size_t i = 0; i < lengthof(Sequence); ++i)
	{
		EXPECT_EQ(Sequence[i].Tile, seq[i].Tile);
		EXPECT_EQ(Sequence[i].DelayMs, seq[i].DelayMs);
	}

	Sequence[0].Tile.Tile++;
	tilemap->AddTileSequence(tile2, Collections::Array<TileFrame>(Sequence));
	EXPECT_TRUE(tilemap->GetTileSequence(tile, seq));
	EXPECT_EQ(lengthof(Sequence), seq.size());
	for (size_t i = 0; i < lengthof(Sequence); ++i)
	{
		EXPECT_EQ(Sequence[i].Tile, seq[i].Tile);
		EXPECT_EQ(Sequence[i].DelayMs, seq[i].DelayMs);
	}

	tilemap->RemoveTileSequence(tile2);
	EXPECT_FALSE(tilemap->GetTileSequence(tile2, seq));
	EXPECT_FALSE(tilemap->GetTileSequence(tile, seq));
}
