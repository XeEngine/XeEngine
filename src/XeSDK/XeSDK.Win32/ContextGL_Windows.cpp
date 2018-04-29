#include "pch.h"
#include "ContextGL_Common.h"

#if defined(PLATFORM_GL)

svar Xe::Graphics::CContextGLCommon::GetFormatsCount() {
	HWND hWnd = GetDesktopWindow();
	HDC hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL, PFD_TYPE_RGBA;
	svar count = DescribePixelFormat(hDC, 1, 0, NULL);
	ReleaseDC(hWnd, hDC);
	return count;
}
svar Xe::Graphics::CContextGLCommon::GetFormatsArray(Format *format, svar maxcount) {
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
bool Xe::Graphics::CContextGLCommon::SetFormat(const Format &format) {
	return false;
}

#endif