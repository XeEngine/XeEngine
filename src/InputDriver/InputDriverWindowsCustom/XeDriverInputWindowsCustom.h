#pragma once
#include "IDriverInputEnumerator.h"

namespace Xe { namespace Driver { namespace Input {

	class WindowsCustom : public Enumerator {
		bool OpenDevice(
			Xe::IO::Gamepad** ppGamepad,
			const Xe::IO::GamepadEntry& entry);

		size_t EnumerateDevices(
			Xe::IO::GamepadEntry* pEntries,
			size_t index,
			size_t count);
	};

} } }