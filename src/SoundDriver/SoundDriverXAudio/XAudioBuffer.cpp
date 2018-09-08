#include "pch.h"
#include <XeSDK/XeSound.h>
#include "XAudioBuffer.h"
#include "XAudioEngine.h"

#ifndef WAVE_FORMAT_XMA2
#define WAVE_FORMAT_XMA2 0x166
#endif

namespace Xe { namespace Sound {

	WORD FORMAT_TAGS[]
	{
		WAVE_FORMAT_UNKNOWN, // WaveType_Unknown
		WAVE_FORMAT_PCM, // WaveType_Pcm
		WAVE_FORMAT_ADPCM, // WaveType_Adpcm
		WAVE_FORMAT_WMAUDIO2, // WaveType_Wma2
		WAVE_FORMAT_WMAUDIO3, // WaveType_Wma3
		WAVE_FORMAT_XMA2, // WaveType_Xma2
	};


	XAudioBuffer::XAudioBuffer(XAudioEngine *pAudio, const WaveDesc &format, ICallback *pCallback) :
		IAudioBuffer(format, pCallback),
		m_pAudio(pAudio),
		m_voiceCallback(this, pCallback),
		m_xabuffer({ 0 }) {
		m_pAudio->AddRef();
	}

	XAudioBuffer::~XAudioBuffer() {
		if (m_pSourceVoice) {
			m_pSourceVoice->FlushSourceBuffers();
			m_pSourceVoice->DestroyVoice();
		}
		m_pAudio->Release();
	}


	bool XAudioBuffer::Initialize() {
		const WaveDesc &format = GetFormat();

		WORD waveFmt;
		if (format.WaveFormat >= 0 && format.WaveFormat < sizeof(FORMAT_TAGS))
		{
			waveFmt = FORMAT_TAGS[format.WaveFormat];
			
			if (format.BitDepth == BitFormat_F32 && format.WaveFormat == WaveType_Pcm)
			{
				waveFmt = WAVE_FORMAT_IEEE_FLOAT;
			}
		}

		WAVEFORMATEX wf =
		{
			waveFmt,
			(WORD)format.NumberOfChannels,
			(DWORD)format.SampleRate,
			(DWORD)(format.SampleRate * format.SampleLength),
			(WORD)format.SampleLength,
			(WORD)GetBitsPerSample(format.BitDepth),
			sizeof(WAVEFORMATEX)
		};

		HRESULT hr = m_pAudio->m_pAudio->CreateSourceVoice(&m_pSourceVoice, &wf,
			0, XAUDIO2_DEFAULT_FREQ_RATIO, m_pCallback ? &m_voiceCallback : nullptr);
		if (FAILED(hr))
		{
			switch (hr)
			{
			case XAUDIO2_E_INVALID_CALL:
				LOGE("The specified WaveDesc is not valid or incompatible.");
				break;
			default:
				LOGE("Unknown error (hr = %08X)", hr);
				break;
			}

			return false;
		}

		m_xabuffer.PlayBegin = 0;
		m_xabuffer.PlayLength = 0;
		return true;
	}

	void XAudioBuffer::Play() {
		m_pSourceVoice->Start();
	}

	void XAudioBuffer::Stop() {
		m_pSourceVoice->Stop();
	}

	void XAudioBuffer::Submit(void* data, svar length) {
		m_xabuffer.pAudioData = (u8*)data;
		m_xabuffer.AudioBytes = length;
		HRESULT hr = m_pSourceVoice->SubmitSourceBuffer(&m_xabuffer);
	}

	float XAudioBuffer::GetVolume() {
		float volume;
		m_pSourceVoice->GetVolume(&volume);
		return volume;
	}

	void XAudioBuffer::SetVolume(float volume) {
		m_pSourceVoice->SetVolume(volume);
	}

	void XAudioBuffer::GetChannelVolumes(int channels, float* volumes) {
		m_pSourceVoice->GetChannelVolumes(channels, volumes);
	}

	void XAudioBuffer::SetChannelVolumes(int channels, const float* volumes) {
		m_pSourceVoice->SetChannelVolumes(channels, volumes);
	}

} }