#pragma once
#include <XeSDK/XeGraphicsColor.h>

namespace Xe { namespace Game {
	struct TilesetProperties {
		// Size of a single tile
		Math::Vector2i TileSize;
		// Size of the texture that contains the tileset
		Math::Vector2i TextureSize;
		// Where the tileset is located inside the texture
		Math::Rectangle<int> Rectangle;
	};
	struct TileData {
		union {
			struct {
				u16 Index : 14;
				u16 Flip : 2;
			};
			u16 Tile;
		};
		u16 RESERVED;
		Graphics::Color BlendColor;
	};
	struct TilemapData {
		TileData* Tilemap;
		int Stride;
		Math::Vector2i Size;
	};

	interface ITilemap2d : public IObject
	{
		enum DrawFlags {
			Draw_Default = 0,
			Draw_Flip = 1,
			Draw_Palette = 2,
			Draw_Parallax = 4,
			Draw_Color = 8,
		};

		virtual ~ITilemap2d() {};

		virtual void SetTileset(const TilesetProperties& tileset) = 0;

		//! \brief Get the current size of the tilemap
		//! \return Size of tilemap; by default it is set to (0, 0)
		virtual const Math::Vector2i& GetMapSize() const = 0;

		//! \brief Set the number of cells horizontally and vertically
		//! \param[in] size Size of the tilemap
		/** \details Every time that a new size iss specified, the old content
		 * will be invalidated.
		 */
		virtual void SetMapSize(const Math::Vector2i& size) = 0;

		virtual void Lock(TilemapData& data) = 0;
		virtual void Unlock() = 0;

		virtual const Math::Rectanglef& GetCamera() const = 0;
		virtual void SetCamera(const Math::Rectanglef& camera) = 0;

		virtual void Draw(int flags) = 0;
	};
} }