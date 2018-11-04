#include "pch.h"
#include <XeSDK/XeIOGamepad.h>
#include "XeDriversInput.h"

namespace Xe { namespace IO {

	bool OpenDevice(
		Xe::IO::Gamepad** ppGamepad,
		const Xe::IO::GamepadEntry& entry)
	{
		auto& gamepadDrivers = Xe::Drivers::GetGamepadDrivers();
		for (auto it = gamepadDrivers.begin(); it != gamepadDrivers.end(); ++it)
		{
			if ((*it)->OpenDevice(ppGamepad, entry))
			{
				return true;
			}
		}

		return false;
	}

	size_t EnumerateDevices(
		Xe::IO::GamepadEntry* pEntries,
		size_t index,
		size_t count)
	{
		size_t found = 0;

		auto& gamepadDrivers = Xe::Drivers::GetGamepadDrivers();
		for (auto it = gamepadDrivers.begin();
			it != gamepadDrivers.end() && index + found < count;
			++it)
		{
			found += (*it)->EnumerateDevices(pEntries, index + found, count);
		}

		return found;
	}

} }