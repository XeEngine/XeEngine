#pragma once
#include "XeDriversInput.h"
#include <XeSDK/IDriversGamepadDriver.h>
#include <list>

namespace Xe { namespace Drivers {
	void RegisterInputDriver(Xe::Drivers::IInputDriver* gamepadDriver);

	void UnregisterInputDriver(Xe::Drivers::IInputDriver* gamepadDriver);

	const std::list<Xe::Drivers::IGamepadDriver*>& GetGamepadDrivers();
} }