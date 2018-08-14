#include "pch.h"
#include "XeSoundNull.h"

namespace Xe { namespace Sound {
	bool AudioNull::NullBuffer::Query(IObject **obj, UID id)
	{
		switch (id) {
		case IAudioBuffer::ID:
		case IObject::ID:
			AddRef();
			*obj = nullptr;
			return true;
		}
		*obj = nullptr;
		return false;
	}
	AudioNull::NullBuffer::NullBuffer(const WaveFormat &format, ICallback *pCallback) :
		IAudioBuffer(format, pCallback)
	{ }

	void AudioNull::NullBuffer::Submit(const float *data, svar length)
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

	bool AudioNull::Query(IObject **obj, UID id)
	{
		switch (id) {
		case IAudio::ID:
		case IObject::ID:
			AddRef();
			*obj = this;
			return true;
		}
		*obj = nullptr;
		return false;
	}

	AudioNull::AudioNull()
	{ }

	bool AudioNull::SetSampleRate(svar sampleRate)
	{
		if (sampleRate % 100)
			return false;

		m_sampleRate = sampleRate;
		return true;
	}

	bool AudioNull::CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback)
	{
		*buffer = new NullBuffer(format, pCallback);
		return true;
	}
} }