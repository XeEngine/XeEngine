#include "pch.h"
#include "XAudioCompatibilityLayer.h"

#ifdef AUDIO_XAUDIO2

CXAudioPre28::CXAudioPre28(IXAudio2_Pre28 *pAudio) :
	m_pAudio(pAudio) {
	m_pAudio->AddRef();
}
CXAudioPre28::~CXAudioPre28() {
	m_pAudio->Release();
}

HRESULT CXAudioPre28::QueryInterface(REFIID riid, void** ppvInterface) {
	if (!ppvInterface)
		return E_INVALIDARG;
	if (riid == __uuidof(IXAudio2)) {
		*ppvInterface = (LPVOID)this;
		AddRef();
		return NOERROR;
	}
	return m_pAudio->QueryInterface(riid, ppvInterface);
}

ULONG CXAudioPre28::AddRef(void) {
	return  m_pAudio->AddRef();
}

ULONG CXAudioPre28::Release(void) {
	ULONG r = m_pAudio->Release();
	if (r == 0)
		delete this;
	return r;
}


HRESULT CXAudioPre28::RegisterForCallbacks(IXAudio2EngineCallback* pCallback) {
	return m_pAudio->RegisterForCallbacks(pCallback);
}

void CXAudioPre28::UnregisterForCallbacks(IXAudio2EngineCallback* pCallback) {
	m_pAudio->UnregisterForCallbacks(pCallback);
}

HRESULT CXAudioPre28::CreateSourceVoice(IXAudio2SourceVoice** ppSourceVoice,
	const WAVEFORMATEX* pSourceFormat,
	UINT32 Flags,
	float MaxFrequencyRatio,
	IXAudio2VoiceCallback* pCallback,
	const XAUDIO2_VOICE_SENDS* pSendList,
	const XAUDIO2_EFFECT_CHAIN* pEffectChain) {
	return m_pAudio->CreateSourceVoice(ppSourceVoice, pSourceFormat);
}

HRESULT CXAudioPre28::CreateSubmixVoice(IXAudio2SubmixVoice** ppSubmixVoice,
	UINT32 InputChannels, UINT32 InputSampleRate,
	UINT32 Flags, UINT32 ProcessingStage,
	const XAUDIO2_VOICE_SENDS* pSendList,
	const XAUDIO2_EFFECT_CHAIN* pEffectChain) {
	return m_pAudio->CreateSubmixVoice(ppSubmixVoice, InputChannels, InputSampleRate, Flags,
		ProcessingStage, pSendList, pEffectChain);
}

HRESULT CXAudioPre28::CreateMasteringVoice(IXAudio2MasteringVoice** ppMasteringVoice,
	UINT32 InputChannels,
	UINT32 InputSampleRate,
	UINT32 Flags, LPCWSTR szDeviceId,
	const XAUDIO2_EFFECT_CHAIN* pEffectChain,
	AUDIO_STREAM_CATEGORY StreamCategory) {
	// TODO convert szDeviceId to DeviceIndex
	UINT32 DeviceIndex = 0;
	return m_pAudio->CreateMasteringVoice(ppMasteringVoice, InputChannels, InputSampleRate,
		Flags, DeviceIndex, pEffectChain);
}

HRESULT CXAudioPre28::StartEngine(void) {
	return m_pAudio->StartEngine();
}

void CXAudioPre28::StopEngine(void) {
	m_pAudio->StopEngine();
}

HRESULT CXAudioPre28::CommitChanges(UINT32 OperationSet) {
	return m_pAudio->CommitChanges(OperationSet);
}

void CXAudioPre28::GetPerformanceData(XAUDIO2_PERFORMANCE_DATA* pPerfData) {
	m_pAudio->GetPerformanceData(pPerfData);
}

void CXAudioPre28::SetDebugConfiguration(const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration,
	void* pReserved) {
	m_pAudio->SetDebugConfiguration(pDebugConfiguration);
}

#endif