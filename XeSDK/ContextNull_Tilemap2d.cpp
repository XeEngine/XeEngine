#include "pch.h"
#include "ContextNull.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// Tilemap2d implementation
		bool CContextNull::CTilemap::Query(IObject **obj, UID id) {
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
		CContextNull::CTilemap::CTilemap(IContext *pContext) :
			m_pContext(pContext)
		{
			m_pContext->AddRef();
		}
		CContextNull::CTilemap::~CTilemap() {
			m_pContext->Release();
		}

		void CContextNull::CTilemap::SetTileset(const TilesetProperties& tileset) {}
		const Size& CContextNull::CTilemap::GetMapSize() const { return Size(); }
		void CContextNull::CTilemap::SetMapSize(const Size& size) {}
		void CContextNull::CTilemap::Lock(TilemapData& data) {
			data.Tilemap = nullptr;
			data.Stride = 0;
			data.Size = Size();
		}
		void CContextNull::CTilemap::Unlock() {}
		const Math::Rectanglef& CContextNull::CTilemap::GetCamera() const { return Math::Rectanglef(); }
		void CContextNull::CTilemap::SetCamera(const Math::Rectanglef& camera) {}
		void CContextNull::CTilemap::Draw(int flags) {}
	}
}