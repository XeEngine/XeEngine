#include "pch.h"
#include "ContextNull.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// Tilemap2d implementation
		bool CContextNull::CTilemap2d::Query(IObject **obj, UID id) {
			switch (id) {
			case Tilemap2d::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextNull::CTilemap2d::CTilemap2d(IContext *context) :
			Tilemap2d(context) {
		}
		CContextNull::CTilemap2d::~CTilemap2d() {

		}
		void CContextNull::CTilemap2d::SetViewSize(const Math::Vector2f& size) {
			m_ViewSize = size;
		}
		void CContextNull::CTilemap2d::SetPosition(uvar layer, const Math::Vector2f& position) {

		}
		void CContextNull::CTilemap2d::OnDraw() {

		}
		void CContextNull::CTilemap2d::OnTilesetChanged(ISurface *pSurface) {

		}
		void CContextNull::CTilemap2d::OnColorLutChanged(ISurface *pSurface) {

		}
	}
}