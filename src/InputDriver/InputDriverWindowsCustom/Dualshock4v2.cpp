#include "pch.h"
#include "Dualshock4v2.h"

namespace Xe {
	GamepadDS4v2::GamepadDS4v2(const Xe::IO::HidDevice& hidDevice) :
		Base(hidDevice)
	{
		LOGI("Found DualShock 4 gen.2, following original DS4 configuration.\n");
	}
}