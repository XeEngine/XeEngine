#include "pch.h"
#include <XeSDK/XeCore.h>

namespace Xe {
	namespace Core {
		void DebugPrint(ctstring str) {
			if (IsDebuggerPresent()) {
				OutputDebugString(str);
				return;
			}
		}
	}
}