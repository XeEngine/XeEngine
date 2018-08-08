#include <XeSDK/IDriversDriver.h>
#include <list>

namespace Xe { namespace Drivers {
	enum DriverTypeFilter
	{
		DriverTypeFilter_Rendering = 1 << DriverType_Rendering,
		DriverTypeFilter_Sound = 1 << DriverType_Sound,
		DriverTypeFilter_Input = 1 << DriverType_Input,
	};

	void RegisterDriver(IDriver* driver);

	void UnregisterDriver(IDriver* driver);

	std::list<IDriver*> GetDrivers(DriverTypeFilter filter);
} }