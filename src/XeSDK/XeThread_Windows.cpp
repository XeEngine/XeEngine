#include "pch.h"
#include <XeSDK/XeThread.h>

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)

namespace Xe {
	void Thread::Sleep(u64 microsec) {
		::Sleep((DWORD)(microsec / 1000));
	}
}

#endif