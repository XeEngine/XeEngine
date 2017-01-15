#include "pch.h"
#include <cstdio>
#include <XeSDK/XeCore.h>
#include <XeSDK/XeMemory.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 6
#define VERSION_BUILD 4
#define ENGINE_NAME _T("XeEngine")
#define ENGINE_DEVELOPER _T("Luciano Ciccariello (Xeeynamo)")
#define ENGINE_VERSION _T("0.6.6")

namespace Xe {
	namespace Core {
		void GetEngineInfo(EngineInfo& properties)
		{
			static const tchar DATETIME_YYYYMMDD[] = {
				__DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], '/',
				__DATE__[0], __DATE__[1], __DATE__[2], '/',
				__DATE__[4], __DATE__[5], ' ',
				__TIME__[0], __TIME__[1], ':',
				__TIME__[3], __TIME__[4], ':',
				__TIME__[6], __TIME__[7], '\0',
			};
			static const EngineInfo ENGINE = {
				ENGINE_NAME,
				ENGINE_DEVELOPER,
				ENGINE_VERSION,
				DATETIME_YYYYMMDD,
				0, 6, 6
			};
			properties = ENGINE;
		}
		int Xe_vsnprintf(tchar* str, int maxlen, const tchar *fmt, va_list args) {
#ifdef PLATFORM_UNICODE
			return _vsnwprintf(str, maxlen, fmt, args);
#else
			return vsnprintf(str, maxlen, fmt, args);
#endif
		}
		void DebugPrintFormat(ctstring str, ...) {
			va_list args;
			va_start(args, str);

			tchar buf[0x100];
			int len = Xe_vsnprintf(nullptr, 0, str, args);
			if (len >= lengthof(buf)) {
				tchar *heapbuf = (tchar*)Memory::Alloc((len + 1) * sizeof(tchar));
				Xe_vsnprintf(heapbuf, len + 1, str, args);
				DebugPrint(heapbuf);
				Memory::Free(heapbuf);
			}
			else {
				Xe_vsnprintf(buf, lengthof(buf), str, args);
				DebugPrint(buf);
			}
			va_end(args);
		}
	}
}