#include "pch.h"
#include <XeSDK/XeCore.h>
#include <XeSDK/XeDebugLog.h>
#include <XeSDK/XeTimer.h>
#include <cstdio>
#include <cstdarg>
#include <wchar.h>

#if _MSC_VER < 1900
#define snprintf _snprintf
#endif

void __stdcall xe_debug_logout(Xe::Debug::Log::Priority priority, ctstring text);
Xe::Debug::Log::PFNLOGOUT xe_debug_pfnlogout = xe_debug_logout;
Xe::Debug::Log::Priority xe_debug_priority = Xe::Debug::Log::Priority_Diagnostics;

void __stdcall xe_debug_logout(Xe::Debug::Log::Priority priority, ctstring text) {
#if 0
	FILE *f = fopen("log_opengl.txt", "a");
	if (f) {
		fputs(text, f);
		fclose(f);
	}
#else
	Xe::Core::DebugPrint(text);
#endif
}

void Xe::Debug::Log::SetLevelFilter(Priority priority) {
	xe_debug_priority = priority;
}
void Xe::Debug::Log::SetTypeFilter(Type type, bool enabled) {

}
Xe::Debug::Log::PFNLOGOUT Xe::Debug::Log::GetDefaultLogOut() {
	return xe_debug_logout;
}
void Xe::Debug::Log::SetLogOut(PFNLOGOUT logout) {
	xe_debug_pfnlogout = logout;
}
void Xe::Debug::Log::Print(Priority priority, Type type, ctstring file, int line, ctstring msgfmt, ...) {
	ctstring PRE_STRING = _T("[%4.3fs] %s:%i: ");
	tchar *msg;
	va_list args;
	va_start(args, msgfmt);
	double time = Timer::Current().AsDouble();
	int n, len;
#if defined(PLATFORM_UNICODE)
	len = _snwprintf(nullptr, 0, PRE_STRING, time, file, line);
	len += _vsnwprintf(nullptr, 0, msgfmt, args);
	msg = new tchar[len + 2];
	n = _snwprintf_s(msg, len, len, PRE_STRING, time, file, line);
	n += vswprintf(msg + n, len - n + 1, msgfmt, args);
#else
	len = snprintf(nullptr, 0, PRE_STRING, time, file, line);
	len += vsnprintf(nullptr, 0, msgfmt, args);
	msg = new tchar[len + 2];
	n = snprintf(msg, len, PRE_STRING, time, file, line);
	n += vsnprintf(msg + n, len - n + 1, msgfmt, args);
#endif
	msg[n] = '\n';
	msg[n + 1] = '\0';
	va_end(args);
	xe_debug_pfnlogout(priority, msg);
	delete[] msg;
}