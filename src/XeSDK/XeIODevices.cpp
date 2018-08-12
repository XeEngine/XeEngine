#include "pch.h"
#include <XeSDK/XeIODevices.h>

namespace Xe { namespace IO {
	std::vector<HidDevice> GetHidDevices() {
		return GetHidDevices(0);
	}
	std::vector<HidDevice> GetHidDevices(u16 vendorId) {
		u16 dummy = 0;
		return GetHidDevices(vendorId, 0, &dummy);
	}
	std::vector<HidDevice> GetHidDevices(u16 vendorId, u16 productId) {
		return GetHidDevices(vendorId, 1, &productId);
	}
} }