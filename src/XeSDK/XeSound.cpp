#include "pch.h"
#include <XeSDK/ISound.h>
#include <XeSDK/XeSound.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeDrivers.h>
#include <XeSDK/IDriversSoundDriver.h>

#define DRIVER_NULL "SoundNull"
#define DRIVER_WASAPI "Wasapi"
#define DRIVER_XAUDIO "XAudio2"

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
			m_SampleRate(0) {
		}
		svar IAudio::GetSampleRate() const {
			return m_SampleRate;
		}
		IAudioBuffer::IAudioBuffer(const WaveFormat &format, ICallback *pCallback) :
			m_format(format),
			m_pCallback(pCallback) {
		}
		const WaveFormat &IAudioBuffer::GetFormat() const {
			return m_format;
		}

		extern bool CreateXAudio(IAudio **pAudio);
		extern bool CreateWasapi(IAudio **pAudio);

		RESULT Create(IAudio **audio, AudioEngine engine) {
			static const ctstring DriverName[] =
			{
				DRIVER_XAUDIO,
				DRIVER_NULL,
				DRIVER_WASAPI,
				DRIVER_XAUDIO
			};

			return Create(audio, DriverName[engine]) ? Error::OK : Error::GENERIC_ERROR;
		}


		bool Create(IAudio **audio, ctstring driverName)
		{
			LOGD("Checking for '%s' sound driver...", driverName);

			Xe::Drivers::Sound::ISoundDriver* soundDriver = nullptr;
			auto drivers = Xe::Drivers::GetDrivers(Xe::Drivers::DriverTypeFilter_Sound);
			for (auto it = drivers.begin(); it != drivers.end(); it++)
			{
				auto currentDriverName = (*it)->GetDriverName();
				if (strcmp(currentDriverName, driverName) == 0)
				{
					soundDriver = (Xe::Drivers::Sound::ISoundDriver*)*it;
				}
			}

			if (soundDriver != nullptr)
			{
				LOGI("Sound driver %s found!", driverName);
				Xe::Sound::AudioInitDesc desc{ 0 };
				desc.SampleRate = 48000;
				if (soundDriver->Factory(audio, desc))
				{
					LOGI("Sound driver %s initialized with success!", driverName);
					return true;
				}
				else
				{
					LOGE("Unable to initialize the sound driver %s.", driverName);
				}
			}
			else
			{
				LOGE("Sound driver %s not found.", driverName);
			}

			*audio = nullptr;
			return false;
		}
	}
}