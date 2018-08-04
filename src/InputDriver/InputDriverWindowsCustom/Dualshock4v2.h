#pragma once
#include "Dualshock4.h"

namespace Xe {
	class GamepadDS4v2 : public GamepadDS4 {
		typedef GamepadDS4 Base;
	
	public:
		GamepadDS4v2(const Xe::IO::HidDevice& hidDevice);
	};
}