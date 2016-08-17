#include "pch.h"
#include <XeSDK/ISound.h>
#include <XeSDK/XeDebugLog.h>

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
#include <xaudio2.h>
#include <basetyps.h>

// FROM COMDECL.H
#define DECLSPEC_UUID_WRAPPER(x) __declspec(uuid(#x))
#ifdef INITGUID

#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            class DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) className; \
            EXTERN_C const GUID DECLSPEC_SELECTANY CLSID_##className = __uuidof(className)

#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            interface DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) interfaceName; \
            EXTERN_C const GUID DECLSPEC_SELECTANY IID_##interfaceName = __uuidof(interfaceName)

#else // INITGUID

#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            class DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) className; \
            EXTERN_C const GUID CLSID_##className

#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            interface DECLSPEC_UUID_WRAPPER(l##-##w1##-##w2##-##b1##b2##-##b3##b4##b5##b6##b7##b8) interfaceName; \
            EXTERN_C const GUID IID_##interfaceName

#endif // INITGUID

// Used in XAUDIO2_DEVICE_DETAILS below to describe the types of applications
// that the user has specified each device as a default for. 0 means that the
// device isn't the default for any role.
typedef enum XAUDIO2_DEVICE_ROLE
{
	NotDefaultDevice = 0x0,
	DefaultConsoleDevice = 0x1,
	DefaultMultimediaDevice = 0x2,
	DefaultCommunicationsDevice = 0x4,
	DefaultGameDevice = 0x8,
	GlobalDefaultDevice = 0xf,
	InvalidDeviceRole = ~GlobalDefaultDevice
} XAUDIO2_DEVICE_ROLE;
// Returned by IXAudio2::GetDeviceDetails
typedef struct XAUDIO2_DEVICE_DETAILS
{
	WCHAR DeviceID[256];                // String identifier for the audio device.
	WCHAR DisplayName[256];             // Friendly name suitable for display to a human.
	XAUDIO2_DEVICE_ROLE Role;           // Roles that the device should be used for.
	WAVEFORMATEXTENSIBLE OutputFormat;  // The device's native PCM audio output format.
} XAUDIO2_DEVICE_DETAILS;
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
///////////////////////////////////////////////////////////////////////////////

// COMPATIBILITY LAYER BETWEEN <2.8 AND >=2.8
class CXAudioPre28 : public IXAudio2 {
	IXAudio2_Pre28 *m_pAudio;
public:
	CXAudioPre28(IXAudio2_Pre28 *pAudio) :
		m_pAudio(pAudio) {
		m_pAudio->AddRef();
	}
	~CXAudioPre28() {
		m_pAudio->Release();
	}

	// IUnknown
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Outptr_ void** ppvInterface) {
		if (!ppvInterface)
			return E_INVALIDARG;
		if (riid == __uuidof(IXAudio2)) {
			*ppvInterface = (LPVOID)this;
			AddRef();
			return NOERROR;
		}
		return m_pAudio->QueryInterface(riid, ppvInterface);
	}
	__declspec(nothrow) ULONG STDMETHODCALLTYPE AddRef(void) {
		return  m_pAudio->AddRef();
	}
	__declspec(nothrow) ULONG STDMETHODCALLTYPE Release(void) {
		ULONG r = m_pAudio->Release();
		if (r == 0)
			delete this;
		return r;
	}
	// IXAudio2
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE RegisterForCallbacks(_In_ IXAudio2EngineCallback* pCallback) {
		return m_pAudio->RegisterForCallbacks(pCallback);
	}
	__declspec(nothrow) void STDMETHODCALLTYPE UnregisterForCallbacks(_In_ IXAudio2EngineCallback* pCallback) {
		m_pAudio->UnregisterForCallbacks(pCallback);
	}
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CreateSourceVoice(_Outptr_ IXAudio2SourceVoice** ppSourceVoice,
		_In_ const WAVEFORMATEX* pSourceFormat,
		UINT32 Flags X2DEFAULT(0),
		float MaxFrequencyRatio X2DEFAULT(XAUDIO2_DEFAULT_FREQ_RATIO),
		_In_opt_ IXAudio2VoiceCallback* pCallback X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) {
		return m_pAudio->CreateSourceVoice(ppSourceVoice, pSourceFormat);
	}
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CreateSubmixVoice(_Outptr_ IXAudio2SubmixVoice** ppSubmixVoice,
		UINT32 InputChannels, UINT32 InputSampleRate,
		UINT32 Flags X2DEFAULT(0), UINT32 ProcessingStage X2DEFAULT(0),
		_In_opt_ const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) {
		return m_pAudio->CreateSubmixVoice(ppSubmixVoice, InputChannels, InputSampleRate, Flags, ProcessingStage, pSendList, pEffectChain);
	}
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CreateMasteringVoice(_Outptr_ IXAudio2MasteringVoice** ppMasteringVoice,
		UINT32 InputChannels X2DEFAULT(XAUDIO2_DEFAULT_CHANNELS),
		UINT32 InputSampleRate X2DEFAULT(XAUDIO2_DEFAULT_SAMPLERATE),
		UINT32 Flags X2DEFAULT(0), _In_opt_z_ LPCWSTR szDeviceId X2DEFAULT(NULL),
		_In_opt_ const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL),
		_In_ AUDIO_STREAM_CATEGORY StreamCategory X2DEFAULT(AudioCategory_GameEffects)) {
		// TODO convert szDeviceId to DeviceIndex
		UINT32 DeviceIndex = 0;
		return m_pAudio->CreateMasteringVoice(ppMasteringVoice, InputChannels, InputSampleRate, Flags, DeviceIndex, pEffectChain);
	}
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE StartEngine(void) {
		return m_pAudio->StartEngine();
	}
	__declspec(nothrow) void STDMETHODCALLTYPE StopEngine(void) {
		m_pAudio->StopEngine();
	}
	__declspec(nothrow) HRESULT STDMETHODCALLTYPE CommitChanges(UINT32 OperationSet) {
		return m_pAudio->CommitChanges(OperationSet);
	}
	__declspec(nothrow) void STDMETHODCALLTYPE GetPerformanceData(_Out_ XAUDIO2_PERFORMANCE_DATA* pPerfData) {
		m_pAudio->GetPerformanceData(pPerfData);
	}
	__declspec(nothrow) void STDMETHODCALLTYPE SetDebugConfiguration(_In_opt_ const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration,
		_Reserved_ void* pReserved X2DEFAULT(NULL)) {
		m_pAudio->SetDebugConfiguration(pDebugConfiguration);
	}
};

#define WAVE_FORMAT_PCM 1

using namespace Xe::Debug;

DEFINE_CLSID(XAudio2_20, fac23f48, 31f5, 45a8, b4, 9b, 52, 25, d6, 14, 01, aa); // March 2008
DEFINE_CLSID(XAudio2_21, e21a7345, eb21, 468e, be, 50, 80, 4d, b9, 7c, f7, 08); // June 2008
DEFINE_CLSID(XAudio2_22, b802058a, 464a, 42db, bc, 10, b6, 50, d6, f2, 58, 6a); // August 2008
DEFINE_CLSID(XAudio2_23, 4c5e637a, 16c7, 4de3, 9c, 46, 5e, d2, 21, 81, 96, 2d); // November 2008
DEFINE_CLSID(XAudio2_24, 03219e78, 5bc3, 44d1, b9, 2e, f6, 3d, 89, cc, 65, 26); // March 2009
DEFINE_CLSID(XAudio2_25, 4c9b6dde, 6809, 46e6, a2, 78, 9b, 6a, 97, 58, 86, 70); // August 2009
DEFINE_CLSID(XAudio2_26, 3eda9b49, 2085, 498b, 9b, b2, 39, a6, 77, 84, 93, de); // February 2010
DEFINE_CLSID(XAudio2_27, 5a508685, a254, 4fba, 9b, 82, 9a, 24, b0, 03, 06, af); // June 2010
DEFINE_IID(IXAudio2_Pre28, 8bcf1f58, 9fe7, 4583, 8a, c6, e2, ad, c4, 65, c8, bb);

namespace Xe {
	namespace Sound {
		bool IsXAudioSupported() {
#if defined(PLATFORM_WIN32)
			static const ctstring XAudio2Vers[] = {
				_T("XAudio2_9.dll"),
				_T("XAudio2_8.dll"),
				_T("XAudio2_7.dll"),
				_T("XAudio2_6.dll"),
				_T("XAudio2_5.dll"),
				_T("XAudio2_4.dll"),
				/*_T("XAudio2_3.dll"),
				_T("XAudio2_2.dll"),
				_T("XAudio2_1.dll"),
				_T("XAudio2_0.dll"),*/
		};
			for (svar i = 0; i < lengthof(XAudio2Vers); i++) {
				HMODULE h = LoadLibraryEx(XAudio2Vers[i], NULL, DONT_RESOLVE_DLL_REFERENCES);
				if (h != 0) {
					FreeLibrary(h);
					return true;
				}
			}
			return false;
#elif defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
			return true;
#endif
		}

		class AudioXAudio2;
		class CBuffer : public IAudioBuffer {
		public:
			struct CXAudio2VoiceCallback : public IXAudio2VoiceCallback {
				static const svar MaxBuffersCount = 2;
				IAudioBuffer *pBuffer;
				ICallback *m_pCallback;

				CXAudio2VoiceCallback(CBuffer *cBuffer, ICallback *pCallback) :
					pBuffer(cBuffer),
					m_pCallback(pCallback) {
				}

				// Called just before this voice's processing pass begins.
				void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnVoiceProcessingPassStart(%i)"), BytesRequired);
					m_pCallback->OnBufferRequred(pBuffer, BytesRequired);
				}

				// Called just after this voice's processing pass ends.
				void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnVoiceProcessingPassEnd()"));
					m_pCallback->OnBufferProcessed();
				}

				// Called when this voice has just finished playing a buffer stream
				// (as marked with the XAUDIO2_END_OF_STREAM flag on the last buffer).
				void STDMETHODCALLTYPE OnStreamEnd() {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnStreamEnd()"));
				}

				// Called when this voice is about to start processing a new buffer.
				void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnBufferStart(%p)"), pBufferContext);
				}

				// Called when this voice has just finished processing a buffer.
				// The buffer can now be reused or destroyed.
				void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnBufferEnd(%p)"), pBufferContext);
				}

				// Called when this voice has just reached the end position of a loop.
				void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnLoopEnd(%p)"), pBufferContext);
				}

				// Called in the event of a critical error during voice processing,
				// such as a failing xAPO or an error from the hardware XMA decoder.
				// The voice may have to be destroyed and re-created to recover from
				// the error.  The callback arguments report which buffer was being
				// processed when the error occurred, and its HRESULT code.
				void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) {
					//LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("OnVoiceError(%p, %p)"), pBufferContext, Error);
				}
			};
			bool Query(IObject **obj, UID id) {
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
			CBuffer(AudioXAudio2 *pAudio, const WaveFormat &format, ICallback *pCallback);
			~CBuffer();
			bool Initialize();
			void Play();
			void Stop();
			void Submit(const float *data, svar length);

		private:
			AudioXAudio2 *m_pAudio;
			CXAudio2VoiceCallback m_voiceCallback;
			IXAudio2SourceVoice *m_pSourceVoice;
			XAUDIO2_BUFFER m_xabuffer;
		};

		class AudioXAudio2 : public IAudio {
			typedef HRESULT(__stdcall * PFNXAUDIO2CREATE) (_Outptr_ IXAudio2** ppXAudio2,
				UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
			PFNXAUDIO2CREATE pfnXAudio2Create;

		public:

			bool Query(IObject **obj, UID id) {
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
			AudioXAudio2() :
				m_hDllAudio(nullptr),
				m_pAudio(nullptr),
				m_pMasterVoice(nullptr)
			{

			}
			~AudioXAudio2() {
				if (m_pMasterVoice)
					m_pMasterVoice->DestroyVoice();
				if (m_pAudio)
					m_pAudio->Release();
				if (m_hDllAudio)
					FreeLibrary(m_hDllAudio);
				CoUninitialize();
			}

			bool Initialize() {
				struct Version {
					ctstring strDllName;
					svar version;
				};
				static const Version XAudio2Vers[] = {
					{ _T("XAudio2_9.dll"), 9 },
					{ _T("XAudio2_8.dll"), 8 },
					{ _T("XAudio2_7.dll"), 7 },
					{ _T("XAudio2_6.dll"), 6 },
					{ _T("XAudio2_5.dll"), 5 },
					{ _T("XAudio2_4.dll"), 4 },
					/*{ _T("XAudio2_3.dll"), 3 },
					{ _T("XAudio2_2.dll"), 2 },
					{ _T("XAudio2_1.dll"), 1 },
					{ _T("XAudio2_0.dll"), 0 },*/
				};
				static const UINT32 FLAGS = 0;
				static const XAUDIO2_PROCESSOR PROCESSOR = XAUDIO2_DEFAULT_PROCESSOR;

#ifdef PLATFORM_WIN32
				for (svar i = 0; i < lengthof(XAudio2Vers); i++) {
					m_hDllAudio = LoadLibraryEx(XAudio2Vers[i].strDllName, NULL, 0);
					if (m_hDllAudio != 0) {
						m_minVersion = XAudio2Vers[i].version;
						break;
					}
				}
				if (m_hDllAudio == nullptr) {
					LOG(Log::Priority_Error, Log::Type_Sound, _T("Unable to find an XAudio2 DLL."));
					return false;
				}
#elif defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE)
				m_minVersion = 8;
#elif defined(PLATFORM_WINUNIVERSAL)
				m_minVersion = 9;
#endif
				LOG(Log::Priority_Info, Log::Type_Sound, _T("XAudio 2.%i found."), m_minVersion);

				HRESULT hr;
#ifdef PLATFORM_WIN32
				hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
				if (m_minVersion >= 8) {
					pfnXAudio2Create = (PFNXAUDIO2CREATE)GetProcAddress(m_hDllAudio, _T("XAudio2Create"));
					hr = pfnXAudio2Create(&m_pAudio, FLAGS, PROCESSOR);
				}
				else {
					static const IID XAUDIO_ID[] = {
						__uuidof(XAudio2_20), // NOT SUPPORTED
						__uuidof(XAudio2_21), // NOT SUPPORTED
						__uuidof(XAudio2_22), // NOT SUPPORTED
						__uuidof(XAudio2_23), // NOT SUPPORTED
						__uuidof(XAudio2_24),
						__uuidof(XAudio2_25),
						__uuidof(XAudio2_26),
						__uuidof(XAudio2_27),
					};
					IXAudio2_Pre28 *pAudioPre28;
					hr = CoCreateInstance(XAUDIO_ID[m_minVersion], NULL,
						CLSCTX_INPROC_SERVER, __uuidof(IXAudio2_Pre28), (void**)&pAudioPre28);
					if (SUCCEEDED(hr))
					{
						hr = pAudioPre28->Initialize(FLAGS, PROCESSOR);
						if (!SUCCEEDED(hr))
						{
							m_pAudio->Release();
							return false;
						}
						m_pAudio = new CXAudioPre28(pAudioPre28);
						pAudioPre28->Release();
					}
					else {
						LOG(Log::Priority_Error, Log::Type_Sound, _T("CoCreateInstance failed.."), m_minVersion);
						return false;
					}
				}
#else
				if (FAILED(hr = XAudio2Create(&m_pAudio, FLAGS, PROCESSOR)))
					return false;
#endif
				SetSampleRate(44100);
				m_pAudio->StartEngine();
				return true;
			}
			bool SetSampleRate(svar sampleRate) {
				LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("Setting sample rate to %i..."), sampleRate);
				if ((sampleRate % 100)) {
					LOG(Log::Priority_Warning, Log::Type_Sound, _T("Sample rate %i is invalid."), sampleRate);
					return false;
				}
				if (m_pMasterVoice != nullptr) {
					LOG(Log::Priority_Diagnostics, Log::Type_Sound, _T("Destroying previous voice..."));
					m_pMasterVoice->DestroyVoice();
				}
				HRESULT hr = m_pAudio->CreateMasteringVoice(&m_pMasterVoice, XAUDIO2_DEFAULT_CHANNELS, sampleRate);
				if (SUCCEEDED(hr)) {
					m_sampleRate = sampleRate;
					LOG(Log::Priority_Info, Log::Type_Sound, _T("Sample rate %i set with success."), sampleRate);
					return true;
				}
				LogHresult(hr);
				return false;
			}

			void LogHresult(HRESULT hr) {
				switch (hr) {
				case S_OK:
					break;
				case 0x8889000A:
					LOG(Log::Priority_Warning, Log::Type_Sound, _T("Sound device already in use."));
					break;
				default:
					LOG(Log::Priority_Warning, Log::Type_Sound, _T("Generic error."));
					break;
				}
			}

			bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback) {
				CBuffer *cBuffer = new CBuffer(this, format, pCallback);
				if (cBuffer->Initialize()) {
					*buffer = cBuffer;
					return true;
				}
				cBuffer->Release();
				*buffer = nullptr;
				return false;
			}

			HMODULE m_hDllAudio;
			svar m_minVersion;
			IXAudio2 *m_pAudio;
			IXAudio2MasteringVoice *m_pMasterVoice;
		};


		CBuffer::CBuffer(AudioXAudio2 *pAudio, const WaveFormat &format, ICallback *pCallback) :
			IAudioBuffer(format, pCallback),
			m_pAudio(pAudio),
			m_voiceCallback(this, pCallback),
			m_xabuffer( { 0 } ){
			m_pAudio->AddRef();
		}
		CBuffer::~CBuffer() {
			if (m_pSourceVoice)
				m_pSourceVoice->DestroyVoice();
			m_pAudio->Release();
		}
		bool CBuffer::Initialize() {
			const WaveFormat &format = GetFormat();
			WAVEFORMATEX wf = { WAVE_FORMAT_IEEE_FLOAT, (WORD)format.NumberOfChannels, (DWORD)format.SampleRate,
				(DWORD)(format.SampleRate * format.SampleLength), (WORD)format.SampleLength, (WORD)32, sizeof(WAVEFORMATEX) };
			HRESULT hr = m_pAudio->m_pAudio->CreateSourceVoice(&m_pSourceVoice, &wf,
				0, XAUDIO2_DEFAULT_FREQ_RATIO, m_pCallback ? &m_voiceCallback : nullptr);
			if (!SUCCEEDED(hr))
				return false;
			m_xabuffer.PlayBegin = 0;
			m_xabuffer.PlayLength = 0;
			return true;
		}
		void CBuffer::Play() {
			m_pSourceVoice->Start();
		}
		void CBuffer::Stop() {
			m_pSourceVoice->Stop();
		}
		void CBuffer::Submit(const float *data, svar length) {
			m_xabuffer.pAudioData = (u8*)data;
			m_xabuffer.AudioBytes = length;
			HRESULT hr = m_pSourceVoice->SubmitSourceBuffer(&m_xabuffer);
		}

		bool CreateXAudio(IAudio **pAudio) {
			AudioXAudio2 *pXAudio2 = new AudioXAudio2;
			if (pXAudio2->Initialize()) {
				*pAudio = pXAudio2;
				return true;
			}
			*pAudio = nullptr;
			return false;
		}
	}
}

#else

namespace Xe {
	namespace Sound {
		bool CreateXAudio(IAudio **pAudio) {
			*pAudio = nullptr;
			return false;
		}
	}
}

#endif