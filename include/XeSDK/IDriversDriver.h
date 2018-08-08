#pragma once
#include <XeSDK/XeDriversDefinition.h>
#include <XeSDK/IObject.h>

namespace Xe { namespace Drivers {

	//! \brief Low-level driver contract to register it in the drivers chain.
	struct IDriver : public IObject
	{
		//! \brief Get the name of the driver
		virtual const char* GetDriverName() const = 0;

		//! \brief Get the driver macro-category.
		virtual DriverType GetDriverType() const = 0;
	};
} }