#pragma once
#include <XeSDK/IIOStream.h>
#include <XeSDK/XeString.h>
#include <vector>
#include <list>
#include <map>

namespace Xe { namespace Game { namespace Tiled {
	enum PropertyType
	{
		PropertyType_Unknown,
		PropertyType_Bool,
		PropertyType_Int,
		PropertyType_Float,
		PropertyType_String,
		PropertyType_Color,
		PropertyType_File
	};

	struct Gid
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

	static_assert(sizeof(Gid) == sizeof(Gid::Data), "Tiled::Gid size is different than expected");

	struct GenericProperty
	{
		PropertyType Type;
		String Value;
		union
		{
			bool b;
			int i32;
			float f;
		};
	};

	struct BoolProperty : public GenericProperty { BoolProperty(bool value) { Type = PropertyType_Bool; b = value; } };
	struct IntProperty : public GenericProperty { IntProperty(int value) { Type = PropertyType_Int; i32 = value; } };
	struct FloatProperty : public GenericProperty { FloatProperty(float value) { Type = PropertyType_Float; f = value; } };
	struct StringProperty : public GenericProperty { StringProperty(const StringSpan& value) { Type = PropertyType_String; Value = value.GetData(); } };
	struct ColorProperty : public GenericProperty { ColorProperty(const StringSpan& value) { Type = PropertyType_Color; Value = value.GetData(); } };
	struct FileProperty : public GenericProperty { FileProperty(const StringSpan& value) { Type = PropertyType_File; Value = value.GetData(); } };

	struct GenericProperties
	{
		std::map<String, GenericProperty> Properties;
	};

	struct Layer;
	struct GenericLayers
	{
		//! Array of Layers
		std::list<Layer> Layers;
	};

	struct Object;
	struct GenericObjects
	{
		//! Array of objects. objectgroup only.
		std::list<Object> Objects;
	};

	struct Frame
	{
		//! Frame duration in milliseconds
		int Duration;

		//! Local tile ID representing this frame
		int TileId;
	};

	struct Tile : public GenericProperties
	{
		//! Local ID of the tile
		int Id;

		//! Probability used when randomizing
		double Probability;

		//! The type of the tile (optional)
		String Type;

		//! Array of Frames
		std::list<Frame> Animation;
	};

	struct Tileset : public GenericProperties
	{
		//! The number of tile columns in the tileset
		int Columns;

		//! GID corresponding to the first tile in the set
		int FirstGid;

		//! Image used for tiles in this set
		String Image;

		//! Width of source image in pixels
		int ImageWidth;

		//! Height  of source image in pixels
		int ImageHeight;

		//! Buffer between image edge and first tile (pixels)
		int Margin;

		//! Name given to this tileset
		String Name;

		//! Spacing between adjacent tiles in image (pixels)
		int Spacing;

		//! The number of tiles in this tileset
		int TileCount;

		//! Maximum height of tiles in this set
		int TileHeight;

		//! Maximum width of tiles in this set
		int TileWidth;

		//! Hex-formatted color (#RRGGBB) (optional)
		String TransparentColor;

		//! tileset (for tileset files, since 1.0)
		String Type;

		//! Source of the tileset
		String Source;

		std::list<Tile> Tiles;

		String TiledVersion;

		float Version;
	};

	struct ObjectPosition
	{
		double X, Y;

		ObjectPosition() : X(0.0), Y(0.0) {}
		ObjectPosition(double x, double y) : X(x), Y(y) {}
	};

	struct Object : public GenericProperties
	{
		//! GID, only if object comes from a Tilemap
		int Gid;

		//! Incremental id - unique across all objects
		int Id;

		//! X coordinate in pixels
		double X;

		//! Y coordinate in pixels
		double Y;

		//! Width in pixels. Ignored if using a gid.
		double Width;

		//! Height in pixels. Ignored if using a gid.
		double Height;

		//! Angle in degrees clockwise
		double Rotation;

		//! String assigned to type field in editor
		String Type;

		//! String assigned to name field in editor
		String Name;

		//! Reference to a template file, in case object is a template instance
		String Template;

		//! Whether object is shown in editor.
		bool Visible;

		//! Used to mark an object as a point
		bool Point;

		//! Used to mark an object as an ellipse
		bool Ellipse;

		//! A list of x,y coordinates in pixels
		std::list<ObjectPosition> Polygon;

		//! A list of x,y coordinates in pixels
		std::list<ObjectPosition> Polyline;
	};

	struct Layer :
		public GenericProperties,
		public GenericLayers,
		public GenericObjects
	{
		// zlib, gzip or empty (default). tilelayer only.
		String Compression;

		//! Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
		std::vector<Gid> Data;

		//! topdown (default) or index. objectgroup only.
		String DrawOrder;

		//! csv (default) or base64`. ``tilelayer only.
		String Encoding;

		//! Row count. Same as map height for fixed-size maps.
		int Height;

		//! Incremental id - unique across all layers
		int Id;

		//! Image used by this layer. imagelayer only.
		String Image;

		//! Name assigned to this layer
		String Name;

		//! Horizontal layer offset in pixels (default: 0)
		double OffsetX;

		//! Vertical layer offset in pixels (default: 0)
		double OffsetY;

		//! Value between 0 and 1
		float Opacity;

		//! Hex-formatted color (#RRGGBB) (optional, imagelayer only
		String TransparentColor;

		//! tilelayer, objectgroup, imagelayer or group
		String Type;

		//! Whether layer is shown or hidden in editor
		bool Visible;

		//! Column count. Same as map width for fixed-size maps.
		int Width;

		//! Horizontal layer offset in tiles. Always 0.
		int X;

		//! Vertical layer offset in tiles. Always 0.
		int Y;
	};

	struct Map :
		public GenericProperties,
		public GenericLayers
	{
		//! Hex-formatted color (#RRGGBB or #AARRGGBB) (optional)
		String BackgroundColor;

		//! Number of tile rows
		int Height;

		//! Length of the side of a hex tile in pixels
		int HexSideLength;

		//! Whether the map has infinite dimensions
		bool Infinite;

		//! Auto-increments for each layer
		int NextLayerId;

		//! Auto-increments for each placed object
		int NextObjectId;

		//! orthogonal, isometric, staggered or hexagonal
		String Orientation;

		//! Rendering direction (orthogonal maps only)
		String RenderOrder;

		//! x or y (staggered / hexagonal maps only)
		String StaggerAxis;

		//! odd or even (staggered / hexagonal maps only)
		String StaggerIndex;

		//! Map grid height
		int TileHeight;

		//! Array of Tilesets
		std::list<Tileset> Tilesets;

		//! Map grid width
		int TileWidth;

		//! map (since 1.0)
		String Type;

		//! Number of tile columns
		int Width;

		//! The Tiled version used to save the file
		String TiledVersion;

		//! The JSON format version
		float Version;
	};

	Tileset& ParseJson(Tileset&& tileset, Xe::IO::IStream* pStream);
	Tileset& ParseJson(Tileset& tileset, Xe::IO::IStream* pStream);
	const Tileset& SaveJson(const Tileset& tileset, Xe::IO::IStream* pStream);

	Map& ParseJson(Map&& map, IO::IStream* pStream);
	Map& ParseJson(Map& map, Xe::IO::IStream* pStream);
	const Map& SaveJson(const Map& map, Xe::IO::IStream* pStream);
} } }