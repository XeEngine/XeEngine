#pragma once
#include "XeDriversInput.h"
#include <XeSDK/IDriversGamepadDriver.h>
#include <list>

namespace Xe { namespace Drivers { namespace Input {
	void RegisterInputDriver(Xe::Drivers::Input::IInputDriver* gamepadDriver);

	void UnregisterInputDriver(Xe::Drivers::Input::IInputDriver* gamepadDriver);

	const std::list<Xe::Drivers::Input::IGamepadDriver*>& GetGamepadDrivers();
} } }