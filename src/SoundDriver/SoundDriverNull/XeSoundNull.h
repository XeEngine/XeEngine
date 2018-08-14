#pragma once
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {
	class AudioNull : public IAudio {
	public:
		class NullBuffer : public IAudioBuffer
		{
		public:
			bool Query(IObject **obj, UID id);

			NullBuffer(const WaveFormat &format, ICallback *pCallback);

			void Submit(const float *data, svar length);
			void Play();
			void Stop();
			float GetVolume();
			void SetVolume(float volume);
			void GetChannelVolumes(int channels, float* volumes);
			void SetChannelVolumes(int channels, const float* volumes);
		};

		bool Query(IObject **obj, UID id);

		AudioNull();

		bool SetSampleRate(svar sampleRate);
		bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback);
	};
} }