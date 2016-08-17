#include "pch.h"
#include "ContextGl.h"
#include <XeSDK/IGraphicsContext.h>
#ifdef PLATFORM_GL
#include <XeSDK/XeDebugLog.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/XeMemory.h>

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
		bool CContextGL::Query(IObject **obj, UID id) {
			switch (id) {
			case IContext::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			case IDrawing2d::ID:
				m_Drawing->AddRef();
				*obj = m_Drawing;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextGL::CContextGL(Core::IView *pView) :
			CContextGLCommon(pView),
			m_Tilemap(nullptr),
			m_Size(800, 480) {
		}
		CContextGL::~CContextGL() {
			if (m_Tilemap) m_Tilemap->Release();
			if (m_Drawing) m_Drawing->Release();
		}

		bool CContextGL::Initialize(const ContextProperties& properties) {
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
		void CContextGL::GetTilemap(Tilemap2d** tilemap) {
			if (m_Tilemap == nullptr)
				m_Tilemap = new CTilemap2d(this);
			m_Tilemap->AddRef();
			*tilemap = m_Tilemap;
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
		bool CreateContextGL(IContext **context, Core::IView* pView, const ContextProperties& properties) {
			*context = nullptr;
			return false;
		}
	}
}
#endif