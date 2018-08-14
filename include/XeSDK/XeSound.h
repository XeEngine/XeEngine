#pragma once
#include <XeSDK/XeError.h>
#include <XeSDK/ISound.h>

namespace Xe {
	//! \brief Audio playback, recording and mixing management.
	namespace Sound {
		enum AudioEngine {
			AudioEngine_Default,
			AudioEngine_Null,
			AudioEngine_Wasapi,
			AudioEngine_XAudio,
		};

		svar GetBitsPerSample(BitFormat format);
		void Convert(float *pDataOut, const void *pDataIn, BitFormat format, svar samplesCount);

		//! \warning DEPRECATED
		RESULT Create(IAudio **audio, AudioEngine audioEngine = AudioEngine_Default);

		bool Create(IAudio **audio, ctstring driverName);
	}
}