#include "pch.h"
#include <XeSDK/XeDrivers.h>
#include "XeDriversInput.h"
#include <algorithm>

namespace Xe { namespace Drivers {

	std::list<IDriver*> g_Drivers;

	std::list<IDriver*> GetAllDrivers()
	{
		return g_Drivers;
	}

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
			ASSERT(dynamic_cast<Xe::Drivers::IInputDriver*>(driver));
			Xe::Drivers::RegisterInputDriver((Xe::Drivers::IInputDriver*)driver);
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
			ASSERT(dynamic_cast<Xe::Drivers::IInputDriver*>(driver));
			Xe::Drivers::UnregisterInputDriver((Xe::Drivers::IInputDriver*)driver);
			break;
		}
	}

	template <class TDriver>
	std::list<TDriver*> InternalGetDrivers(DriverTypeFilter filter)
	{
		std::list<TDriver*> drivers;
		for (auto it = g_Drivers.begin(); it != g_Drivers.end(); ++it)
		{
			if ((1 << (*it)->GetDriverType()) & filter)
			{
				drivers.push_back(static_cast<TDriver*>(*it));
			}
		}

		return drivers;
	}

	template <>
	std::list<IRenderingDriver*> GetDrivers()
	{
		return InternalGetDrivers<IRenderingDriver>(DriverTypeFilter_Rendering);
	}

	template <>
	std::list<ISoundDriver*> GetDrivers()
	{
		return InternalGetDrivers<ISoundDriver>(DriverTypeFilter_Sound);
	}

	template <>
	std::list<IInputDriver*> GetDrivers()
	{
		return InternalGetDrivers<IInputDriver>(DriverTypeFilter_Input);
	}
}}