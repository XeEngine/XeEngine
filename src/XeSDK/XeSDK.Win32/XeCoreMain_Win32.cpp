#include "pch.h"
#include <XeSDK/XeStorage.h>
#include <XeSDK/IStorageDirectory.h>

#include "IDriverInputEnumerator.h"
//#include "XeDriverInputWindowsCustom.h"
#include "XeDriverInputXInput.h"

namespace Xe {

	//Xe::Driver::Input::WindowsCustom g_DriverInputCustom;
	Xe::Driver::Input::XInput g_DriverInputXInput;

	void RegisterInputDrivers()
	{
		//Xe::IO::SubscribeEnumerator(g_DriverInputCustom);
		Xe::IO::SubscribeEnumerator(g_DriverInputXInput);
	}

	namespace Core {
		void Initialize() {
			RegisterInputDrivers();
		}
	}
}
