#include <XeSDK/XeDef.h>
#include <vector>

namespace Xe {
	namespace IO {
		struct HidDevice {
			char Path[0x100];
			u16 VendorId;
			u16 ProductId;
		};

		std::vector<HidDevice> GetHidDevices();
		std::vector<HidDevice> GetHidDevices(u16 vendorId);
		std::vector<HidDevice> GetHidDevices(u16 vendorId, u16 productId);
		std::vector<HidDevice> GetHidDevices(u16 vendorId, size_t productIdCount, const u16 *productIds);
	}
}