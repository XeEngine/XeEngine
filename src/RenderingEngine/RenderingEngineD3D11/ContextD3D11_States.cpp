#include "pch.h"
#include "ContextD3D11.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// IDepthStencilState implementation
		bool CContextD3D11::CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) {
			*depthStencilState = new CDepthStencilState(this, desc);
			return *depthStencilState != nullptr;
		}
		void CContextD3D11::SelectDepthStencilState(IDepthStencilState *depthStencilState) {
			if (depthStencilState != nullptr) {
				CDepthStencilState *p;
				if (depthStencilState->Query((IObject**)&p, CDepthStencilState::ID)) {
					if (p->m_context == this) {
						m_Drawing->Flush();
						if (m_DepthStencilState) m_DepthStencilState->Release();
						m_DepthStencilState = p;
						m_DepthStencilState->AddRef();
						// TODO set state
					}
					p->Release();
				}
				// If the object does not belong to current context, then ignore it.
			}
			else {
				if (m_DepthStencilState) m_DepthStencilState->Release();
				m_DepthStencilState = nullptr;
				// TODO clear state
			}
		}

		bool CContextD3D11::CDepthStencilState::Query(IObject **obj, UID id) {
			switch (id) {
			case CDepthStencilState::ID:
			case IDepthStencilState::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextD3D11::CDepthStencilState::CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc) :
			m_context(context) {
			m_context->AddRef();
		}
		CContextD3D11::CDepthStencilState::~CDepthStencilState() {
			m_context->Release();
		}
	}
}