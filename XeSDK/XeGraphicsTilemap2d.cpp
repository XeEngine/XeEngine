#include "pch.h"
#include <XeSDK/XeGraphicsTilemap2d.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
	namespace Graphics {
		Tilemap2d::Tilemap2d(IContext *pContext) :
			Tilemap((Tile*)Memory::Alloc(sizeof(Tile) * TilemapWidth * TilemapHeight)),
			m_Wrap(Wrap_Repeat),
			m_ViewSize(Math::Vector2f(160.0f, 144.0f)),
			m_LayerPos(new Math::Vector2f[LayersCount]),
			m_pContext(pContext),
			m_pTileset(nullptr),
			m_pClut(nullptr),
			m_IsContentInvalidated(true)
		{

		}
		Tilemap2d::~Tilemap2d() {
			if (m_pTileset) m_pTileset->Release();
			if (m_pClut) m_pClut->Release();
			Memory::Free(Tilemap);
			delete[] m_LayerPos;
		}
		const Math::Vector2f& Tilemap2d::GetViewSize() const {
			return m_ViewSize;
		}
		const Math::Vector2f& Tilemap2d::GetPosition(uvar layer) {
			return m_LayerPos[layer];
		}
		const Tilemap2d::Tile& Tilemap2d::GetTile(svar x, svar y) const {
			svar i = GetRealIndex(x, y);
			if (i < 0) return m_TileBorder;
			return Tilemap[i];
		}
		void Tilemap2d::SetTile(svar x, svar y, const Tile& tile) {
			if (x >= 0 && x < TilemapWidth &&
				y >= 0 && y < TilemapHeight)
				Tilemap[x + y * TilemapWidth] = tile;
		}
		void Tilemap2d::Invalidate() {
			m_IsContentInvalidated = true;
		}
		bool Tilemap2d::IsInvaldiated() const {
			return m_IsContentInvalidated;
		}
		void Tilemap2d::Validate() {
			m_IsContentInvalidated = false;
		}
		void Tilemap2d::SelectSurface(ISurface *pSurface) {
			SelectSurface(&m_pTileset, pSurface);
			OnTilesetChanged(pSurface);
		}
		void Tilemap2d::SelectColorLut(ISurface *pSurface) {
			SelectSurface(&m_pClut, pSurface);
			OnColorLutChanged(pSurface);
		}

		void Tilemap2d::Update(double deltaTime) {

		}
		void Tilemap2d::Draw() {
			OnDraw();
		}

		svar Tilemap2d::GetRealIndex(svar x, svar y) const {
			switch (m_Wrap) {
			case Wrap_Tile:
				if (x < 0) return -1;
				else if (x >= TilemapWidth) return -1;
				if (y < 0) y = 0;
				else if (y >= TilemapHeight) return -1;
				break;
			case Wrap_Clamp:
				if (x < 0) x = 0;
				else if (x >= TilemapWidth) x = TilemapWidth - 1;
				if (y < 0) y = 0;
				else if (y >= TilemapHeight) y = TilemapHeight - 1;
				break;
			case Wrap_Repeat:
				if (x < 0) x = TilemapWidth - (x % TilemapWidth);
				else if (x >= TilemapWidth) x %= TilemapWidth;
				if (y < 0) y = TilemapHeight - (y % TilemapHeight);
				else if (y >= TilemapHeight) y %= TilemapHeight;
				break;
			}
			return x + y * TilemapWidth;
		}
		void Tilemap2d::SelectSurface(ISurface **pDst, ISurface *pSrc) {
			if (*pDst) (*pDst)->Release();
			*pDst = pSrc;
			if (pSrc) pSrc->AddRef();
		}
	}
}