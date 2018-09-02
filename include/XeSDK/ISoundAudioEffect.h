#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Sound {
		struct WaveFormat;
		interface IEffect : public IObject {
			virtual svar CalculateInputLength(svar bytes, const WaveFormat &waveFormat) = 0;
			virtual svar Process(float *pWaveOut, svar nLengthOut, float *pWaveIn, svar nLengthIn, const WaveFormat &waveFormat) = 0;
		};
	}
}