#include "pch.h"
#include "XAudioEngine.h"
#include "XAudioBuffer.h"
#include "XAudioCompatibilityLayer.h"

using namespace Xe::Debug;

namespace Xe { namespace Sound {

	void XAudioEngine::LogHresult(HRESULT hr) {
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

	bool XAudioEngine::Query(IObject **obj, UID id) {
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
	
	XAudioEngine::XAudioEngine() :
		m_hDllAudio(nullptr),
		m_pAudio(nullptr),
		m_pMasterVoice(nullptr)
	{

	}
	
	XAudioEngine::~XAudioEngine() {
		if (m_pMasterVoice)
			m_pMasterVoice->DestroyVoice();
		if (m_pAudio)
			m_pAudio->Release();
		if (m_hDllAudio)
			FreeLibrary(m_hDllAudio);
		CoUninitialize();
	}

	bool XAudioEngine::Initialize() {
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

	bool XAudioEngine::SetSampleRate(svar sampleRate) {
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

	bool XAudioEngine::CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback) {
		XAudioBuffer *cBuffer = new XAudioBuffer(this, format, pCallback);
		if (cBuffer->Initialize()) {
			*buffer = cBuffer;
			return true;
		}
		cBuffer->Release();
		*buffer = nullptr;
		return false;
	}

} }