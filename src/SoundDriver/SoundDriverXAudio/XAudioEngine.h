#pragma once
#include <XeSDK/ISound.h>
#include "XAudioDefinitions.h"
#include <XeSDK/IDriversSoundDriver.h>

namespace Xe { namespace Sound {
	class XAudioEngine : public IAudio {
		typedef HRESULT(__stdcall * PFNXAUDIO2CREATE) (_Outptr_ IXAudio2** ppXAudio2,
			UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
		PFNXAUDIO2CREATE pfnXAudio2Create;

	public:
		static HRESULT Log(HRESULT hr, ctstring func, ctstring source, int line);

		XAudioEngine();
		~XAudioEngine();

		bool Initialize(const Xe::Sound::AudioInitDesc& desc);
		bool SetSampleRate(svar sampleRate);

		bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format,
			IAudioBuffer::ICallback *pCallback);

		HMODULE m_hDllAudio;
		svar m_MinVersion;
		IXAudio2 *m_pAudio;
		IXAudio2MasteringVoice *m_pMasterVoice;
	};
} }