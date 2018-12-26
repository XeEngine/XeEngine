#include "pch.h"
#include <XeGame/XeGameTiledDocument.h>
#include <XeSDK/XeJson.h>
#include <XeSDK/XeConvert.h>

using namespace Xe;
using namespace Xe::Game;

void Internal_WriteToStream(IO::IStream* pStream, const rapidjson::Value& value)
{
	rapidjson::XeWriteStream stream(pStream);
	rapidjson::PrettyWriter<rapidjson::XeWriteStream> writer(stream);
	writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
	value.Accept(writer);
}

void Internal_Parse(Tiled::Tileset& obj, const rapidjson::Value& value);
void Internal_Save(const Tiled::Tileset& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator);
void Internal_Parse(Tiled::Layer& obj, const rapidjson::Value& value);
void Internal_Save(const Tiled::Layer& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator);
void Internal_Parse(Tiled::Object& obj, const rapidjson::Value& value);
void Internal_Save(const Tiled::Object& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator);
void Internal_Parse(Tiled::ObjectPosition& obj, const rapidjson::Value& value);
void Internal_Save(const Tiled::ObjectPosition& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator);

void Internal_ParseProperties(Tiled::GenericProperties& obj, const rapidjson::Value& value)
{
	if (value.HasMember("properties"))
	{
		const auto& properties = value["properties"].GetArray();
		for (rapidjson::SizeType i = 0; i < properties.Size(); ++i)
		{
			const auto& property = properties[i];

			if (property.HasMember("name"))
			{
				const auto& name = property["name"].GetString();
				Tiled::GenericProperty value;

				const auto type = StringSpan(JSON_TRYGET(property, "type", GetString(), ""));
				if (type == "bool")
				{
					value.Type = Tiled::PropertyType_Bool;
					value.b = JSON_TRYGET(property, "value", GetBool(), false);
				}
				else if (type == "int")
				{
					value.Type = Tiled::PropertyType_Int;
					value.i32 = JSON_TRYGET(property, "value", GetInt(), false);
				}
				else if (type == "float")
				{
					value.Type = Tiled::PropertyType_Float;
					value.f = JSON_TRYGET(property, "value", GetFloat(), false);
				}
				else if (type == "string")
				{
					value.Type = Tiled::PropertyType_String;
					value.Value = JSON_TRYGET(property, "value", GetString(), false);
				}
				else if (type == "color")
				{
					value.Type = Tiled::PropertyType_Color;
					value.Value = JSON_TRYGET(property, "value", GetString(), false);
				}
				else if (type == "file")
				{
					value.Type = Tiled::PropertyType_File;
					value.Value = JSON_TRYGET(property, "value", GetString(), false);
				}
				else
				{
					value.Type = Tiled::PropertyType_Unknown;
					value.Value = JSON_TRYGET(property, "value", GetString(), false);
				}

				obj.Properties[StringSpan(name)] = value;
			};
		}
	};
}

void Internal_SaveProperties(const Tiled::GenericProperties& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
{
	if (obj.Properties.size() <= 0)
		return;

	rapidjson::Value propertiesValue;
	propertiesValue.SetArray();

	for (const auto& property : obj.Properties)
	{
		rapidjson::Value propertyValue;
		propertyValue.SetObject();
		
		JSON_STRADD(propertyValue, "name", property.first);
		switch (property.second.Type)
		{
		case Tiled::PropertyType_Bool:
			JSON_STRADD(propertyValue, "type", "bool");
			JSON_ADD(propertyValue, "value", property.second.b);
			break;
		case Tiled::PropertyType_Int:
			JSON_STRADD(propertyValue, "type", "int");
			JSON_ADD(propertyValue, "value", property.second.i32);
			break;
		case Tiled::PropertyType_Float:
			JSON_STRADD(propertyValue, "type", "float");
			JSON_ADD(propertyValue, "value", property.second.f);
			break;
		case Tiled::PropertyType_String:
			JSON_STRADD(propertyValue, "type", "string");
			JSON_STRADD(propertyValue, "value", property.second.Value);
			break;
		case Tiled::PropertyType_Color:
			JSON_STRADD(propertyValue, "type", "color");
			JSON_STRADD(propertyValue, "value", property.second.Value);
			break;
		case Tiled::PropertyType_File:
			JSON_STRADD(propertyValue, "type", "file");
			JSON_STRADD(propertyValue, "value", property.second.Value);
			break;
		default:
			JSON_STRADD(propertyValue, "type", "string");
			JSON_STRADD(propertyValue, "value", property.second.Value);
			break;
		}

		propertiesValue.PushBack(propertyValue, allocator);
	}

	JSON_ADD(value, "properties", propertiesValue);
}

template <class T>
void Internal_ParseList(std::list<T>& list, const rapidjson::Value& value, const char* memberName)
{
	if (value.HasMember(memberName))
	{
		const auto& items = value[memberName].GetArray();
		list.clear();

		for (rapidjson::SizeType i = 0; i < items.Size(); ++i)
		{
			list.push_back(T());
			auto& item = *std::prev(list.end());
			Internal_Parse(item, items[i]);
		}
	}
}

template <class T>
void Internal_SaveList(const std::list<T>& items, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator, const char* memberName)
{
	if (items.size() > 0)
	{
		rapidjson::Value jsonArray;
		jsonArray.SetArray();

		for (const auto& item : items)
		{
			rapidjson::Value jsonItem;
			jsonItem.SetObject();

			Internal_Save(item, jsonItem, allocator);
			jsonArray.PushBack(jsonItem, allocator);
		}

		value.AddMember(rapidjson::Value::StringRefType::GenericStringRef(memberName), jsonArray, allocator);
	}
}

void Internal_ReadLayerDataRaw(std::vector<Tiled::Gid>& vector, const rapidjson::Value& jsonData)
{
	if (!jsonData.IsArray())
		return;

	const auto& array = jsonData.GetArray();
	for (rapidjson::SizeType i = 0; i < array.Size(); i++)
	{
		vector[i] = array[i].GetUint();
	}
}

void Internal_WriteLayerDataRaw(const std::vector<Tiled::Gid>& vector, rapidjson::Value& jsonData, rapidjson::MemoryPoolAllocator<>& allocator)
{
	jsonData.SetArray();
	for (auto& c : vector)
	{
		jsonData.PushBack(c, allocator);
	}
}

void Internal_ReadLayerDataBase64(std::vector<Tiled::Gid>& vector, const rapidjson::Value& jsonData)
{
	if (!jsonData.IsString())
		return;

	ObjPtr<IO::IMemoryStream> stream = IO::IMemoryStream::From(vector.data(), vector.size() * sizeof(Tiled::Gid), true);
	Convert::FromBase64String(stream, jsonData.GetString());
}

void Internal_WriteLayerDataBase64(const std::vector<Tiled::Gid>& vector, rapidjson::Value& jsonData, rapidjson::MemoryPoolAllocator<>& allocator)
{
	ObjPtr<IO::IMemoryStream> outStream = IO::IMemoryStream::New(0, true);
	ObjPtr<IO::IMemoryStream> inStream = IO::IMemoryStream::From(vector.data(), vector.size() * sizeof(Tiled::Gid), true);

	auto length = Convert::ToBase64String(outStream, inStream, vector.size() * sizeof(Tiled::Gid));
	jsonData.SetString((const char*)outStream->GetMemory(), length, allocator);
}

void Internal_ReadLayerDataBase64Gzip(std::vector<Tiled::Gid>& vector, const rapidjson::Value& jsonData)
{
	if (!jsonData.IsString())
		return;

	// TODO not implemented yet
}

void Internal_ReadLayerDataBase64Zlib(std::vector<Tiled::Gid>& vector, const rapidjson::Value& jsonData)
{
	if (!jsonData.IsString())
		return;

	// TODO not implemented yet
}

void Internal_Parse(Tiled::Tileset& obj, const rapidjson::Value& value)
{
	JSON_GET(value, "columns", GetInt(), obj.Columns);
	JSON_GET(value, "firstgid", GetInt(), obj.FirstGid);
	JSON_GET(value, "image", GetString(), obj.Image);
	JSON_GET(value, "imagewidth", GetInt(), obj.ImageWidth);
	JSON_GET(value, "imageheight", GetInt(), obj.ImageHeight);
	JSON_GET(value, "margin", GetInt(), obj.Margin);
	JSON_GET(value, "name", GetString(), obj.Name);
	JSON_GET(value, "source", GetString(), obj.Source);
	JSON_GET(value, "spacing", GetInt(), obj.Spacing);
	JSON_GET(value, "tilecount", GetInt(), obj.TileCount);
	JSON_GET(value, "tileheight", GetInt(), obj.TileHeight);
	JSON_GET(value, "tilewidth", GetInt(), obj.TileWidth);
	JSON_GET(value, "transparentcolor", GetString(), obj.TransparentColor);
	JSON_GET(value, "type", GetString(), obj.Type);
	JSON_GET(value, "tiledversion", GetString(), obj.TiledVersion);
	JSON_GET(value, "version", GetFloat(), obj.Version);

	Internal_ParseProperties(obj, value);
}

void Internal_Save(const Tiled::Tileset& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
{
	JSON_ADD(value, "columns", obj.Columns);
	JSON_ADD(value, "firstgid", obj.FirstGid);
	JSON_STRADD(value, "image", obj.Image);
	JSON_ADD(value, "imagewidth", obj.ImageWidth);
	JSON_ADD(value, "imageheight", obj.ImageHeight);
	JSON_ADDIF(value, "margin", obj.Margin, 0);
	JSON_STRADD(value, "name", obj.Name);
	JSON_STRADD(value, "source", obj.Source);
	JSON_ADDIF(value, "spacing", obj.Spacing, 0);
	JSON_ADD(value, "tilecount", obj.TileCount);
	JSON_ADD(value, "tileheight", obj.TileHeight);
	JSON_ADD(value, "tilewidth", obj.TileWidth);
	JSON_STRADD(value, "transparentcolor", obj.TransparentColor);
	JSON_STRADD(value, "type", obj.Type);
	JSON_STRADD(value, "tiledversion", obj.TiledVersion);
	JSON_ADD(value, "version", obj.Version);

	Internal_SaveProperties(obj, value, allocator);
}

void Internal_Parse(Tiled::Layer& obj, const rapidjson::Value& value)
{
	JSON_GET(value, "compression", GetString(), obj.Compression);
	JSON_GET(value, "draworder", GetString(), obj.DrawOrder);
	JSON_GET(value, "encoding", GetString(), obj.Encoding);
	JSON_GET(value, "height", GetInt(), obj.Height);
	JSON_GET(value, "id", GetInt(), obj.Id);
	JSON_GET(value, "image", GetString(), obj.Image);
	JSON_GET(value, "name", GetString(), obj.Name);
	JSON_GET(value, "offsetx", GetDouble(), obj.OffsetX);
	JSON_GET(value, "offsety", GetDouble(), obj.OffsetY);
	JSON_GET(value, "opacity", GetFloat(), obj.Opacity);
	JSON_GET(value, "transparentcolor", GetString(), obj.TransparentColor);
	JSON_GET(value, "type", GetString(), obj.Type);
	JSON_GET(value, "visible", GetBool(), obj.Visible);
	JSON_GET(value, "width", GetInt(), obj.Width);
	JSON_GET(value, "x", GetInt(), obj.X);
	JSON_GET(value, "y", GetInt(), obj.Y);

	if (value.HasMember("data"))
	{
		obj.Data.resize(obj.Width * obj.Height);
		if (obj.Encoding == StringSpan("base64"))
		{
			if (obj.Compression == StringSpan("gzip"))
			{
				Internal_ReadLayerDataBase64Gzip(obj.Data, value["data"]);
			}
			else if (obj.Compression == StringSpan("zlib"))
			{
				Internal_ReadLayerDataBase64Zlib(obj.Data, value["data"]);
			}
			else
			{
				Internal_ReadLayerDataBase64(obj.Data, value["data"]);
			}
		}
		else
		{
			Internal_ReadLayerDataRaw(obj.Data, value["data"]);
		}
	}

	Internal_ParseList(obj.Objects, value, "objects");
	Internal_ParseList(obj.Layers, value, "layers");
	Internal_ParseProperties(obj, value);
}

void Internal_Save(const Tiled::Layer& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
{
	JSON_ADD(value, "id", obj.Id);
	JSON_STRADD(value, "compression", obj.Compression);
	JSON_STRADD(value, "draworder", obj.DrawOrder);
	JSON_STRADD(value, "encoding", obj.Encoding);
	JSON_ADDIF(value, "height", obj.Height, 0);
	JSON_STRADD(value, "image", obj.Image);
	JSON_STRADD(value, "name", obj.Name);
	JSON_ADDIF(value, "offsetx", obj.OffsetX, 0.0);
	JSON_ADDIF(value, "offsety", obj.OffsetY, 0.0);
	JSON_ADD(value, "opacity", obj.Opacity);
	JSON_STRADD(value, "transparentcolor", obj.TransparentColor);
	JSON_STRADD(value, "type", obj.Type);
	JSON_ADD(value, "visible", obj.Visible);
	JSON_ADDIF(value, "width", obj.Width, 0);
	JSON_ADD(value, "x", obj.X);
	JSON_ADD(value, "y", obj.Y);

	rapidjson::Value jsonData;
	if (obj.Encoding == StringSpan("base64"))
	{
		if (obj.Compression == StringSpan("gzip"))
		{
			//Internal_ReadLayerDataBase64Gzip(obj.Data, value["data"]);
		}
		else if (obj.Compression == StringSpan("zlib"))
		{
			//Internal_ReadLayerDataBase64Zlib(obj.Data, value["data"]);
		}
		else
		{
			Internal_WriteLayerDataBase64(obj.Data, jsonData, allocator);
		}
	}
	else
	{
		Internal_WriteLayerDataRaw(obj.Data, jsonData, allocator);
	}
	JSON_ADD(value, "data", jsonData);

	Internal_SaveList(obj.Objects, value, allocator, "objects");
	Internal_SaveList(obj.Layers, value, allocator, "layers");
	Internal_SaveProperties(obj, value, allocator);
}

void Internal_Parse(Tiled::ObjectPosition& obj, const rapidjson::Value& value)
{
	JSON_GET(value, "x", GetDouble(), obj.X);
	JSON_GET(value, "y", GetDouble(), obj.Y);
}

void Internal_Save(const Tiled::ObjectPosition& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
{
	JSON_ADDIF(value, "x", obj.X, 0);
	JSON_ADDIF(value, "y", obj.Y, 0);
}

void Internal_Parse(Tiled::Object& obj, const rapidjson::Value& value)
{
	JSON_GET(value, "gid", GetInt(), obj.Gid);
	JSON_GET(value, "id", GetInt(), obj.Id);
	JSON_GET(value, "x", GetDouble(), obj.X);
	JSON_GET(value, "y", GetDouble(), obj.Y);
	JSON_GET(value, "width", GetDouble(), obj.Width);
	JSON_GET(value, "height", GetDouble(), obj.Height);
	JSON_GET(value, "rotation", GetDouble(), obj.Rotation);
	JSON_GET(value, "type", GetString(), obj.Type);
	JSON_GET(value, "name", GetString(), obj.Name);
	JSON_GET(value, "template", GetString(), obj.Template);
	JSON_GET(value, "visible", GetBool(), obj.Visible);
	JSON_GET(value, "point", GetBool(), obj.Point);
	JSON_GET(value, "ellipse", GetBool(), obj.Ellipse);

	Internal_ParseList(obj.Polygon, value, "polygon");
	Internal_ParseList(obj.Polyline, value, "polyline");
}

void Internal_Save(const Tiled::Object& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
{
	JSON_ADDIF(value, "gid", obj.Gid, 0);
	JSON_ADD(value, "id", obj.Id, 0);
	JSON_ADD(value, "x", obj.X);
	JSON_ADD(value, "y", obj.Y);
	JSON_ADD(value, "width", obj.Width);
	JSON_ADD(value, "height", obj.Height);
	JSON_ADD(value, "rotation", obj.Rotation);
	JSON_STRADD(value, "type", obj.Type);
	JSON_STRADD(value, "name", obj.Name);
	JSON_STRADD(value, "template", obj.Template);
	JSON_ADD(value, "visible", obj.Visible);
	JSON_ADDIF(value, "point", obj.Point, false);
	JSON_ADDIF(value, "ellipse", obj.Ellipse, false);

	Internal_SaveList(obj.Polygon, value, allocator, "polygon");
	Internal_SaveList(obj.Polyline, value, allocator, "polyline");
}

void Internal_Parse(Tiled::Map& obj, const rapidjson::Value& value)
{
	JSON_GET(value, "backgroundcolor", GetString(), obj.BackgroundColor);
	JSON_GET(value, "height", GetInt(), obj.Height);
	JSON_GET(value, "hexsidelength", GetInt(), obj.HexSideLength);
	JSON_GET(value, "infinite", GetBool(), obj.Infinite);
	JSON_GET(value, "nextlayerid", GetInt(), obj.NextLayerId);
	JSON_GET(value, "nextobjectid", GetInt(), obj.NextObjectId);
	JSON_GET(value, "orientation", GetString(), obj.Orientation);
	JSON_GET(value, "renderorder", GetString(), obj.RenderOrder);
	JSON_GET(value, "staggeraxis", GetString(), obj.StaggerAxis);
	JSON_GET(value, "staggerindex", GetString(), obj.StaggerIndex);
	JSON_GET(value, "tileheight", GetInt(), obj.TileHeight);
	JSON_GET(value, "tilewidth", GetInt(), obj.TileWidth);
	JSON_GET(value, "width", GetInt(), obj.Width);
	JSON_GET(value, "type", GetString(), obj.Type);
	JSON_GET(value, "tiledversion", GetString(), obj.TiledVersion);
	JSON_GET(value, "version", GetFloat(), obj.Version);

	Internal_ParseList(obj.Tilesets, value, "tilesets");
	Internal_ParseList(obj.Layers, value, "layers");
	Internal_ParseProperties(obj, value);
}

void Internal_Save(const Tiled::Map& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
{
	JSON_STRADD(value, "backgroundcolor", obj.BackgroundColor);
	JSON_ADD(value, "height", obj.Height);
	JSON_ADDIF(value, "hexsidelength", obj.HexSideLength, 0);
	JSON_ADD(value, "infinite", obj.Infinite);
	JSON_ADD(value, "nextlayerid", obj.NextLayerId);
	JSON_ADD(value, "nextobjectid", obj.NextObjectId);
	JSON_STRADD(value, "orientation", obj.Orientation);
	JSON_STRADD(value, "renderorder", obj.RenderOrder);
	JSON_STRADD(value, "staggeraxis", obj.StaggerAxis);
	JSON_STRADD(value, "staggerindex", obj.StaggerIndex);
	JSON_ADD(value, "tileheight", obj.TileHeight);
	JSON_ADD(value, "tilewidth", obj.TileWidth);
	JSON_ADD(value, "width", obj.Width);
	JSON_STRADD(value, "type", obj.Type);
	JSON_STRADD(value, "tiledversion", obj.TiledVersion);
	JSON_ADD(value, "version", obj.Version);

	Internal_SaveList(obj.Tilesets, value, allocator, "tilesets");
	Internal_SaveList(obj.Layers, value, allocator, "layers");
	Internal_SaveProperties(obj, value, allocator);
}

Tiled::Tileset& Tiled::ParseJson(Tileset&& tileset, Xe::IO::IStream* pStream)
{
	return ParseJson(tileset, pStream);
}

Tiled::Tileset& Tiled::ParseJson(Tileset& tileset, Xe::IO::IStream* pStream)
{
	rapidjson::XeReadStream stream(pStream);
	Internal_Parse(tileset, rapidjson::Document().ParseStream(stream));

	return tileset;
}

const Tiled::Tileset& Tiled::SaveJson(const Tileset& tileset, Xe::IO::IStream* pStream)
{
	rapidjson::Document document;
	document.SetObject();
	Internal_Save(tileset, document, document.GetAllocator());
	Internal_WriteToStream(pStream, document);
	
	return tileset;
}

Tiled::Map& Tiled::ParseJson(Map&& map, IO::IStream* pStream)
{
	return ParseJson(map, pStream);
}

Tiled::Map& Tiled::ParseJson(Map& map, IO::IStream* pStream)
{
	rapidjson::XeReadStream stream(pStream);
	Internal_Parse(map, rapidjson::Document().ParseStream(stream));

	return map;
}

const Tiled::Map& Tiled::SaveJson(const Map& map, IO::IStream* pStream)
{
	rapidjson::Document document;
	document.SetObject();
	Internal_Save(map, document, document.GetAllocator());
	Internal_WriteToStream(pStream, document);

	return map;
}