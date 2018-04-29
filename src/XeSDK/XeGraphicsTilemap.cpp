#include "pch.h"
#include <XeSDK.h>
#include <XeSDK/IGraphicsTilemap.h>

namespace Xe { namespace Graphics {
	/*using namespace Xe::Math;
	class CTilemap : public ITilemap {
		IContext& m_Context;
		IDrawing2d& m_Drawing;

		Size m_TileSize;
		Vector2f m_TileSizef;

		int m_TilesPerRow;
		Rectangle<float> m_TilesetRectf;
		Vector2f m_TilesetPos;
		Vector2f m_TilesetSize;
		Vector2f m_TilesetMul;
		Vector2f m_TilesetPadding;

		Rectanglef m_Camera;
		
		Size m_MapSize;
		int m_ParallaxSize;
		u16* m_Tilemap;
		float* m_Parallax;

		void SetTileset(const TilesetProperties& tileset);

		const Size& GetMapSize() const;
		void SetMapSize(const Size& size);

		void Lock(TilemapData& data);

		const Rectanglef& GetCamera() const;
		void SetCamera(const Rectanglef& camera);

		void Draw(int flags);
		void DrawStandard() const;
		void DrawFlip() const;

	public:
		CTilemap(IContext& context, IDrawing2d& drawing) :
			m_Context(context),
			m_Drawing(drawing)
		{
			m_Tilemap = (u16*)Memory::Alloc(0);
			m_Parallax = (float*)Memory::Alloc(0);
		}
		~CTilemap() {
			Memory::Free(m_Tilemap);
			Memory::Free(m_Parallax);
		}
	};
	
	void CTilemap::SetTileset(const TilesetProperties& tileset) {
		m_TilesPerRow = (tileset.Rectangle.right - tileset.Rectangle.left) / tileset.TileSize.x;
		int tilesPerCol = (tileset.Rectangle.bottom - tileset.Rectangle.top) / tileset.TileSize.y;
		float xMul = 1.0f / (float)tileset.TextureSize.x;
		float yMul = 1.0f / (float)tileset.TextureSize.y;

		m_TileSize = tileset.TileSize;
		m_TileSizef.x = (float)m_TileSize.x;
		m_TileSizef.y = (float)m_TileSize.y;
		m_TilesetRectf.left = tileset.Rectangle.left * xMul;
		m_TilesetRectf.top = tileset.Rectangle.top * yMul;
		m_TilesetRectf.right = tileset.Rectangle.right * xMul;
		m_TilesetRectf.bottom = tileset.Rectangle.bottom * yMul;
		m_TilesetMul.x = m_TileSizef.x * xMul;
		m_TilesetMul.y = m_TileSizef.y * xMul;
	}

	const Size& CTilemap::GetMapSize() const {
		return m_MapSize;
	}
	void CTilemap::SetMapSize(const Size& size) {
		Memory::Free(m_Tilemap);
		Memory::Free(m_Parallax);

		m_MapSize = size;
		m_ParallaxSize = Math::Max(size.x, size.y);
		m_Tilemap = (u16*)Memory::Alloc(size.x * size.y * sizeof(u16));
		m_Parallax = (float*)Memory::Alloc(m_ParallaxSize * sizeof(float));
	}
	void CTilemap::Lock(TilemapData& data) {
		data.Tilemap = m_Tilemap;
		data.Stride = m_MapSize.x;
		data.Size = m_MapSize;
	}
	const Rectanglef& CTilemap::GetCamera() const {
		return m_Camera;
	}
	void CTilemap::SetCamera(const Rectanglef& camera) {
		m_Camera = camera;
	}

	void CTilemap::Draw(int flags) {
		switch (flags) {
		case Draw_Default:
			DrawStandard();
			break;
		}
	}
	void CTilemap::DrawStandard() const {
		static const float Z = 0.0f;
		static const Color COLOR = Color::White;

		const float MULX = m_TilesetMul.x;
		const float MULY = m_TilesetMul.y;
		const float PADX = 0;
		const float PADY = 0;

		Rectangle<int> tileRect(
			(int)Floor(m_Camera.left) / m_TileSize.x,
			(int)Floor(m_Camera.top) / m_TileSize.y,
			((int)Floor(m_Camera.right) + m_TileSize.x - 1) / m_TileSize.x,
			((int)Floor(m_Camera.bottom) + m_TileSize.y - 1) / m_TileSize.y);
		int stride = m_MapSize.x;

		float xStart = Fmod(m_Camera.left, m_TileSizef.x);
		float yStart = Fmod(m_Camera.top, m_TileSizef.y);
		Vector2f pos[2];
		pos[0].y = -yStart;
		pos[1].y = pos[0].y + m_TileSizef.y;
		for (int j = tileRect.top; j < tileRect.bottom; j++) {
			pos[0].x = -xStart;
			pos[1].x = pos[0].x + m_TileSizef.x;
			for (int i = tileRect.left; i < tileRect.right; i++) {
				int index = i % stride + (j % m_MapSize.y) * stride;
				u16 tile = m_Tilemap[index] & 0x3FFF;
				if (tile > 0) {
					float fx = (tile % m_TilesPerRow) * m_TilesetMul.x;
					float fy = (tile / m_TilesPerRow) * m_TilesetMul.y;
					const Vector2f UV[] = {
						Vector2f(fx + PADX, fy + PADY),
						Vector2f(fx + MULX - PADX, fy + PADY),
						Vector2f(fx + PADX, fy + MULY - PADY),
						Vector2f(fx + MULX - PADX , fy + MULY - PADY),
					};
					m_Drawing.DrawSurface(pos, UV, Z, COLOR, m_Drawing.MODE_TEXTURED);
				}
				pos[0].x = pos[1].x;
				pos[1].x += m_TileSizef.x;
			}
			pos[0].y = pos[1].y;
			pos[1].y += m_TileSizef.y;
		}
	}
	void CTilemap::DrawFlip() const {
		static const float Z = 0.0f;
		static const Color COLOR = Color::White;

		const float MULX = m_TilesetMul.x;
		const float MULY = m_TilesetMul.y;
		const float PADX = 0;
		const float PADY = 0;

		Rectangle<int> tileRect(
			(int)Floor(m_Camera.left) / m_TileSize.x,
			(int)Floor(m_Camera.top) / m_TileSize.y,
			((int)Floor(m_Camera.right) + m_TileSize.x - 1) / m_TileSize.x,
			((int)Floor(m_Camera.bottom) + m_TileSize.y - 1) / m_TileSize.y);
		int stride = m_MapSize.x;

		float xStart = Fmod(m_Camera.left, m_TileSizef.x);
		float yStart = Fmod(m_Camera.top, m_TileSizef.y);
		Vector2f pos[2];
		pos[0].y = -yStart;
		pos[1].y = pos[0].y + m_TileSizef.y;
		for (int j = tileRect.top; j < tileRect.bottom; j++) {
			pos[0].x = -xStart;
			pos[1].x = pos[0].x + m_TileSizef.x;
			for (int i = tileRect.left; i < tileRect.right; i++) {
				int index = i % stride + (j % m_MapSize.y) * stride;
				u16 tile = m_Tilemap[index] & 0x3FFF;
				if (tile > 0) {
					float fx = (tile % m_TilesPerRow) * m_TilesetMul.x;
					float fy = (tile / m_TilesPerRow) * m_TilesetMul.y;
					int flipXY = tile >> 14;
					switch (flipXY) {
					case 0: {
						const Vector2f UV[] = {
							Vector2f(fx + PADX, fy + PADY),
							Vector2f(fx + MULX - PADX, fy + PADY),
							Vector2f(fx + PADX, fy + MULY - PADY),
							Vector2f(fx + MULX - PADX , fy + MULY - PADY),
						};
						m_Drawing.DrawSurface(pos, UV, Z, COLOR, m_Drawing.MODE_TEXTURED);
					} break;
					case 1: {
						const Vector2f UV[] = {
							Vector2f(fx + MULX - PADX, fy + PADY),
							Vector2f(fx + PADX, fy + PADY),
							Vector2f(fx + MULX - PADX, fy + MULY - PADY),
							Vector2f(fx + PADX, fy + MULY - PADY),
						};
						m_Drawing.DrawSurface(pos, UV, Z, COLOR, m_Drawing.MODE_TEXTURED);
					} break;
					case 2: {
						const Vector2f UV[] = {
							Vector2f(fx + PADX, fy + MULY - PADY),
							Vector2f(fx + MULX - PADX, fy + MULY - PADY),
							Vector2f(fx + PADX, fy + PADY),
							Vector2f(fx + MULX - PADX, fy + PADY),
						};
						m_Drawing.DrawSurface(pos, UV, Z, COLOR, m_Drawing.MODE_TEXTURED);
					} break;
					case 3: {
						const Vector2f UV[] = {
							Vector2f(fx + MULX - PADX, fy + MULY - PADY),
							Vector2f(fx + PADX, fy + MULY - PADY),
							Vector2f(fx + MULX - PADX, fy + PADY),
							Vector2f(fx + PADX , fy + PADY),
						};
						m_Drawing.DrawSurface(pos, UV, Z, COLOR, m_Drawing.MODE_TEXTURED);
					} break;
					}
				}
				pos[0].x = pos[1].x;
				pos[1].x += m_TileSizef.x;
			}
			pos[0].y = pos[1].y;
			pos[1].y += m_TileSizef.y;
		}
	}

	void ITilemap::CreateInstance(ITilemap** tilemap, IContext& context, IDrawing2d& drawing) {
		*tilemap = new CTilemap(context, drawing);
	}*/
} }