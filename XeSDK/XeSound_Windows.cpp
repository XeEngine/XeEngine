#include "pch.h"
#include <XeSDK/ISound.h>
#include <XeSDK/XeSound.h>

namespace Xe {
	namespace Sound {
		AudioEngine GetDefaultAudioEngine() {
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
			if (IsSupported(AudioEngine_XAudio))
				return AudioEngine_XAudio;
			if (IsSupported(AudioEngine_Wasapi))
				return AudioEngine_Wasapi;
			return AudioEngine_Null;
#else
			return AudioEngine_Null;
#endif
		}
	}
}