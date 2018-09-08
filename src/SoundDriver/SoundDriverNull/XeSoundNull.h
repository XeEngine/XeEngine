#pragma once
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {
	class AudioNull : public IAudio {
	public:
		class NullBuffer : public IAudioBuffer
		{
		public:
			NullBuffer(const WaveDesc &format, ICallback *pCallback);

			void Submit(void *data, svar length);
			void Play();
			void Stop();
			float GetVolume();
			void SetVolume(float volume);
			void GetChannelVolumes(int channels, float* volumes);
			void SetChannelVolumes(int channels, const float* volumes);
		};

		AudioNull();

		bool SetSampleRate(svar sampleRate);
		bool CreateBuffer(IAudioBuffer **buffer, const WaveDesc &format, IAudioBuffer::ICallback *pCallback);
	};
} }