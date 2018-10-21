#include "pch.h"
#include <XeSDK/XeThread.h>
#include <synchapi.h>

namespace Xe {
	void Thread::Sleep(u64 microsec) {
		SleepEx((DWORD)(microsec / 1000), FALSE);
	}
}
