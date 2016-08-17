#include "pch.h"
#include <XeSDK/XeStorage.h>
#include <XeSDK/IStorageDirectory.h>

namespace Xe {
	namespace Core {
		void Main();
	}
}

using namespace Xe;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	Xe::Core::Main();
	return 0;
}