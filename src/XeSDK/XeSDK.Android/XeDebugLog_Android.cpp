#include "pch.h"
#include <XeSDK/XeCore.h>
#include <android/log.h>

// -llog
namespace Xe {
	namespace Core {
		void DebugPrint(ctstring str) {
			static const int PRIO[] = {
				ANDROID_LOG_DEBUG,
				ANDROID_LOG_INFO,
				ANDROID_LOG_WARN,
				ANDROID_LOG_ERROR,
				ANDROID_LOG_FATAL,
				ANDROID_LOG_FATAL,
			};
			__android_log_write(ANDROID_LOG_DEBUG, "Xe", str);
		}
	}
}