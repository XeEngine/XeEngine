#include "pch.h"
#include <XeSDK/XeStorage.h>
#include <XeSDK/IStorageDirectory.h>
#include <XeSDK/XeDrivers.h>
#include <XeSDK/IDriversGamepadDriver.h>

#include "XeDriverInputWindowsCustom.h"
#include "XeDriverInputXInput.h"

namespace Xe {

	Xe::Drivers::Input::WindowsCustom g_DriverInputCustom;
	Xe::Drivers::Input::XInput g_DriverInputXInput;

	void RegisterInputDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverInputCustom);
		Xe::Drivers::RegisterDriver(&g_DriverInputXInput);
	}

	namespace Core
	{
		void Initialize()
		{
			RegisterInputDrivers();
		}
	}
}
