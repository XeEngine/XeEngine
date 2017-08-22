#include "pch.h"
#include <XeSDK/ISound.h>
#include <XeSDK/XeDebugLog.h>
#include "XAudioEngine.h"

namespace Xe {
	namespace Sound {

		bool IsXAudioSupported() {
#if defined(PLATFORM_WIN32)
			static const ctstring XAudio2Vers[] = {
				_T("XAudio2_9.dll"),
				_T("XAudio2_8.dll"),
				_T("XAudio2_7.dll"),
				_T("XAudio2_6.dll"),
				_T("XAudio2_5.dll"),
				_T("XAudio2_4.dll"),
				/*_T("XAudio2_3.dll"),
				_T("XAudio2_2.dll"),
				_T("XAudio2_1.dll"),
				_T("XAudio2_0.dll"),*/
			};
			for (svar i = 0; i < lengthof(XAudio2Vers); i++) {
				HMODULE h = LoadLibraryEx(XAudio2Vers[i], NULL, DONT_RESOLVE_DLL_REFERENCES);
				if (h != 0) {
					FreeLibrary(h);
					return true;
				}
			}
			return false;
#elif defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
			return true;
#endif
			return false;
		}

		bool CreateXAudio(IAudio **pAudio) {

#ifdef AUDIO_XAUDIO2

			XAudioEngine *pXAudio2 = new XAudioEngine;
			if (pXAudio2->Initialize()) {
				*pAudio = pXAudio2;
				return true;
			}

#endif
			*pAudio = nullptr;
			return false;
		}
	}
}