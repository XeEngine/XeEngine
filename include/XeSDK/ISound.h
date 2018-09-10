#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Sound {
		enum BitFormat
		{
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

		enum WaveFormat
		{
			WaveType_Unknown,

			// Most supported format
			WaveType_Pcm,

			// Compressed PCM in 4-bit
			WaveType_Adpcm,

			// Xbox specific format
			WaveType_Wma2,

			// Xbox specific format
			WaveType_Wma3,

			// Xbox specific format
			WaveType_Xma2
		};

		struct WaveDesc
		{
			WaveFormat WaveFormat;
			BitFormat BitDepth;
			u32 SampleRate;
			u16 SampleLength;
			u16 NumberOfChannels;
		};

		class IAudioBuffer;
		interface IAudioBufferCallback : public IObject
		{
			virtual ~IAudioBufferCallback() {};

			virtual void OnBufferRequred(IAudioBuffer *pBuffer, svar bytesRequired) = 0;
			virtual void OnBufferProcessed() = 0;
		};

		//! \brief where to write audio data to send it to audio card
		class IAudioBuffer : public IObject {
		public:

			//! \brief specify wave format and callback function
			IAudioBuffer(const WaveDesc &properties);
			virtual ~IAudioBuffer() = 0 { }

			virtual void SetCallback(IAudioBufferCallback& callback) = 0;

			//! \brief wave format specified from the beginning
			const WaveDesc &GetFormat() const;

			//! \brief send audio data through the buffer
			//! \param[in] data that contains the information to send
			//! \param[in] length in bytes of data to send
			virtual void Submit(void *data, svar length) = 0;

			virtual bool IsPlaying() const = 0;

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
			const WaveDesc m_format;
		};

		interface IAudio : public IObject {
		public:
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

			virtual bool CreateBuffer(IAudioBuffer **buffer, const WaveDesc &format) = 0;

		protected:
			svar m_SampleRate;
		};
	}
}