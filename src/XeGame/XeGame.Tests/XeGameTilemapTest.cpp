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
	ITilemap2d* pTilemap;
	Vector2i cameraSize(123, 456);
	Vector2i cameraPos(222, 444);
	TilemapBufferSize bufferSize;

	Factory(&pTilemap, nullptr);
	ASSERT_NE(nullptr, pTilemap);

	pTilemap->SetCameraSize(cameraSize);
	pTilemap->SetCameraPosition(cameraPos);
	pTilemap->SetBufferSize(bufferSize);

	ASSERT_EQ(cameraSize, pTilemap->GetCameraSize());
	ASSERT_EQ(cameraPos, pTilemap->GetCameraPosition());
	ASSERT_EQ(bufferSize, pTilemap->GetBufferSize());
}

TEST(XeGameTilemapTest, DontPerformCallbackTest)
{
	const int ExpectedSizeX = 123;
	const int ExpectedSizeY = 456;
	const int ExpectedStride = 789;
	Xe::Game::TileData* const ExpectedTilemap = (Xe::Game::TileData*)0xDEADB33F;

	ITilemap2d* pTilemap;
	Factory(&pTilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	pTilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;
	args.Destination.Size.x = ExpectedSizeX;
	args.Destination.Size.y = ExpectedSizeY;
	args.Destination.Stride = ExpectedStride;
	args.Destination.Tilemap = ExpectedTilemap;
	pTilemap->Flush();

	ASSERT_EQ(Vector2i(0, 0), pTilemap->GetCameraSize());
	ASSERT_EQ(ExpectedSizeX, args.Destination.Size.x);
	ASSERT_EQ(ExpectedSizeY, args.Destination.Size.y);
	ASSERT_EQ(ExpectedStride, args.Destination.Stride);
	ASSERT_EQ(ExpectedTilemap, args.Destination.Tilemap);
}

TEST(XeGameTilemapTest, PerformRequestTilesCallbackBasicTest)
{
	const int CameraSizeX = 4;
	const int CameraSizeY = 2;
	const int UnexpectedStride = 789;
	Xe::Game::TileData* const UnexpectedTilemap = (Xe::Game::TileData*)0xDEADB33F;

	ITilemap2d* pTilemap;
	Factory(&pTilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	pTilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;
	args.Destination.Size.x = 111111;
	args.Destination.Size.y = 222222;
	args.Destination.Stride = UnexpectedStride;
	args.Destination.Tilemap = UnexpectedTilemap;

	pTilemap->SetTileSize({ TilemapTile_16, TilemapTile_16 });
	pTilemap->SetBufferSize(TilemapBufferSize(TilemapBuffer_32, TilemapBuffer_16));
	pTilemap->SetCameraSize(Math::Vector2i(CameraSizeX, CameraSizeY));

	TilemapData layer;
	layer.Stride = 0;
	ASSERT_TRUE(pTilemap->GetBuffer(&layer));
	ASSERT_NE(nullptr, layer.Tilemap);
	ASSERT_NE(0, layer.Stride);
	ASSERT_NE(0, layer.Size.x);
	ASSERT_NE(0, layer.Size.y);
	Memory::Fill(layer.Tilemap, 0xCC, layer.Stride * layer.Size.y);

	pTilemap->Flush();

	ASSERT_EQ(CameraSizeX, args.Destination.Size.x);
	ASSERT_EQ(CameraSizeY, args.Destination.Size.y);
	ASSERT_NE(UnexpectedStride, args.Destination.Stride);
	ASSERT_NE(UnexpectedTilemap, args.Destination.Tilemap);
	ASSERT_NE(nullptr, args.Destination.Tilemap);
	ASSERT_EQ(CameraSizeX * CameraSizeY, delegateTest.tilesWritten);

	ASSERT_TRUE(pTilemap->GetBuffer(&layer));
	for (int x = 0; x < CameraSizeX; x++)
	{
		for (int y = 0; y < CameraSizeY; y++)
		{
			auto pTile = (MetaTile*)&layer.Tilemap[x + y * layer.Size.x];
			ASSERT_EQ(x, pTile->X);
			ASSERT_EQ(y, pTile->Y);
		}
	}
}

TEST(XeGameTilemapTest, CheckBoundariesOnRequestTilesCallbackTest)
{
	const int CameraSizeX = 99;
	const int CameraSizeY = 99;

	ITilemap2d* pTilemap;
	Factory(&pTilemap, nullptr);

	TilemapRequestTilesDelegateTest delegateTest;
	pTilemap->SetRequestTilesCallback(&delegateTest);

	auto& args = delegateTest.args;

	pTilemap->SetTileSize({ TilemapTile_16, TilemapTile_16 });
	pTilemap->SetBufferSize(TilemapBufferSize(TilemapBuffer_32, TilemapBuffer_16));
	pTilemap->SetCameraSize(Math::Vector2i(CameraSizeX, CameraSizeY));

	pTilemap->Flush();

	ASSERT_EQ(32, args.Destination.Size.x);
	ASSERT_EQ(16, args.Destination.Size.y);
	ASSERT_EQ(32 * sizeof(TileData), args.Destination.Stride);
}