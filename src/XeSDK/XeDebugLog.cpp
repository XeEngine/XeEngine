#include "pch.h"
#include <XeSDK/XeTimer.h>
#include "XeDebugLog.h"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif

namespace Xe { namespace Debug {
	void Log::Print(Priority priority, Type type, ctstring file, int line, ctstring msgfmt, ...) {
		ctstring PRE_STRING = _T("[%4.3fs] %s:%i: ");
		tchar *msg;
		va_list args, argsCopy;
		va_start(args, msgfmt);
		va_copy(argsCopy, args);
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
		len += vsnprintf(nullptr, 0, msgfmt, argsCopy);
		va_end(argsCopy);
		msg = new tchar[len + 2];
		n = snprintf(msg, len, PRE_STRING, time, file, line);
		n += vsnprintf(msg + n, len - n + 1, msgfmt, args);
#endif
		msg[n] = '\n';
		msg[n + 1] = '\0';
		va_end(args);

		Logger::LogLevel level;
		switch (priority) {
		case Priority_Diagnostics:
			level = Logger::LogLevel_Debug;
			break;
		case Priority_Info:
			level = Logger::LogLevel_Info;
			break;
		case Priority_Warning:
			level = Logger::LogLevel_Warning;
			break;
		case Priority_Error:
			level = Logger::LogLevel_Error;
			break;
		case Priority_Critical:
			level = Logger::LogLevel_Critical;
			break;
		case Priority_Assertion:
			level = Logger::LogLevel_Critical;
			break;
		}
		Logger::Instance.Log(level, msg);
		delete[] msg;
	}
} }