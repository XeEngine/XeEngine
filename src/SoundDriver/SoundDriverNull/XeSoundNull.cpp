#include "pch.h"
#include "XeSoundNull.h"

namespace Xe { namespace Sound {
	AudioNull::NullBuffer::NullBuffer(const WaveDesc &format, ICallback *pCallback) :
		IAudioBuffer(format, pCallback)
	{ }

	void AudioNull::NullBuffer::Submit(void *data, svar length)
	{ }

	void AudioNull::NullBuffer::Play()
	{ }

	void AudioNull::NullBuffer::Stop()
	{ }

	float AudioNull::NullBuffer::GetVolume()
	{
		return 1.0f;
	}

	void AudioNull::NullBuffer::SetVolume(float volume)
	{ }

	void AudioNull::NullBuffer::GetChannelVolumes(int channels, float* volumes)
	{ }

	void AudioNull::NullBuffer::SetChannelVolumes(int channels, const float* volumes)
	{ }

	AudioNull::AudioNull()
	{ }

	bool AudioNull::SetSampleRate(svar sampleRate)
	{
		if (sampleRate % 100)
			return false;

		m_SampleRate = sampleRate;
		return true;
	}

	bool AudioNull::CreateBuffer(IAudioBuffer **buffer, const WaveDesc &format, IAudioBuffer::ICallback *pCallback)
	{
		*buffer = new NullBuffer(format, pCallback);
		return true;
	}
} }