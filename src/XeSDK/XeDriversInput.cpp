#include "pch.h"
#include "XeDriversInput.h"

namespace Xe { namespace Drivers { namespace Input {
	std::list<Xe::Drivers::Input::IGamepadDriver*> g_GamepadDrivers;

	void RegisterInputDriver(Xe::Drivers::Input::IInputDriver* gamepadDriver)
	{
		ASSERT(gamepadDriver != nullptr);
		switch (gamepadDriver->GetInputDeviceType())
		{
		case InputDeviceType_Gamepad:
			g_GamepadDrivers.push_back((Xe::Drivers::Input::IGamepadDriver*)gamepadDriver);
			break;
		}
	}

	void UnregisterInputDriver(Xe::Drivers::Input::IInputDriver* gamepadDriver)
	{
		ASSERT(gamepadDriver != nullptr);
		switch (gamepadDriver->GetInputDeviceType())
		{
		case InputDeviceType_Gamepad:
			g_GamepadDrivers.remove((Xe::Drivers::Input::IGamepadDriver*)gamepadDriver);
			break;
		}
	}

	const std::list<Xe::Drivers::Input::IGamepadDriver*>& GetGamepadDrivers()
	{
		return g_GamepadDrivers;
	}

} } }