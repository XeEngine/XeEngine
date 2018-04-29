#include "pch.h"
#include <XeSDK/XeIOGamepad.h>

namespace Xe {
	namespace IO {
		int QueryGamepads(GamepadEntry* pEntries, int count) {
			return 0;
		}

		GamepadResult OpenGamepad(const GamepadEntry &entry, Gamepad **ppGamepad) {
			*ppGamepad = nullptr;
			return GamepadResult ::NotSupported;
		}
	}
}
