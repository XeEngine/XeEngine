#pragma once
#include <XeSDK/ICoreView.h>

#if defined(PLATFORM_WIN32)
#include <windef.h>
#elif defined(PLATFORM_WINRT)
#include <Unknwn.h>
#endif

namespace Xe { namespace Core {
	struct ViewInternal {
#if defined(PLATFORM_WIN32)
		HWND Window;
#elif defined(PLATFORM_WINRT)
		IUnknown* Window;
#endif
	};
	void GetViewInternal(IView* pView, ViewInternal& viewInternal);
} }