#include "pch.h"
#include "XeDriversInput.h"

namespace Xe { namespace Drivers {
	std::list<Xe::Drivers::IGamepadDriver*> g_GamepadDrivers;

	void RegisterInputDriver(Xe::Drivers::IInputDriver* gamepadDriver)
	{
		ASSERT(gamepadDriver != nullptr);
		switch (gamepadDriver->GetInputDeviceType())
		{
		case InputDeviceType_Gamepad:
			g_GamepadDrivers.push_back((Xe::Drivers::IGamepadDriver*)gamepadDriver);
			break;
		}
	}

	void UnregisterInputDriver(Xe::Drivers::IInputDriver* gamepadDriver)
	{
		ASSERT(gamepadDriver != nullptr);
		switch (gamepadDriver->GetInputDeviceType())
		{
		case InputDeviceType_Gamepad:
			g_GamepadDrivers.remove((Xe::Drivers::IGamepadDriver*)gamepadDriver);
			break;
		}
	}

	const std::list<Xe::Drivers::IGamepadDriver*>& GetGamepadDrivers()
	{
		return g_GamepadDrivers;
	}

} }