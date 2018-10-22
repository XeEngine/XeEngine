#pragma once
#include <XeSDK/IDriversGamepadDriver.h>

namespace Xe { namespace Drivers { namespace Input {
	class DefaultGamepadEventHandler : public IGamepadEventHandler
	{
		void OnGamepadAttached(const Xe::IO::GamepadEntry& entry) { }
		void OnGamepadDeattached(const Xe::IO::GamepadEntry& entry) { }
	};
} } }