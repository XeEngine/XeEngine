#include "pch.h"
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeGame/XeGameTiledDocument.h>

using namespace Xe;
using namespace Xe::IO;
using namespace Xe::Game::Tiled;

#define STREAM_FROM_FIXEDARRAY(a) ObjPtr<IStream>(Xe::IO::IMemoryStream::From(a, lengthof(a)))

template <class T>
T& Add(std::list<T>& list)
{
	list.push_back(T());
	return *(std::prev(list.end()));
}

template <class T>
T& Add(std::vector<T>& list)
{
	list.push_back(T());
	return *(std::prev(list.end()));
}
TEST(XeGameTiledTest, ParseTilesetTest) {

	const char json[] =
		"{"
		"\"columns\":8" ","
		"\"firstgid\":3" ","
		"\"image\":\"image.png\"" ","
		"\"imagewidth\":512" ","
		"\"imageheight\":384" ","
		"\"margin\":4" ","
		"\"name\":\"test\"" ","
		"\"source\":\"SomeFilePath\"" ","
		"\"spacing\":2" ","
		"\"tilecount\":1024" ","
		"\"tileheight\":16" ","
		"\"tilewidth\":24" ","
		"\"transparentcolor\":\"#ff00ff\"" ","
		"\"type\":\"actual\"" ","
		"\"tiledversion\":\"1.0.1\"" ","
		"\"version\":1.0"
		"}";

	Tileset tileset;
	ParseJson(tileset, STREAM_FROM_FIXEDARRAY(json));

	ASSERT_EQ(8, tileset.Columns);
	ASSERT_EQ(3, tileset.FirstGid);
	ASSERT_STRCASEEQ("image.png", tileset.Image);
	ASSERT_EQ(512, tileset.ImageWidth);
	ASSERT_EQ(384, tileset.ImageHeight);
	ASSERT_EQ(4, tileset.Margin);
	ASSERT_STRCASEEQ("test", tileset.Name);
	ASSERT_STRCASEEQ("SomeFilePath", tileset.Source);
	ASSERT_EQ(2, tileset.Spacing);
	ASSERT_EQ(1024, tileset.TileCount);
	ASSERT_EQ(16, tileset.TileHeight);
	ASSERT_EQ(24, tileset.TileWidth);
	ASSERT_STRCASEEQ("#ff00ff", tileset.TransparentColor);
	ASSERT_STRCASEEQ("actual", tileset.Type);
	ASSERT_STRCASEEQ("1.0.1", tileset.TiledVersion);
	ASSERT_EQ(1.0f, tileset.Version);
}

TEST(XeGameTiledTest, SaveTilesetTest) {

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	Tileset expected, actual;

	expected.Columns = 1;
	expected.FirstGid = 2;
	expected.Image = "test.png";
	expected.ImageWidth = 3;
	expected.ImageHeight = 4;
	expected.Margin = 5;
	expected.Name = "name";
	expected.Source = "SomeSource";
	expected.Spacing = 6;
	expected.TileCount = 7;
	expected.TileHeight = 8;
	expected.TileWidth = 9;
	expected.TransparentColor = "SomeHexCode";
	expected.Type = "SomeType";
	expected.TiledVersion = "SomeVersion";
	expected.Version = 2.0f;

	SaveJson(expected, memStream);
	memStream->SetPosition(0);
	ParseJson(actual, memStream);

	ASSERT_EQ(expected.Columns, actual.Columns);
	ASSERT_EQ(expected.FirstGid, actual.FirstGid);
	ASSERT_STRCASEEQ(expected.Image, actual.Image);
	ASSERT_EQ(expected.ImageWidth, actual.ImageWidth);
	ASSERT_EQ(expected.ImageHeight, actual.ImageHeight);
	ASSERT_EQ(expected.Margin, actual.Margin);
	ASSERT_STRCASEEQ(expected.Name, actual.Name);
	ASSERT_STRCASEEQ(expected.Source, actual.Source);
	ASSERT_EQ(expected.Spacing, actual.Spacing);
	ASSERT_EQ(expected.TileCount, actual.TileCount);
	ASSERT_EQ(expected.TileHeight, actual.TileHeight);
	ASSERT_EQ(expected.TileWidth, actual.TileWidth);
	ASSERT_STRCASEEQ(expected.TransparentColor, actual.TransparentColor);
	ASSERT_STRCASEEQ(expected.Type, actual.Type);
	ASSERT_STRCASEEQ(expected.TiledVersion, actual.TiledVersion);
	ASSERT_EQ(expected.Version, actual.Version);
}

TEST(XeGameTiledTest, SaveAndParseTilesetTile) {
	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	Tileset expectedTileset, actualTileset;

	auto& expected = Add(expectedTileset.Tiles);
	expected.Id = 123;
	expected.Probability = 2.0;
	expected.Type = "SomeTileType";
	expected.Properties[StringSpan("SomePropertyName")] = Xe::Game::Tiled::StringProperty("SomePropertyValue");
	
	auto& expectedAnim = Add(expected.Animation);
	expectedAnim.TileId = 456;
	expectedAnim.Duration = 789;

	SaveJson(expectedTileset, memStream);
	memStream->SetPosition(0);
	ParseJson(actualTileset, memStream);

	const auto& actual = *actualTileset.Tiles.begin();
	ASSERT_EQ(expected.Id, actual.Id);
	ASSERT_EQ(expected.Probability, actual.Probability);
	ASSERT_STRCASEEQ(expected.Type, actual.Type);

	const auto& actualAnim = *actual.Animation.begin();
	ASSERT_EQ(expectedAnim.TileId, actualAnim.TileId);
	ASSERT_EQ(expectedAnim.Duration, actualAnim.Duration);
}

TEST(XeGameTiledTest, SaveAndParseMapTest) {

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	Map expected, actual;

	expected.BackgroundColor = "SomeHexCode";
	expected.Height = 1;
	expected.HexSideLength = 2;
	expected.Infinite = true;
	expected.NextLayerId = 3;
	expected.NextObjectId = 4;
	expected.Orientation = "SomeOrientation";
	expected.RenderOrder = "SomeRenderOrder";
	expected.StaggerAxis = "SomeStaggerAxis";
	expected.StaggerIndex = "SomeStaggerIndex";
	expected.TileHeight = 5;
	expected.TileWidth = 6;
	expected.Type = "SomeType";
	expected.TiledVersion = "SomeTiledVersion";
	expected.Version = 2.0f;

	SaveJson(expected, memStream);
	memStream->SetPosition(0);
	ParseJson(actual, memStream);

	ASSERT_STRCASEEQ(expected.BackgroundColor, actual.BackgroundColor);
	ASSERT_EQ(expected.Height, actual.Height);
	ASSERT_EQ(expected.HexSideLength, actual.HexSideLength);
	ASSERT_EQ(expected.Infinite, actual.Infinite);
	ASSERT_EQ(expected.NextLayerId, actual.NextLayerId);
	ASSERT_EQ(expected.NextObjectId, actual.NextObjectId);
	ASSERT_STRCASEEQ(expected.Orientation, actual.Orientation);
	ASSERT_STRCASEEQ(expected.RenderOrder, actual.RenderOrder);
	ASSERT_STRCASEEQ(expected.StaggerAxis, actual.StaggerAxis);
	ASSERT_STRCASEEQ(expected.StaggerIndex, actual.StaggerIndex);
	ASSERT_EQ(expected.TileHeight, actual.TileHeight);
	ASSERT_EQ(expected.TileWidth, actual.TileWidth);
	ASSERT_STRCASEEQ(expected.Type, actual.Type);
	ASSERT_STRCASEEQ(expected.TiledVersion, actual.TiledVersion);
	ASSERT_EQ(expected.Version, actual.Version);
}

TEST(XeGameTiledTest, ParsePropertiesTest) {

	const char json[] =
		"{ \"properties\":["
			"{"
				"\"name\":\"TestBool\"" ","
				"\"type\":\"bool\"" ","
				"\"value\":true"
			"}" ","
			"{"
				"\"name\":\"TestInt\"" ","
				"\"type\":\"int\"" ","
				"\"value\":123"
			"}" ","
			"{"
				"\"name\":\"TestFloat\"" ","
				"\"type\":\"float\"" ","
				"\"value\":128.5"
			"}" ","
			"{"
				"\"name\":\"TestString\"" ","
				"\"type\":\"string\"" ","
				"\"value\":\"test string\""
			"}" ","
			"{"
				"\"name\":\"TestColor\"" ","
				"\"type\":\"color\"" ","
				"\"value\":\"#deadc0de\""
			"}" ","
			"{"
				"\"name\":\"TestFile\"" ","
				"\"type\":\"file\"" ","
				"\"value\":\"file/path/test\""
			"}" ","
			"{"
				"\"name\":\"TestUnknown\"" ","
				"\"type\":\"random\"" ","
				"\"value\":\"some random data\""
			"}"
		"]}";

	Tileset tileset;
	ParseJson(tileset, STREAM_FROM_FIXEDARRAY(json));

	{
		const auto& it = tileset.Properties.find(StringSpan("TestBool"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Bool, (*it).second.Type);
		ASSERT_EQ(true, (*it).second.b == true);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestInt"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Int, (*it).second.Type);
		ASSERT_EQ(123, (*it).second.i32);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestFloat"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Float, (*it).second.Type);
		ASSERT_EQ(128.5f, (*it).second.f);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestString"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_String, (*it).second.Type);
		ASSERT_STREQ("test string", (*it).second.Value);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestColor"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Color, (*it).second.Type);
		ASSERT_STREQ("#deadc0de", (*it).second.Value);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestFile"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_File, (*it).second.Type);
		ASSERT_STREQ("file/path/test", (*it).second.Value);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestUnknown"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Unknown, (*it).second.Type);
		ASSERT_STREQ("some random data", (*it).second.Value);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("TestNotExistent"));
		ASSERT_EQ(tileset.Properties.end(), it);
	}
}

TEST(XeGameTiledTest, SaveAndParsePropertiesTest)
{
	Tileset tileset;
	tileset.Properties[StringSpan("ThisIsABoolTrue")] = Xe::Game::Tiled::BoolProperty(true);
	tileset.Properties[StringSpan("ThisIsABoolFalse")] = Xe::Game::Tiled::BoolProperty(false);
	tileset.Properties[StringSpan("ThisIsAInt")] = Xe::Game::Tiled::IntProperty(123);
	tileset.Properties[StringSpan("ThisIsAFloat")] = Xe::Game::Tiled::FloatProperty(15.5f);
	tileset.Properties[StringSpan("ThisIsAString")] = Xe::Game::Tiled::StringProperty("string test");
	tileset.Properties[StringSpan("ThisIsAColor")] = Xe::Game::Tiled::ColorProperty("color");
	tileset.Properties[StringSpan("ThisIsAFile")] = Xe::Game::Tiled::FileProperty("some/file/path");

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	SaveJson(tileset, memStream);
	tileset.Properties.clear();

	memStream->SetPosition(0);
	ParseJson(tileset, memStream);

	ASSERT_EQ(7, tileset.Properties.size());

	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsABoolTrue"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Bool, (*it).second.Type);
		ASSERT_TRUE((*it).second.b);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsABoolFalse"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Bool, (*it).second.Type);
		ASSERT_FALSE((*it).second.b);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsAInt"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Int, (*it).second.Type);
		ASSERT_EQ(true, (*it).second.i32 == 123);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsAFloat"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Float, (*it).second.Type);
		ASSERT_EQ(true, (*it).second.f == 15.5f);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsAString"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_String, (*it).second.Type);
		ASSERT_STREQ("string test", (*it).second.Value);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsAColor"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_Color, (*it).second.Type);
		ASSERT_STREQ("color", (*it).second.Value);
	}
	{
		const auto& it = tileset.Properties.find(StringSpan("ThisIsAFile"));
		ASSERT_EQ(Xe::Game::Tiled::PropertyType_File, (*it).second.Type);
		ASSERT_STREQ("some/file/path", (*it).second.Value);
	}
}

TEST(XeGameTiledTest, SaveAndParseLayerTest)
{
	Map map, actualMap;
	auto& expected = Add<Layer>(map.Layers);
	expected.Compression = "SomeCompression";
	expected.DrawOrder = "SomeDrawOrder";
	expected.Encoding = "TotallyNotBase64";
	expected.Height = 1;
	expected.Id = 2;
	expected.Image = "CuteCat.jpg";
	expected.Name = "HorribleName";
	expected.OffsetX = 1.5;
	expected.OffsetY = 2.5;
	expected.Opacity = 2.8;
	expected.TransparentColor = "GreenIsNotCreative";
	expected.Type = "SomeStrangeType";
	expected.Visible = false;
	expected.Width = 3;
	expected.X = 4;
	expected.Y = 5;

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	SaveJson(map, memStream);

	memStream->SetPosition(0);
	ParseJson(actualMap, memStream);

	ASSERT_EQ(1, actualMap.Layers.size());
	const auto& actual = *actualMap.Layers.begin();

	ASSERT_STREQ(expected.Compression, actual.Compression);
	ASSERT_STREQ(expected.DrawOrder, actual.DrawOrder);
	ASSERT_STREQ(expected.Encoding, actual.Encoding);
	ASSERT_EQ(expected.Height, actual.Height);
	ASSERT_EQ(expected.Id, actual.Id);
	ASSERT_STREQ(expected.Image, actual.Image);
	ASSERT_STREQ(expected.Name, actual.Name);
	ASSERT_EQ(expected.OffsetX, actual.OffsetX);
	ASSERT_EQ(expected.OffsetY, actual.OffsetY);
	ASSERT_EQ(expected.Opacity, actual.Opacity);
	ASSERT_STREQ(expected.TransparentColor, actual.TransparentColor);
	ASSERT_STREQ(expected.Type, actual.Type);
	ASSERT_EQ(expected.Visible, actual.Visible);
	ASSERT_EQ(expected.Width, actual.Width);
	ASSERT_EQ(expected.X, actual.X);
	ASSERT_EQ(expected.Y, actual.Y);
}

TEST(XeGameTiledTest, ParseLayerDataTest)
{
	const char json[] =
		"{ \"layers\":["
		"{"
		"\"width\": 4" ","
		"\"height\": 4" ","
		"\"data\": [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]"
		"}"
		"]}";

	Map map;
	ParseJson(map, STREAM_FROM_FIXEDARRAY(json));

	const auto& layer = *map.Layers.begin();
	ASSERT_EQ(16, layer.Data.size());
	for (auto i = 0; i < 16; ++i)
	{
		ASSERT_EQ(i, layer.Data[i]);
	}
}

TEST(XeGameTiledTest, SaveAndParseLayerDataTest)
{
	Map map, actualMap;
	auto& layer = Add<Layer>(map.Layers);
	layer.Width = 32;
	layer.Height = 16;

	layer.Data.resize(layer.Width * layer.Height);
	for (int i = 0; i < layer.Width * layer.Height; i++)
	{
		layer.Data[i] = i;
	}

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	SaveJson(map, memStream);

	memStream->SetPosition(0);
	ParseJson(actualMap, memStream);

	const auto& actualLayer = *actualMap.Layers.begin();
	ASSERT_EQ(layer.Width * layer.Height, actualLayer.Data.size());
	for (int i = 0; i < layer.Width * layer.Height; i++)
	{
		ASSERT_EQ(i, actualLayer.Data[i]);
	}
}

TEST(XeGameTiledTest, ParseLayerDataBase64Test)
{
	const char json[] =
		"{ \"layers\":["
		"{"
		"\"width\": 4" ","
		"\"height\": 4" ","
		"\"encoding\": \"base64\"" ","
		"\"data\": \"AAAAAAEAAAACAAAAAwAAAAQAAAAFAAAABgAAAAcAAAAIAAAACQAAAAoAAAALAAAADAAAAA0AAAAOAAAADwAAAA==\""
		"}"
		"]}";

	Map map;
	ParseJson(map, STREAM_FROM_FIXEDARRAY(json));

	const auto& layer = *map.Layers.begin();
	ASSERT_EQ(16, layer.Data.size());
	for (auto i = 0; i < 16; ++i)
	{
		ASSERT_EQ(i, layer.Data[i]);
	}
}

TEST(XeGameTiledTest, SaveAndParseLayerDataBase64Test)
{
	Map map, actualMap;
	auto& layer = Add<Layer>(map.Layers);
	layer.Width = 32;
	layer.Height = 16;
	layer.Encoding = "base64";

	layer.Data.resize(layer.Width * layer.Height);
	for (int i = 0; i < layer.Width * layer.Height; i++)
	{
		layer.Data[i] = i;
	}

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	SaveJson(map, memStream);

	memStream->SetPosition(0);
	ParseJson(actualMap, memStream);

	const auto& actualLayer = *actualMap.Layers.begin();
	ASSERT_EQ(layer.Width * layer.Height, actualLayer.Data.size());
	for (int i = 0; i < layer.Width * layer.Height; i++)
	{
		ASSERT_EQ(i, actualLayer.Data[i]);
	}
}

TEST(XeGameTiledTest, FlippedAndRotatedTiles)
{
	Xe::Game::Tiled::Gid data[8];
	data[0] = 2;
	data[1] = 2147483650;
	data[2] = 1073741826;
	data[3] = 3221225474;
	data[4] = 536870914;
	data[5] = 2684354562;
	data[6] = 3221225474;
	data[7] = 1610612738;

	ASSERT_EQ(2, data[0].Tile);
	ASSERT_EQ(0, data[0].Mirror);
	ASSERT_EQ(0, data[0].Flip);
	ASSERT_EQ(0, data[0].Rotate);

	ASSERT_EQ(2, data[1].Tile);
	ASSERT_EQ(1, data[1].Mirror);
	ASSERT_EQ(0, data[1].Flip);
	ASSERT_EQ(0, data[1].Rotate);

	ASSERT_EQ(2, data[2].Tile);
	ASSERT_EQ(0, data[2].Mirror);
	ASSERT_EQ(1, data[2].Flip);
	ASSERT_EQ(0, data[2].Rotate);

	ASSERT_EQ(2, data[3].Tile);
	ASSERT_EQ(1, data[3].Mirror);
	ASSERT_EQ(1, data[3].Flip);
	ASSERT_EQ(0, data[3].Rotate);

	ASSERT_EQ(2, data[4].Tile);
	ASSERT_EQ(0, data[4].Mirror);
	ASSERT_EQ(0, data[4].Flip);
	ASSERT_EQ(1, data[4].Rotate);

	ASSERT_EQ(2, data[5].Tile);
	ASSERT_EQ(1, data[5].Mirror);
	ASSERT_EQ(0, data[5].Flip);
	ASSERT_EQ(1, data[5].Rotate);

	ASSERT_EQ(2, data[6].Tile);
	ASSERT_EQ(1, data[6].Mirror);
	ASSERT_EQ(1, data[6].Flip);
	ASSERT_EQ(0, data[6].Rotate);

	ASSERT_EQ(2, data[7].Tile);
	ASSERT_EQ(0, data[7].Mirror);
	ASSERT_EQ(1, data[7].Flip);
	ASSERT_EQ(1, data[7].Rotate);
}

TEST(XeGameTiledTest, SaveAndParseObject)
{
	Map map, actualMap;
	auto& layer = Add<Layer>(map.Layers);
	auto& expected = Add<Object>(layer.Objects);

	expected.Gid = 1;
	expected.Id = 2;
	expected.X = 1.5f;
	expected.Y = 2.5f;
	expected.Width = 3.5f;
	expected.Height = 4.5f;
	expected.Rotation = 2.8f;
	expected.Type = "SomeType";
	expected.Name = "SomeName";
	expected.Template = "SomeTemplate";
	expected.Visible = false;
	expected.Ellipse = true;
	expected.Polygon.push_back(ObjectPosition(1.0, 2.0));
	expected.Polygon.push_back(ObjectPosition(3.0, 4.0));
	expected.Polyline.push_back(ObjectPosition(5.0, 6.0));
	expected.Polyline.push_back(ObjectPosition(7.0, 8.0));

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	SaveJson(map, memStream);

	memStream->SetPosition(0);
	ParseJson(actualMap, memStream);

	auto& actualLayer = *actualMap.Layers.begin();
	ASSERT_EQ(1, actualLayer.Objects.size());

	auto& actual = *actualLayer.Objects.begin();
	ASSERT_EQ(expected.Gid, actual.Gid);
	ASSERT_EQ(expected.Id, actual.Id);
	ASSERT_EQ(expected.X, actual.X);
	ASSERT_EQ(expected.Y, actual.Y);
	ASSERT_EQ(expected.Width, actual.Width);
	ASSERT_EQ(expected.Height, actual.Height);
	ASSERT_EQ(expected.Rotation, actual.Rotation);
	ASSERT_EQ(expected.Visible, actual.Visible);
	ASSERT_EQ(expected.Ellipse, expected.Ellipse);
	ASSERT_STREQ(expected.Type, actual.Type);
	ASSERT_STREQ(expected.Name, actual.Name);
	ASSERT_STREQ(expected.Template, actual.Template);

	ASSERT_EQ(expected.Polygon.size(), actual.Polygon.size());
	for (auto itE = expected.Polygon.begin(), itA = expected.Polygon.begin();
		itE != expected.Polygon.end(); ++itE, ++itA)
	{
		ASSERT_EQ((*itE).X, (*itA).X);
		ASSERT_EQ((*itE).Y, (*itA).Y);
	}

	ASSERT_EQ(expected.Polyline.size(), actual.Polyline.size());
	for (auto itE = expected.Polyline.begin(), itA = expected.Polyline.begin();
		itE != expected.Polyline.end(); ++itE, ++itA)
	{
		ASSERT_EQ((*itE).X, (*itA).X);
		ASSERT_EQ((*itE).Y, (*itA).Y);
	}
}
