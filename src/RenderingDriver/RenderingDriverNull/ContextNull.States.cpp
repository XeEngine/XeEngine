#include "pch.h"
#include "ContextNull.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// IDepthStencilState implementation
		bool CContextNull::CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) {
			*depthStencilState = new CDepthStencilState(this, desc);
			return *depthStencilState != nullptr;
		}
		void CContextNull::SelectDepthStencilState(IDepthStencilState *depthStencilState) {
			if (depthStencilState != nullptr) {
				CDepthStencilState *p;
				if (p = static_cast<CDepthStencilState*>(depthStencilState)) {
					if (p->m_context == this) {
						m_Drawing->Flush();
						if (m_DepthStencilState) m_DepthStencilState->Release();
						m_DepthStencilState = p;
						m_DepthStencilState->AddRef();
						// TODO set state
					}
				}
				// If the object does not belong to current context, then ignore it.
			}
			else {
				if (m_DepthStencilState) m_DepthStencilState->Release();
				m_DepthStencilState = nullptr;
				// TODO clear state
			}
		}

		CContextNull::CDepthStencilState::CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc) :
			m_context(context) {
			m_context->AddRef();
		}
		CContextNull::CDepthStencilState::~CDepthStencilState() {
			m_context->Release();
		}
	}
}