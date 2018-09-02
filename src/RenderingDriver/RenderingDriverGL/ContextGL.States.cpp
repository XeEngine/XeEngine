#include "pch.h"
#ifdef PLATFORM_GL
#include <XeSDK/IGraphicsDrawing2d.h>
#include "ContextGL.Common.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// IDepthStencilState implementation
		bool CContextGLCommon::CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) {
			*depthStencilState = new CDepthStencilState(this, desc);
			return *depthStencilState != nullptr;
		}
		void CContextGLCommon::SelectDepthStencilState(IDepthStencilState *depthStencilState) {
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

		CContextGLCommon::CDepthStencilState::CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc) :
			m_context(context) {
			m_context->AddRef();
		}
		CContextGLCommon::CDepthStencilState::~CDepthStencilState() {
			m_context->Release();
		}
	}
}
#endif