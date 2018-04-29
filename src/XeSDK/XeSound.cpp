#include "pch.h"
#include <XeSDK/ISound.h>
#include <XeSDK/XeSound.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeDebugLog.h>

using namespace Xe::Debug;

namespace Xe {
	namespace Sound {
		inline float ConvertS8(s8 *n, int i) { return (float)n[i] / 128.0f; }
		inline float ConvertS16L(s16 *n, int i) { return (float)n[i] / 32768.0f; }
		inline float ConvertS24L(s8 *n, int i) {
			s8 *pn = &n[i * 3];
			int value = pn[0] | (pn[1] << 8) | (pn[2] << 16);
			return (float)value / 8388608.0f;
		}
		template <typename T, int Size>
		void Convert(float *pDataOut, const void *pDataIn, float (*f)(T*, int i), svar samplesCount) {
			T *ptr = (T*)pDataIn;
			while (samplesCount > 4) {
				pDataOut[0] = f(ptr, 0);
				pDataOut[1] = f(ptr, 1);
				pDataOut[2] = f(ptr, 2);
				pDataOut[3] = f(ptr, 3);
				pDataOut += 4;
				ptr += Size * 4;
				samplesCount -= 4;
			}
			if (samplesCount > 1) {
				if (samplesCount > 2) {
					pDataOut[0] = f(ptr, 0);
					pDataOut[1] = f(ptr, 1);
					pDataOut[2] = f(ptr, 2);
				}
				else {
					pDataOut[0] = f(ptr, 0);
					pDataOut[1] = f(ptr, 1);
				}
			}
			else if (samplesCount > 0)
				pDataOut[0] = f(ptr, 0);
		}

		svar GetBitsPerSample(BitFormat format) {
			return (svar)format & ~7;
		}
		void Convert(float *pDataOut, const void *pDataIn, BitFormat format, svar samplesCount) {
			switch (format) {
			case BitFormat_S8: Convert<s8, 1>(pDataOut, pDataIn, ConvertS8, samplesCount); break;
			case BitFormat_S16L: Convert<s16, 1>(pDataOut, pDataIn, ConvertS16L, samplesCount); break;
			case BitFormat_S24L: Convert<s8, 3>(pDataOut, pDataIn, ConvertS24L, samplesCount); break;
			default: Memory::Fill(pDataOut, 0, samplesCount * sizeof(float)); break;
			}
		}

		IAudio::IAudio() :
			m_sampleRate(0) {
		}
		svar IAudio::GetSampleRate() const {
			return m_sampleRate;
		}
		IAudioBuffer::IAudioBuffer(const WaveFormat &format, ICallback *pCallback) :
			m_format(format),
			m_pCallback(pCallback) {
		}
		const WaveFormat &IAudioBuffer::GetFormat() const {
			return m_format;
		}

		class AudioNull : public IAudio {
		public:
			class NullBuffer : public IAudioBuffer {
			public:
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
				NullBuffer(const WaveFormat &format, ICallback *pCallback) :
					IAudioBuffer(format, pCallback) {
				}
				void Submit(const float *data, svar length) {

				}
				void Play() {

				}
				void Stop() {

				}

				float GetVolume() {
					return 1.0f;
				}

				void SetVolume(float volume) {
				}

				void GetChannelVolumes(int channels, float* volumes) {
				}

				void SetChannelVolumes(int channels, const float* volumes) {
				}
			};

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
			AudioNull() {
			}
			bool SetSampleRate(svar sampleRate) {
				if (sampleRate % 100)
					return false;
				m_sampleRate = sampleRate;
				return true;
			}
			bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback) {
				*buffer = new NullBuffer(format, pCallback);
				return true;
			}
		};

		extern bool IsXAudioSupported();
		extern bool CreateXAudio(IAudio **pAudio);
		extern bool IsWasapiSupported();
		extern bool CreateWasapi(IAudio **pAudio);

		bool IsSupported(AudioEngine audioEngine) {
			switch (audioEngine) {
			case AudioEngine_Null:
				return true;
			case AudioEngine_Wasapi:
				return false;
			case AudioEngine_XAudio:
				return IsXAudioSupported();
			default:
				return false;
			}
		}
		RESULT Create(IAudio **audio, AudioEngine engine) {
			static const ctstring StrEngineName[] = {
				_T("DEFAULT"),
				_T("NULL"),
				_T("WASAPI"),
				_T("XAUDIO2"),
			};
			static const Log::Type LogType = Log::Type_Sound;

			*audio = nullptr;
			if (engine != AudioEngine_Default)
			{
				LOG(Log::Priority_Diagnostics, LogType, _T("Checking if engine %s is supported..."), StrEngineName[engine]);
				if (!IsSupported(engine)) {
					LOG(Log::Priority_Error, LogType, _T("Engine %s is not supported."), StrEngineName[engine]);
					return Error::GENERIC_ERROR;
				}
			}
			else {
				LOG(Log::Priority_Diagnostics, LogType, _T("Default engine specified."));
				engine = GetDefaultAudioEngine();
			}
			LOG(Log::Priority_Diagnostics, LogType, _T("Creating engine %s..."), StrEngineName[engine]);
			switch (engine) {
			case AudioEngine_Null:
				*audio = new AudioNull;
				break;
			case AudioEngine_Wasapi:
				CreateWasapi(audio);
				break;
			case AudioEngine_XAudio:
				CreateXAudio(audio);
				break;
			default:
				LOG(Log::Priority_Error, LogType, _T("Ooops, engine %s is not yet supported"), StrEngineName[engine]);
				return Error::NOT_IMPLEMENTED;
			}
			LOG(Log::Priority_Info, LogType, _T("Engine %s was created."), StrEngineName[engine]);
			return Error::OK;
		}
	}
}