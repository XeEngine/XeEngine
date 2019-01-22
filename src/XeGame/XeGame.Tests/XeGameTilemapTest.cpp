#include "pch.h"
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeGame/XeGameTiledDocument.h>
#include <XeGame/XeGameTilemap2d.h>
#include <XeGame/IGameTilemap2d.h>
#include <XeGame/IGameTilemapLayer.h>

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
		auto tilemapOffset = (u8*)dst.Data;
		auto stride = args.Arguments->Destination.Stride;

		tilesWritten = 0;
		for (size_t x = 0; x < args.Arguments->Destination.Size.x; x++)
		{
			for (size_t y = 0; y < args.Arguments->Destination.Size.y; y++)
			{
				MetaTile tile;
				tile.X = (u16)x;
				tile.Y = (u16)y;
				dst.Tile(x, y) = TileData{ tile.Data };
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
	Vector2f cameraPos(222, 444);

	Factory(&tilemap, nullptr);
	EXPECT_NE(nullptr, tilemap.Get());

	tilemap->SetCameraSize(cameraSize);

	EXPECT_EQ(cameraSize, tilemap->GetCameraSize());
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
	args.Destination.Data = ExpectedTilemap;
	tilemap->Flush();

	EXPECT_EQ(Vector2i(0, 0), tilemap->GetCameraSize());
	EXPECT_EQ(ExpectedSizeX, args.Destination.Size.x);
	EXPECT_EQ(ExpectedSizeY, args.Destination.Size.y);
	EXPECT_EQ(ExpectedStride, args.Destination.Stride);
	EXPECT_EQ(ExpectedTilemap, args.Destination.Data);
}

TEST(XeGameTilemapTest, CheckBoundariesOnRequestTilesCallbackTest)
{
	const int CameraSizeX = 100;
	const int CameraSizeY = 80;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	tilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;

	tilemap->SetLayersCount(1);
	tilemap->SetTileSize({ 16, 16 });
	tilemap->GetLayer(0)->SetBufferSize({ 32, 16 });
	tilemap->SetCameraSize(Math::Vector2i(CameraSizeX, CameraSizeY));

	tilemap->Flush();

	EXPECT_EQ(7, args.Destination.Size.x);
	EXPECT_EQ(6, args.Destination.Size.y);
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

TEST(XeGameTilemapTest, TilemapSetLayersCountTest)
{
	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);

	tilemap->SetLayersCount(1);
	EXPECT_EQ(1, tilemap->GetLayerCount());

	auto layer = tilemap->GetLayer(0);
	EXPECT_NE(nullptr, layer.Get());
	EXPECT_NE(2, layer->i_count);

	tilemap->SetLayersCount(0);
	EXPECT_NE(1, layer->i_count);
	EXPECT_THROW(tilemap->GetLayer(0), std::invalid_argument);

	tilemap->SetLayersCount(1);
	EXPECT_NE(layer.Get(), tilemap->GetLayer(0).Get());
}

TEST(XeGameTilemapTest, TilemapLayerPropertiesTest)
{
	const auto expectedName = "LayerNameTest";
	const Vector2u expectedBufferSize = { 4, 8 };
	const Vector2f expectedPosition = { -123.f, 456.08f };
	const Vector2f expectedMultiplier = { 2.f, -4.5f };
	const TexId expectedTexId = (TexId)0x1234;
	const ClutId expectedClutId = (ClutId)0x5678;
	const size_t expectedTilesPerRow = 88;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);
	tilemap->SetLayersCount(1);
	auto layer = tilemap->GetLayer(0);

	EXPECT_STRCASENE(expectedName, layer->GetName());
	layer->SetName(expectedName);
	EXPECT_STRCASEEQ(expectedName, layer->GetName());

	EXPECT_NE(0, layer->GetBufferSize().x);
	EXPECT_NE(0, layer->GetBufferSize().y);
	layer->SetBufferSize(expectedBufferSize);
	EXPECT_EQ(expectedBufferSize.x, layer->GetBufferSize().x);
	EXPECT_EQ(expectedBufferSize.y, layer->GetBufferSize().y);
	EXPECT_THROW(layer->SetBufferSize({ 0, 0 }), std::invalid_argument);
	EXPECT_THROW(layer->SetBufferSize({ 0, 1 }), std::invalid_argument);
	EXPECT_THROW(layer->SetBufferSize({ 1, 0 }), std::invalid_argument);

	EXPECT_EQ(0.0f, layer->GetScrollOffset().x);
	EXPECT_EQ(0.0f, layer->GetScrollOffset().y);
	layer->SetScrollOffset(expectedPosition);
	EXPECT_EQ(expectedPosition.x, layer->GetScrollOffset().x);
	EXPECT_EQ(expectedPosition.y, layer->GetScrollOffset().y);

	EXPECT_EQ(1.0f, layer->GetScrollMultiplier().x);
	EXPECT_EQ(1.0f, layer->GetScrollMultiplier().y);
	layer->SetScrollMultiplier(expectedMultiplier);
	EXPECT_EQ(expectedMultiplier.x, layer->GetScrollMultiplier().x);
	EXPECT_EQ(expectedMultiplier.y, layer->GetScrollMultiplier().y);

	EXPECT_FALSE(layer->IsVisible());
	layer->SetVisible(true);
	EXPECT_TRUE(layer->IsVisible());
	layer->SetVisible(false);
	EXPECT_FALSE(layer->IsVisible());

	EXPECT_EQ(Xe::Game::TexInvalid, layer->GetTexture());
	EXPECT_NE(0, layer->GetTilesPerRow());
	layer->SetTexture(expectedTexId, expectedTilesPerRow);
	EXPECT_EQ(expectedTexId, layer->GetTexture());
	EXPECT_EQ(expectedTilesPerRow, layer->GetTilesPerRow());

	EXPECT_EQ(Xe::Game::ClutInvalid, layer->GetPalette());
	layer->SetPalette(expectedClutId);
	EXPECT_EQ(expectedClutId, layer->GetPalette());
}

TEST(XeGameTilemapTest, TilemapLayerLockExceptionTest)
{
	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);
	tilemap->SetLayersCount(1);
	auto layer = tilemap->GetLayer(0);

	Xe::Game::TilemapData tilemapData;
	EXPECT_THROW(layer->Unlock(), std::logic_error);
	EXPECT_TRUE(layer->Lock(tilemapData, Xe::Game::Lock_Read));
	EXPECT_THROW(layer->Lock(tilemapData, Xe::Game::Lock_Read), std::logic_error);
	layer->Unlock();
	EXPECT_THROW(layer->Unlock(), std::logic_error);
}

TEST(XeGameTilemapTest, TilemapLayerLockReadTest)
{
	TileData expectedTile;

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);
	tilemap->SetLayersCount(1);
	auto layer = tilemap->GetLayer(0);

	Xe::Game::TilemapData tilemapData;
	EXPECT_TRUE(layer->Lock(tilemapData, Xe::Game::Lock_Read));
	expectedTile = tilemapData.Tile(0, 0);
	tilemapData.Tile(0, 0) = { expectedTile + 1 };
	layer->Unlock();

	EXPECT_TRUE(layer->Lock(tilemapData, Xe::Game::Lock_Read));
	EXPECT_EQ(expectedTile, tilemapData.Tile(0, 0));
	layer->Unlock();
}

TEST(XeGameTilemapTest, TilemapLayerLockWriteTest)
{
	const TileData expectedTile = { 123 };
	const TileData expectedTile2 = { expectedTile + 1 };

	ObjPtr<ITilemap2d> tilemap;
	Factory(&tilemap, nullptr);
	tilemap->SetLayersCount(1);
	auto layer = tilemap->GetLayer(0);

	Xe::Game::TilemapData tilemapData;
	EXPECT_TRUE(layer->Lock(tilemapData, Xe::Game::Lock_Write));
	tilemapData.Tile(0, 0) = { expectedTile };
	layer->Unlock();

	EXPECT_TRUE(layer->Lock(tilemapData, Xe::Game::Lock_ReadWrite));
	EXPECT_EQ(expectedTile, tilemapData.Tile(0, 0));
	tilemapData.Tile(0, 0) = expectedTile2;
	layer->Unlock();

	EXPECT_TRUE(layer->Lock(tilemapData, Xe::Game::Lock_Read));
	EXPECT_EQ(expectedTile2, tilemapData.Tile(0, 0));
	layer->Unlock();
}