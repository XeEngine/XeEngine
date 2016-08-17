#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/IGraphicsContext.h>

namespace Xe {
	namespace Graphics {
		//! \brief Offers fast tile drawing techniques
		/** \details It will be offered to the developer an array of Tile,
		 * a texture where graphics is stored and, optionally, a palette
		 * for indexed colors.
		 */
		class Tilemap2d : public IObject {
		public:
			static const UID ID = 0x911b3281a4c74364ULL;

			//! \brief How wrap at or over edges of the tilemap.
			enum Wrap {
				//! \brief Use a specific tile
				Wrap_Tile,
				//! \brief Use the tile at the border
				Wrap_Clamp,
				//! \brief Repeat the content
				Wrap_Repeat,
			};

			//! \brief How a tile block can be filled.
			struct Tile {
				union {
					struct {
						//! \brief Tile index starting from top-left
						u16 tile;
						//! \brief Palette index to use to draw current tile
						u8 palette;

						union
						{
							//! \brief Holds various optional parameters
							u8 Param;
							struct
							{
								//! \brief tile is flipped vertically
								u8 flip : 1;
								//! \brief tile is flipped horizontally
								u8 mirror : 1;
								//! \brief used to draw bunch of tiles separately
								u8 priority : 1;
								//! \brief RESERVED FOR FUTURE USE
								u8 reserved : 1;
								//! \brief Path flags 1. Not used in rendering.
								u8 path1 : 2;
								//! \brief Path flags 2. Not used in rendering.
								u8 path2 : 2;
							};
						};
					};
					u32 data;
				};
			};

			static const svar TileSize = 16;
			static const svar TilemapWidth = 32;
			static const svar TilemapHeight = 24;
			static const svar LayersCount = 4;

			//! \brief Map where tile information is stored in top-left order.
			Tile *Tilemap;

			Tilemap2d(IContext *context);
			virtual ~Tilemap2d();

			//! \brief Get the view of area; default is 160.0f, 144.0f
			const Math::Vector2f& GetViewSize() const;
			//! \brief View of area
			virtual void SetViewSize(const Math::Vector2f& size) = 0;

			//! \brief Get the position of the specified layer
			//! \param[in] layer Must be a value between 0 and LayersCount-1
			const Math::Vector2f& GetPosition(uvar layer);
			//! \brief Set the position of the specified layer
			//! \param[in] layer Must be a value between 0 and LayersCount-1
			virtual void SetPosition(uvar layer, const Math::Vector2f& position) = 0;

			//! \brief Get a tile in specified position
			//! \details If x or y are out from Tilemap size, wrap will be used.
			const Tile& GetTile(svar x, svar y) const;
			//! \brief Set a tile into specified position
			//! \details If x or y are out from Tilemap size, tile will be not set.
			void SetTile(svar x, svar y, const Tile& tile);

			//! \brief Tell to the structure that Tilemap content was updated.
			void Invalidate();

			void SelectSurface(ISurface *pSurface);
			void SelectColorLut(ISurface *pSurface);

			void Update(double deltaTime);
			void Draw();

		protected:
			Wrap m_Wrap;
			Tile m_TileBorder;
			Math::Vector2f m_ViewSize;
			Math::Vector2f *m_LayerPos;
			IContext *m_pContext;
			ISurface *m_pTileset;
			ISurface *m_pClut;

			//! \brief Calculate the index with wrapped content
			svar GetRealIndex(svar x, svar y) const;

			bool IsInvaldiated() const;
			void Validate();

		private:
			bool m_IsContentInvalidated;

			void SelectSurface(ISurface **pDst, ISurface *pSrc);
			virtual void OnDraw() = 0;
			virtual void OnTilesetChanged(ISurface *pSurface) = 0;
			virtual void OnColorLutChanged(ISurface *pSurface) = 0;
		};
	}
}