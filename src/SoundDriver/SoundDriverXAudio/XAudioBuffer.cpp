#include "pch.h"
#include "XAudioBuffer.h"
#include "XAudioEngine.h"

namespace Xe { namespace Sound {

	bool XAudioBuffer::Query(IObject **obj, UID id) {
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

	XAudioBuffer::XAudioBuffer(XAudioEngine *pAudio, const WaveFormat &format, ICallback *pCallback) :
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
		const WaveFormat &format = GetFormat();

		WAVEFORMATEX wf = { WAVE_FORMAT_IEEE_FLOAT, (WORD)format.NumberOfChannels, (DWORD)format.SampleRate,
			(DWORD)(format.SampleRate * format.SampleLength), (WORD)format.SampleLength, (WORD)(sizeof(float) * 8), sizeof(WAVEFORMATEX) };

		HRESULT hr = m_pAudio->m_pAudio->CreateSourceVoice(&m_pSourceVoice, &wf,
			0, XAUDIO2_DEFAULT_FREQ_RATIO, m_pCallback ? &m_voiceCallback : nullptr);

		if (!SUCCEEDED(hr))
			return false;

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

	void XAudioBuffer::Submit(const float *data, svar length) {
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