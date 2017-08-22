#pragma once
#include <XeSDK/ISound.h>
#include "XAudioDefinitions.h"

namespace Xe { namespace Sound {
	class XAudioEngine : public IAudio {
		typedef HRESULT(__stdcall * PFNXAUDIO2CREATE) (_Outptr_ IXAudio2** ppXAudio2,
			UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
		PFNXAUDIO2CREATE pfnXAudio2Create;

	public:
		static void LogHresult(HRESULT hr);
		static bool IsXAudioSupported();

		bool Query(IObject **obj, UID id);

		XAudioEngine();
		~XAudioEngine();

		bool Initialize();
		bool SetSampleRate(svar sampleRate);

		bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format,
			IAudioBuffer::ICallback *pCallback);

		HMODULE m_hDllAudio;
		svar m_minVersion;
		IXAudio2 *m_pAudio;
		IXAudio2MasteringVoice *m_pMasterVoice;
	};
} }