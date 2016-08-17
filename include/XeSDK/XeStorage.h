#pragma once
#include <XeSDK/IStorage.h>

namespace Xe {
	namespace Storage {
		enum StorageType {
			Storage_Default,
		};
		bool GetStorage(IStorage **storage, StorageType type = Storage_Default);
	}
}