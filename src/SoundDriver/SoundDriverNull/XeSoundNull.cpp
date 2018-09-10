#include "pch.h"
#include "XeSoundNull.h"
#include "DummyAudioBufferCallback.h"

namespace Xe { namespace Sound {
	AudioNull::NullBuffer::NullBuffer(const WaveDesc &format) :
		IAudioBuffer(format),
		m_pCallback(new DummyAudioBufferCallback),
		m_IsPlaying(false)
	{ }

	AudioNull::NullBuffer::~NullBuffer()
	{
		m_pCallback->Release();
	}

	void AudioNull::NullBuffer::SetCallback(IAudioBufferCallback& pCallback)
	{
		m_pCallback->Release();
		m_pCallback = &pCallback;
		m_pCallback->AddRef();
	}

	void AudioNull::NullBuffer::Submit(void *data, svar length)
	{ }

	bool AudioNull::NullBuffer::IsPlaying() const
	{
		return m_IsPlaying;
	}

	void AudioNull::NullBuffer::Play()
	{
		m_IsPlaying = true;
	}

	void AudioNull::NullBuffer::Stop()
	{
		m_IsPlaying = false;
	}

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

	bool AudioNull::CreateBuffer(IAudioBuffer **buffer, const WaveDesc &format)
	{
		*buffer = new NullBuffer(format);
		return true;
	}
} }