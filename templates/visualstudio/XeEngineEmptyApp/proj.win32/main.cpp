#include "pch.h"

// We need the minimum for wWinMain.
// The following macros speed-up the inclusion of Windows.h
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOSERVICE
#define NOMCX
#define NOIME
#include <Windows.h>

namespace Xe { namespace Core {
	void Main();
} }

int main()
{
	// Enable console logging
	Xe::ConsoleLogHandler consoleLogHandler;
	Xe::Logger::GetLogEvent() += &consoleLogHandler;
	
	// Invoke app entrypoint
	Xe::Core::Main();

	return 0;
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	return main();
}