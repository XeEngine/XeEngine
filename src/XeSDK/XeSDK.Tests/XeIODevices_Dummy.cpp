#include "pch.h"
#include <XeSDK/XeIODevices.h>

namespace Xe { namespace IO {
	std::vector<HidDevice> GetHidDevices(u16 vendorId, size_t productIdCount, const u16 *productIds) { return std::vector<HidDevice>(); }
} }