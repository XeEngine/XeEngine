#pragma once
#include "pch.h"
#ifdef PLATFORM_GL
#include "XeCoreView.h"
#include "ContextGL.h"

#pragma comment(lib, "Opengl32.lib")

namespace Xe {
	namespace Core {
		class IView;
	}
	namespace Graphics {
		class ContextGL_Win32 : public CContextGL {
			bool m_IsInitialized;
			VBlankSync m_Sync;
			HWND m_hWnd;
			HDC m_hDc;
			HGLRC m_hGlRc;

			bool CreateContext();
		public:
			ContextGL_Win32(Core::IView* pView);
			~ContextGL_Win32();
			bool Initialize(const ContextProperties& properties);

			void SwapBuffersSub(VBlankSync sync);
		};

		ContextGL_Win32::ContextGL_Win32(Core::IView* pView) :
			CContextGL(pView),
			m_IsInitialized(false),
			m_Sync(VBlank_Wait),
			m_hDc(nullptr),
			m_hGlRc(nullptr) {

			Core::ViewInternal view;
			Core::GetViewInternal(m_pView, view);
			m_hWnd = view.Window;
		}
		ContextGL_Win32::~ContextGL_Win32() {
			if (m_hGlRc) wglDeleteContext(m_hGlRc);
			if (m_hDc) ReleaseDC(m_hWnd, m_hDc);
		}
		bool ContextGL_Win32::Initialize(const ContextProperties& properties) {
			if (m_IsInitialized) return true;
			// Now try to create a valid GL context
			if (CreateContext(/*m_pView*/)) {
				// Then initialize the real GL stuff
				bool r = CContextGL::Initialize(properties);
				if (r) glExtSwapInterval(m_Sync);
				return r;
			}
			return false;
		}
		void ContextGL_Win32::SwapBuffersSub(VBlankSync sync) {
			if (m_Sync != sync) {
				m_Sync = sync;
				glExtSwapInterval(sync); // wglSwapIntervalEXT
			}
			::SwapBuffers(m_hDc);
		}

		bool ContextGL_Win32::CreateContext() {
			int nMyPixelFormatID;
			static const PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), // struct size
				1,                              // Version number
				PFD_DRAW_TO_WINDOW |			// Flags, draw to a window,
				PFD_DOUBLEBUFFER |
				PFD_SUPPORT_OPENGL,				// use OpenGL
				PFD_TYPE_RGBA,					// RGBA pixel values
				24,								// Pixel format used
				0, 0, 0,						// RGB bits & shift sizes.
				0, 0, 0,						// Don't care about them
				0, 0,							// No alpha buffer info
				0, 0, 0, 0, 0,					// No accumulation buffer
				24,								// bits for depth buffer
				8,								// bits fo stencil buffer
				0,								// No auxiliary buffers
				PFD_MAIN_PLANE,					// Layer type
				0,								// Reserved (must be 0)
				0,								// No layer mask
				0,								// No visible mask
				0								// No damage mask
			};
			if (m_hWnd) {
				m_hDc = GetDC(m_hWnd);
				if (m_hDc == nullptr)
					return false;
				nMyPixelFormatID = ChoosePixelFormat(m_hDc, &pfd);
				if (nMyPixelFormatID == 0)
					return false;
				if (SetPixelFormat(m_hDc, nMyPixelFormatID, &pfd) == FALSE)
					return false;

				m_hGlRc = wglCreateContext(m_hDc);
				return wglMakeCurrent(m_hDc, m_hGlRc) == TRUE;
			}
			return false;
		}

		bool CreateContextGL(IContext **context, Core::IView* pView, const ContextProperties& properties) {
			ContextGL_Win32 *tmp = new ContextGL_Win32(pView);
			if (tmp->Initialize(properties)) {
				*context = tmp;
				return true;
			}
			tmp->Release();
			return false;
		}
	}
}
#endif