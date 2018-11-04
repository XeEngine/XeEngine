#include <XeSDK/IDriversDriver.h>
#include <XeSDK/IDriversRenderingDriver.h>
#include <XeSDK/IDriversSoundDriver.h>
#include <XeSDK/IDriversGamepadDriver.h>
#include <list>

namespace Xe { namespace Drivers {
	enum DriverTypeFilter
	{
		DriverTypeFilter_Rendering = 1 << DriverType_Rendering,
		DriverTypeFilter_Sound = 1 << DriverType_Sound,
		DriverTypeFilter_Input = 1 << DriverType_Input,
	};

	// Get all the registered drivers
	std::list<IDriver*> GetAllDrivers();

	// Register a specific driver
	void RegisterDriver(IDriver* driver);

	// Unregister a previously registered driver
	void UnregisterDriver(IDriver* driver);

	// Get all the drivers from the given driver type
	template <class TDriver>
	std::list<TDriver*> GetDrivers()
	{
		static_assert(std::is_base_of<IDriver, TDriver>::value,
			"TDriver must derive IDriver");

		std::list<TDriver*> driversFound;
		auto drivers = GetAllDrivers();

		for (auto it = drivers.begin(); it != drivers.end(); ++it)
		{
			auto driver = dynamic_cast<TDriver*>(*it);
			if (driver)
			{
				drivers.push_back(driver);
			}
		}

		return drivers;
	}

	template <>
	std::list<IRenderingDriver*> GetDrivers();

	template <>
	std::list<ISoundDriver*> GetDrivers();

	template <>
	std::list<IInputDriver*> GetDrivers();
} }