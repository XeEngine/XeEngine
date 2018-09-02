#include "pch.h"
#include "ContextGl.h"
#include <XeSDK/IGraphicsContext.h>
#ifdef PLATFORM_GL
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/XeMemory.h>

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
		CContextGL::CContextGL(Core::IView *pView) :
			CContextGLCommon(pView),
			m_Size(800, 480) {
		}
		CContextGL::~CContextGL() {
			if (m_Drawing) m_Drawing->Release();
		}

		bool CContextGL::Initialize(const ContextInitDesc& properties) {
			if (CContextGLCommon::Initialize(properties)) {
				return true;
			}
			return false;
		}
		void CContextGL::GetDrawing(IDrawing2d** drawing) {
			if (m_Drawing == nullptr)
				m_Drawing = new CDrawing(this);
			m_Drawing->AddRef();
			*drawing = m_Drawing;
		}
		void CContextGL::CreateTilemap(ITilemap** pTilemap) {
			*pTilemap = new CTilemap(this);
		}

		void CContextGL::SetClearDepth(float depth) {
			SetClearDepthSub(depth);
			glClearDepth((GLclampd)depth);
			//glClearDepthf((GLclampf)depth); // GLES
		}

		void CContextGL::SwapBuffers(VBlankSync sync) {
			//m_Drawing->Flush();
			SwapBuffersSub(sync);
		}
	}
}
#else
namespace Xe {
	namespace Graphics {
		bool CreateContextGL(IContext **context, Core::IView* pView, const ContextInitDesc& properties) {
			*context = nullptr;
			return false;
		}
	}
}
#endif