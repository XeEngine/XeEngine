#pragma once
#include "XAudioDefinitions.h"

#ifdef AUDIO_XAUDIO2
// COMPATIBILITY LAYER BETWEEN <2.8 AND >=2.8

#undef INTERFACE
#define INTERFACE IXAudio2_Pre28
DECLARE_INTERFACE_(IXAudio2_Pre28, IUnknown)
{
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, __deref_out void** ppvInterface) PURE;
	STDMETHOD_(ULONG, AddRef) (THIS) PURE;
	STDMETHOD_(ULONG, Release) (THIS) PURE;

	STDMETHOD(GetDeviceCount) (THIS_ __out UINT32* pCount) PURE;
	STDMETHOD(GetDeviceDetails) (THIS_ UINT32 Index, __out XAUDIO2_DEVICE_DETAILS* pDeviceDetails) PURE;

	STDMETHOD(Initialize) (THIS_ UINT32 Flags X2DEFAULT(0),
		XAUDIO2_PROCESSOR XAudio2Processor X2DEFAULT(XAUDIO2_DEFAULT_PROCESSOR)) PURE;
	STDMETHOD(RegisterForCallbacks) (__in IXAudio2EngineCallback* pCallback) PURE;
	STDMETHOD_(void, UnregisterForCallbacks) (__in IXAudio2EngineCallback* pCallback) PURE;
	STDMETHOD(CreateSourceVoice) (THIS_ __deref_out IXAudio2SourceVoice** ppSourceVoice,
		__in const WAVEFORMATEX* pSourceFormat,
		UINT32 Flags X2DEFAULT(0),
		float MaxFrequencyRatio X2DEFAULT(XAUDIO2_DEFAULT_FREQ_RATIO),
		__in_opt IXAudio2VoiceCallback* pCallback X2DEFAULT(NULL),
		__in_opt const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
		__in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) PURE;
	STDMETHOD(CreateSubmixVoice) (THIS_ __deref_out IXAudio2SubmixVoice** ppSubmixVoice,
		UINT32 InputChannels, UINT32 InputSampleRate,
		UINT32 Flags X2DEFAULT(0), UINT32 ProcessingStage X2DEFAULT(0),
		__in_opt const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
		__in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) PURE;
	STDMETHOD(CreateMasteringVoice) (THIS_ __deref_out IXAudio2MasteringVoice** ppMasteringVoice,
		UINT32 InputChannels X2DEFAULT(XAUDIO2_DEFAULT_CHANNELS),
		UINT32 InputSampleRate X2DEFAULT(XAUDIO2_DEFAULT_SAMPLERATE),
		UINT32 Flags X2DEFAULT(0), UINT32 DeviceIndex X2DEFAULT(0),
		__in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) PURE;
	STDMETHOD(StartEngine) (THIS) PURE;
	STDMETHOD_(void, StopEngine) (THIS) PURE;
	STDMETHOD(CommitChanges) (THIS_ UINT32 OperationSet) PURE;
	STDMETHOD_(void, GetPerformanceData) (THIS_ __out XAUDIO2_PERFORMANCE_DATA* pPerfData) PURE;
	STDMETHOD_(void, SetDebugConfiguration) (THIS_ __in_opt const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration,
		__in_opt __reserved void* pReserved X2DEFAULT(NULL)) PURE;
};

class CXAudioPre28 : public IXAudio2 {
	IXAudio2_Pre28 *m_pAudio;
public:
	CXAudioPre28(IXAudio2_Pre28 *pAudio);

	~CXAudioPre28();

	#pragma region IUnknown

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Outptr_ void** ppvInterface);

	__declspec(nothrow) ULONG STDMETHODCALLTYPE AddRef(void);

	__declspec(nothrow) ULONG STDMETHODCALLTYPE Release(void);

	#pragma endregion

	#pragma region IXAudio2

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE RegisterForCallbacks(_In_ IXAudio2EngineCallback* pCallback);

	__declspec(nothrow) void STDMETHODCALLTYPE UnregisterForCallbacks(_In_ IXAudio2EngineCallback* pCallback);

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CreateSourceVoice(_Outptr_ IXAudio2SourceVoice** ppSourceVoice,
		_In_ const WAVEFORMATEX* pSourceFormat,
		UINT32 Flags X2DEFAULT(0),
		float MaxFrequencyRatio X2DEFAULT(XAUDIO2_DEFAULT_FREQ_RATIO),
		_In_opt_ IXAudio2VoiceCallback* pCallback X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL));

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CreateSubmixVoice(_Outptr_ IXAudio2SubmixVoice** ppSubmixVoice,
		UINT32 InputChannels, UINT32 InputSampleRate,
		UINT32 Flags X2DEFAULT(0), UINT32 ProcessingStage X2DEFAULT(0),
		_In_opt_ const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL));

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CreateMasteringVoice(_Outptr_ IXAudio2MasteringVoice** ppMasteringVoice,
		UINT32 InputChannels X2DEFAULT(XAUDIO2_DEFAULT_CHANNELS),
		UINT32 InputSampleRate X2DEFAULT(XAUDIO2_DEFAULT_SAMPLERATE),
		UINT32 Flags X2DEFAULT(0), _In_opt_z_ LPCWSTR szDeviceId X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL),
		_In_ AUDIO_STREAM_CATEGORY StreamCategory X2DEFAULT(AudioCategory_GameEffects));

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE StartEngine(void);

	__declspec(nothrow) void STDMETHODCALLTYPE StopEngine(void);

	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CommitChanges(UINT32 OperationSet);

	__declspec(nothrow) void STDMETHODCALLTYPE GetPerformanceData(_Out_ XAUDIO2_PERFORMANCE_DATA* pPerfData);

	__declspec(nothrow) void STDMETHODCALLTYPE SetDebugConfiguration(_In_opt_ const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration,
		_Reserved_ void* pReserved X2DEFAULT(NULL));

	#pragma endregion
};


#endif