#pragma once

#ifdef PLATFORM_WIN32

#include "ContextGL.h"
#include <Windows.h>

namespace Xe { namespace Graphics {
	class ContextGL_Win32 : public CContextGL
	{
		bool m_IsInitialized;
		VBlankSync m_Sync;
		HWND m_hWnd;
		HDC m_hDc;
		HGLRC m_hGlRc;

		bool CreateContext();

	public:
		ContextGL_Win32(Xe::Core::IFrameView* pView);
		~ContextGL_Win32();

		bool Initialize(const ContextInitDesc& properties);
		void SwapBuffersSub(VBlankSync sync);

		svar GetFormatsCount() const;
		svar GetFormatsArray(Format *format, svar maxcount) const;
		bool SetFormat(const Format &format) const;
	};
} }
#endif