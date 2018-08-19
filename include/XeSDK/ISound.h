#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Sound {
		enum BitFormat {
			BitFormat_Unspecified = 0,
			BitFormat_S8 = 8,
			BitFormat_U8 = 9,
			BitFormat_S16L = 16,
			BitFormat_U16L = 17,
			BitFormat_S16B = 18,
			BitFormat_U16B = 19,
			BitFormat_S24L = 24,
			BitFormat_U24L = 25,
			BitFormat_S24B = 26,
			BitFormat_U24B = 27,
			BitFormat_S32L = 32,
			BitFormat_U32L = 33,
			BitFormat_S32B = 34,
			BitFormat_U32B = 35,
			BitFormat_F32 = 36,
			BitFormat_F64 = 70,
		};
		struct WaveFormat {
			uvar SampleLength;
			uvar SampleRate;
			uvar NumberOfChannels;
		};

		//! \brief where to write audio data to send it to audio card
		class IAudioBuffer : public IObject {
		public:
			interface ICallback {
				virtual void OnBufferRequred(IAudioBuffer *pBuffer, svar bytesRequired) = 0;
				virtual void OnBufferProcessed() = 0;
			};

			static const UID ID = 0x38e46cb0fe6d4ed3ULL;

			//! \brief specify wave format and callback function
			IAudioBuffer(const WaveFormat &properties, ICallback *pCallback);

			//! \brief wave format specified from the beginning
			const WaveFormat &GetFormat() const;

			//! \brief send audio data through the buffer
			//! \param[in] data that contains the information to send
			//! \param[in] length in bytes of data to send
			virtual void Submit(const float *data, svar length) = 0;

			//! \brief start to process the buffer
			//! \details revert with Stop
			//! \sa Stop
			virtual void Play() = 0;

			//! \brief stop to process the buffer
			//! \details revert with Play
			//! \sa Play
			virtual void Stop() = 0;

			virtual float GetVolume() = 0;
			virtual void SetVolume(float volume) = 0;
			virtual void GetChannelVolumes(int channels, float* volumes) = 0;
			virtual void SetChannelVolumes(int channels, const float* volumes) = 0;

		protected:
			const WaveFormat m_format;
			const ICallback *m_pCallback;
		};
		interface IAudio : public IObject {
		public:
			static const UID ID = 0x93b3a075c0ee4125ULL;

			IAudio();

			//! \brief Get the current sample rate
			//! \return Default sample rate or sample rate set from SetSampleRate
			svar GetSampleRate() const;

			//! \brief Set a new sample rate for this audio device
			//! \param[in] sampleRate to set; it must be a multiple of 100
			//! \return false if specified sampleRate is not supported
			/** \details Setting a new sample rate means that device must be
			 * re-set.
			 */
			virtual bool SetSampleRate(svar sampleRate) = 0;

			virtual bool CreateBuffer(IAudioBuffer **buffer, const WaveFormat &format, IAudioBuffer::ICallback *pCallback) = 0;

		protected:
			svar m_sampleRate;
		};
	}
}