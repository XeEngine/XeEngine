#include "pch.h"
#include "ContextD3D11.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// Tilemap2d implementation
		bool CContextD3D11::CTilemap2d::Query(IObject **obj, UID id) {
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
		CContextD3D11::CTilemap2d::CTilemap2d(IContext *context) :
			Tilemap2d(context) {
		}
		CContextD3D11::CTilemap2d::~CTilemap2d() {

		}
		void CContextD3D11::CTilemap2d::SetViewSize(const Math::Vector2f& size) {
			m_ViewSize = size;
		}
		void CContextD3D11::CTilemap2d::SetPosition(uvar layer, const Math::Vector2f& position) {

		}
		void CContextD3D11::CTilemap2d::OnDraw() {

		}
		void CContextD3D11::CTilemap2d::OnTilesetChanged(ISurface *pSurface) {

		}
		void CContextD3D11::CTilemap2d::OnColorLutChanged(ISurface *pSurface) {

		}
	}
}