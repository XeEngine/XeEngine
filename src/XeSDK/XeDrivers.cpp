#include "pch.h"
#include <XeSDK/XeDrivers.h>
#include "XeDriversInput.h"
#include <algorithm>

namespace Xe { namespace Drivers {

	std::list<IDriver*> g_Drivers;

	void RegisterDriver(IDriver* driver)
	{
		ASSERT(driver != nullptr);
		driver->AddRef();
		g_Drivers.push_back(driver);

		switch (driver->GetDriverType())
		{
		case DriverType_Rendering:
			break;
		case DriverType_Sound:
			break;
		case DriverType_Input:
			ASSERT(dynamic_cast<Xe::Drivers::Input::IInputDriver*>(driver));
			Xe::Drivers::Input::RegisterInputDriver((Xe::Drivers::Input::IInputDriver*)driver);
			break;
		}
	}

	void UnregisterDriver(IDriver* driver)
	{
		ASSERT(driver != nullptr);
#ifdef DEVELOPMENT
		bool found = (std::find(g_Drivers.begin(), g_Drivers.end(), driver) != g_Drivers.end());
		ASSERT(found);
#endif
		g_Drivers.remove(driver);
		driver->Release();

		switch (driver->GetDriverType())
		{
		case DriverType_Rendering:
			break;
		case DriverType_Sound:
			break;
		case DriverType_Input:
			ASSERT(dynamic_cast<Xe::Drivers::Input::IInputDriver*>(driver));
			Xe::Drivers::Input::UnregisterInputDriver((Xe::Drivers::Input::IInputDriver*)driver);
			break;
		}
	}

	std::list<IDriver*> GetDrivers(DriverTypeFilter filter)
	{
		std::list<IDriver*> drivers;
		for (auto it = g_Drivers.begin(); it != g_Drivers.end(); ++it)
		{
			if ((1 << (*it)->GetDriverType()) & filter)
			{
				drivers.push_back(*it);
			}
		}

		return drivers;
	}
}}