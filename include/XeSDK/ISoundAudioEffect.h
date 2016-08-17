#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Sound {
		struct WaveFormat;
		class IEffect : public IObject {
		public:
			static const UID ID = 0xb7fd310af5404c47ULL;

			virtual svar CalculateInputLength(svar bytes, const WaveFormat &waveFormat) = 0;
			virtual svar Process(float *pWaveOut, svar nLengthOut, float *pWaveIn, svar nLengthIn, const WaveFormat &waveFormat) = 0;
		};
	}
}