#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Sound {
		struct WaveDesc;
		interface IEffect : public IObject {
			virtual svar CalculateInputLength(svar bytes, const WaveDesc &waveFormat) = 0;
			virtual svar Process(float *pWaveOut, svar nLengthOut, float *pWaveIn, svar nLengthIn, const WaveDesc &waveFormat) = 0;
		};
	}
}