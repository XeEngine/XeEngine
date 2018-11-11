#include "pch.h"

// We need the minimum for wWinMain.
// These defines speed-up the inclusion of Windows.h
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOSERVICE
#define NOMCX
#define NOIME
#include <Windows.h>

namespace Xe {
	namespace Core {
		void Main();
	}
}

int main() {
	Xe::Core::Main();
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	Xe::Core::Main();
	return 0;
}
