#include "pch.h"

#ifdef PLATFORM_WIN32

#include "ContextGL.Win32.h"
#pragma comment(lib, "Opengl32.lib")

using namespace Xe::Graphics;

ContextGL_Win32::ContextGL_Win32(Xe::Core::IFrameView* pView) :
	CContextGL(pView),
	m_IsInitialized(false),
	m_Sync(VBlank_Wait),
	m_hWnd(nullptr),
	m_hDc(nullptr),
	m_hGlRc(nullptr)
{ }

ContextGL_Win32::~ContextGL_Win32()
{
	if (m_hGlRc) wglDeleteContext(m_hGlRc);
	if (m_hDc) ReleaseDC(m_hWnd, m_hDc);
}

bool ContextGL_Win32::Initialize(const ContextInitDesc& properties)
{
	ASSERT(properties.FrameView != nullptr);
	if (m_IsInitialized) return true;

	m_hWnd = (HWND)properties.FrameView->GetSystemWindow();

	// Now try to create a valid GL context
	if (CreateContext(/*m_pView*/)) {
		// Then initialize the real GL stuff
		bool r = CContextGL::Initialize(properties);
		if (r)
		{
			glExtSwapInterval(m_Sync);
			CHECKERROR;
		}

		CHECKERROR;
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


svar ContextGL_Win32::GetFormatsCount() const
{
	HWND hWnd = GetDesktopWindow();
	HDC hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL, PFD_TYPE_RGBA;
	svar count = DescribePixelFormat(hDC, 1, 0, NULL);
	ReleaseDC(hWnd, hDC);
	return count;
}

svar ContextGL_Win32::GetFormatsArray(Format *format, svar maxcount) const
{
	HWND hWnd = GetDesktopWindow();
	HDC hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL, PFD_TYPE_RGBA;
	int count = DescribePixelFormat(hDC, 1, 0, &pfd);

	for (int i = 1; i < count; i++) {
		Format *pFormat = format + i;
		if (DescribePixelFormat(hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
			break;
		pFormat->id = (u32)i;
		pFormat->r = pfd.cRedBits;
		pFormat->g = pfd.cGreenBits;
		pFormat->b = pfd.cBlueBits;
		pFormat->a = pfd.cAlphaBits;
		pFormat->depth = pfd.cDepthBits;
		pFormat->stencil = pfd.cStencilBits;
	}
	ReleaseDC(hWnd, hDC);
	return count;
}

bool ContextGL_Win32::SetFormat(const Format &format) const
{
	return false;
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

	if (m_hWnd)
	{
		m_hDc = GetDC(m_hWnd);
		if (m_hDc == nullptr)
			return false;
		nMyPixelFormatID = ChoosePixelFormat(m_hDc, &pfd);
		if (nMyPixelFormatID == 0)
			return false;
		if (SetPixelFormat(m_hDc, nMyPixelFormatID, &pfd) == FALSE)
			return false;

		HGLRC tempContext = wglCreateContext(m_hDc);
		wglMakeCurrent(m_hDc, tempContext);
		LoadLibraries();
		if (wglCreateContextAttribsARB)
		{
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
			// Try to use the OpenGL 3.2 extension
			const int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 2,
#ifdef DEVELOPMENT
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#else
				WGL_CONTEXT_FLAGS_ARB, 0,
#endif
				0
			};

			LOGD("WGL_ARB_create_context found");
			m_hGlRc = wglCreateContextAttribsARB(m_hDc, 0, attribs);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempContext);
		}
		else
		{
			// Not supported, falling to the old implementation
			m_hGlRc = tempContext;
		}

		return wglMakeCurrent(m_hDc, m_hGlRc) == TRUE;
	}
	return false;
}
#endif