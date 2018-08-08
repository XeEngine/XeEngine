#pragma once
#include <XeSDK/IDriversGamepadDriver.h>

namespace Xe { namespace Drivers { namespace Input {

	class WindowsCustom : public IGamepadDriver {

		// Inherited from IDriver
		const char* GetDriverName() const;
		DriverType GetDriverType() const;

		// Inherited from IInputDriver
		InputDeviceType GetInputDeviceType() const;

		// Inherited from IGamepadDriver
		bool OpenDevice(
			Xe::IO::Gamepad** ppGamepad,
			const Xe::IO::GamepadEntry& entry);

		size_t EnumerateDevices(
			Xe::IO::GamepadEntry* pEntries,
			size_t index,
			size_t count);
	};

} } }