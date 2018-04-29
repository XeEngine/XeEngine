#include "pch.h"
#include "ContextD3D11.h"
#include <XeSDK/XeMemory.h>

namespace Xe { namespace Graphics {
	bool CContextD3D11::CTilemap::Query(IObject **obj, UID id) {
		switch (id) {
		case ITilemap::ID:
		case IObject::ID:
			AddRef();
			*obj = this;
			return true;
		}
		*obj = nullptr;
		return false;
	}
	CContextD3D11::CTilemap::CTilemap(IContext* pContext) :
		m_pContext(pContext)
	{
		m_pContext->AddRef();
		m_pContext->GetDrawing(&m_pDrawing);
	}
	CContextD3D11::CTilemap::~CTilemap() {
		Memory::Free(m_Tilemap);
		Memory::Free(m_Parallax);
		m_pDrawing->Release();
		m_pContext->Release();
	}
	void CContextD3D11::CTilemap::SetTileset(const TilesetProperties& tileset) {
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

	const Size& CContextD3D11::CTilemap::GetMapSize() const {
		return m_MapSize;
	}
	void CContextD3D11::CTilemap::SetMapSize(const Size& size) {
		Memory::Free(m_Tilemap);
		Memory::Free(m_Parallax);

		m_MapSize = size;
		m_ParallaxSize = Math::Max(size.x, size.y);
		m_Tilemap = (TileData*)Memory::Alloc(size.x * size.y * sizeof(TileData));
		m_Parallax = (float*)Memory::Alloc(m_ParallaxSize * sizeof(float));
	}
	void CContextD3D11::CTilemap::Lock(TilemapData& data) {
		data.Tilemap = m_Tilemap;
		data.Stride = m_MapSize.x;
		data.Size = m_MapSize;
	}

	void CContextD3D11::CTilemap::Unlock() {}

	const Rectanglef& CContextD3D11::CTilemap::GetCamera() const {
		return m_Camera;
	}
	void CContextD3D11::CTilemap::SetCamera(const Rectanglef& camera) {
		m_Camera = camera;
	}

	void CContextD3D11::CTilemap::Draw(int flags) {
		switch (flags) {
		case Draw_Default:
			DrawStandard();
			break;
		}
	}
	void CContextD3D11::CTilemap::DrawStandard() const {
		static const float Z = 0.0f;
		static const Color COLOR = Color::White;

		const float MULX = m_TilesetMul.x;
		const float MULY = m_TilesetMul.y;
		const float PADX = 0;
		const float PADY = 0;

		Math::Rectangle<int> tileRect(
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
				u16 tile = m_Tilemap[index].Index;
				if (tile > 0) {
					Color color = m_Tilemap[index].BlendColor;
					float fx = (tile % m_TilesPerRow) * m_TilesetMul.x;
					float fy = (tile / m_TilesPerRow) * m_TilesetMul.y;
					const Vector2f UV[] = {
						Vector2f(fx + PADX, fy + PADY),
						Vector2f(fx + MULX - PADX, fy + PADY),
						Vector2f(fx + PADX, fy + MULY - PADY),
						Vector2f(fx + MULX - PADX , fy + MULY - PADY),
					};
					m_pDrawing->DrawSurface(pos, UV, Z, color, m_pDrawing->MODE_TEXTURED);
				}
				pos[0].x = pos[1].x;
				pos[1].x += m_TileSizef.x;
			}
			pos[0].y = pos[1].y;
			pos[1].y += m_TileSizef.y;
		}
	}
	void CContextD3D11::CTilemap::DrawFlip() const {
		static const float Z = 0.0f;
		static const Color COLOR = Color::White;

		const float MULX = m_TilesetMul.x;
		const float MULY = m_TilesetMul.y;
		const float PADX = 0;
		const float PADY = 0;

		Math::Rectangle<int> tileRect(
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
				u16 tile = m_Tilemap[index].Index;
				if (tile > 0) {
					Color color = m_Tilemap[index].BlendColor;
					float fx = (tile % m_TilesPerRow) * m_TilesetMul.x;
					float fy = (tile / m_TilesPerRow) * m_TilesetMul.y;
					switch (m_Tilemap[index].Flip) {
					case 0: {
						const Vector2f UV[] = {
							Vector2f(fx + PADX, fy + PADY),
							Vector2f(fx + MULX - PADX, fy + PADY),
							Vector2f(fx + PADX, fy + MULY - PADY),
							Vector2f(fx + MULX - PADX , fy + MULY - PADY),
						};
						m_pDrawing->DrawSurface(pos, UV, Z, color, m_pDrawing->MODE_TEXTURED);
					} break;
					case 1: {
						const Vector2f UV[] = {
							Vector2f(fx + MULX - PADX, fy + PADY),
							Vector2f(fx + PADX, fy + PADY),
							Vector2f(fx + MULX - PADX, fy + MULY - PADY),
							Vector2f(fx + PADX, fy + MULY - PADY),
						};
						m_pDrawing->DrawSurface(pos, UV, Z, color, m_pDrawing->MODE_TEXTURED);
					} break;
					case 2: {
						const Vector2f UV[] = {
							Vector2f(fx + PADX, fy + MULY - PADY),
							Vector2f(fx + MULX - PADX, fy + MULY - PADY),
							Vector2f(fx + PADX, fy + PADY),
							Vector2f(fx + MULX - PADX, fy + PADY),
						};
						m_pDrawing->DrawSurface(pos, UV, Z, color, m_pDrawing->MODE_TEXTURED);
					} break;
					case 3: {
						const Vector2f UV[] = {
							Vector2f(fx + MULX - PADX, fy + MULY - PADY),
							Vector2f(fx + PADX, fy + MULY - PADY),
							Vector2f(fx + MULX - PADX, fy + PADY),
							Vector2f(fx + PADX , fy + PADY),
						};
						m_pDrawing->DrawSurface(pos, UV, Z, color, m_pDrawing->MODE_TEXTURED);
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
} }