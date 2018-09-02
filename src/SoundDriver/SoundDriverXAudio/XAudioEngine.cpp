#include "pch.h"
#include "XAudioEngine.h"
#include "XAudioBuffer.h"
#include "XAudioCompatibilityLayer.h"

#define LOGX(hr) (Log((hr), __func__, __FILE__, __LINE__))

using namespace Xe::Debug;

namespace Xe { namespace Sound {

	HRESULT XAudioEngine::Log(HRESULT hr, ctstring func, ctstring source, int line)
	{
		ctstring msg = nullptr;

		if (FAILED(hr))
		{
			switch (hr)
			{
			case 0x8889000A:
				msg = "Sound device already in use.";
				break;
			default:
				msg = "Unknown error";
				break;
			}

			Xe::Logger::DebugError("%s:%i %s: %s (%08X)", source, line, func, msg, hr);
		}

		return hr;
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
		HRESULT hr;
		LOGX(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED));
		HR(hr);
	}
	
	XAudioEngine::~XAudioEngine()
	{
		if (m_pMasterVoice)
			m_pMasterVoice->DestroyVoice();
		if (m_pAudio)
			m_pAudio->Release();
		if (m_hDllAudio)
			FreeLibrary(m_hDllAudio);
		CoUninitialize();
	}

	bool XAudioEngine::Initialize(const Xe::Sound::AudioInitDesc& desc)
	{
		static const UINT32 FLAGS = 0;
		static const XAUDIO2_PROCESSOR PROCESSOR = XAUDIO2_DEFAULT_PROCESSOR;

#if SETTINGS_LOADLIBRARY == 1
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
		
		for (svar i = 0; i < lengthof(XAudio2Vers); i++) {
			m_hDllAudio = LoadLibraryEx(XAudio2Vers[i].strDllName, NULL, 0);
			if (m_hDllAudio != 0) {
				m_MinVersion = XAudio2Vers[i].version;
				break;
			}
		}
		if (m_hDllAudio == nullptr) {
			LOG(Log::Priority_Error, Log::Type_Sound, _T("Unable to find an XAudio2 DLL."));
			return false;
		}
#else
		m_MinVersion = 9;
#endif
		LOG(Log::Priority_Info, Log::Type_Sound, _T("XAudio 2.%i found."), m_MinVersion);

		HRESULT hr;
#if SETTINGS_LOADLIBRARY == 1
		if (m_MinVersion >= 8)
		{
			pfnXAudio2Create = (PFNXAUDIO2CREATE)GetProcAddress(m_hDllAudio, _T("XAudio2Create"));
			hr = pfnXAudio2Create(&m_pAudio, FLAGS, PROCESSOR);
		}
		else
		{
			static const IID XAUDIO_ID[] =
			{
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
			hr = CoCreateInstance(XAUDIO_ID[m_MinVersion], NULL,
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
			else
			{
				Logger::DebugError("CoCreateInstance failed..");
				return false;
			}
		}
#else
		if (FAILED(hr = XAudio2Create(&m_pAudio, FLAGS, PROCESSOR)))
		{
			HR(hr);
			return false;
		}
#endif
		
		SetSampleRate(desc.SampleRate);

		if (FAILED(hr = m_pAudio->StartEngine()))
		{
			HR(hr);
			return false;
		}
		return true;
	}

	bool XAudioEngine::SetSampleRate(svar sampleRate)
	{
		if (sampleRate == 0)
		{
			sampleRate = XAUDIO2_DEFAULT_SAMPLERATE;
		}

		Logger::Info("Setting sample rate to %i...", sampleRate);
		if (sampleRate <= 0 || (sampleRate % 100) != 0)
		{
			Logger::DebugWarning("Sample rate %i is invalid.", sampleRate);
			return false;
		}

		if (m_pMasterVoice != nullptr)
		{
			Logger::Info("Destroying previous voice...");
			m_pMasterVoice->DestroyVoice();
		}

		UINT32 inputChannels = XAUDIO2_DEFAULT_CHANNELS;
		UINT32 flags = 0;
		LPCWSTR szDeviceId = NULL;

		HRESULT hr = LOGX(m_pAudio->CreateMasteringVoice(&m_pMasterVoice, inputChannels, sampleRate, flags, szDeviceId));
		if (SUCCEEDED(hr))
		{
			m_SampleRate = sampleRate;
			Logger::Info("Sample rate %i set with success.", sampleRate);
			return true;
		}

		return false;
	}

	bool XAudioEngine::CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback)
	{
		XAudioBuffer *cBuffer = new XAudioBuffer(this, format, pCallback);
		if (cBuffer->Initialize())
		{
			*buffer = cBuffer;
			return true;
		}

		cBuffer->Release();
		*buffer = nullptr;
		return false;
	}

} }