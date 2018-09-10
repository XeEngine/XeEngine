#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {
	typedef u64 SampleOffset;

	interface IAudioSource: public IObject {
		//! \brief Get the wave descriptor.
		virtual const WaveDesc &GetDesc() const = 0;

		//! \brief Get the total sampels count.
		virtual SampleOffset GetSamplesCount() const = 0;

		//! \brief Get the stream position.
		virtual SampleOffset GetPosition() const = 0;

		//! \brief Set the stream to the next sampleIndex to read.
		virtual void SetPosition(SampleOffset sampleIndex) = 0;

		//! \brief Get the first sample index for a loop.
		virtual SampleOffset GetLoopStart() const = 0;

		//! \brief Set the start loop position.
		//! \param[in] sampleIndex Sample index where to start again the file.
		/** \details When GetLoopEnd is reached, SetPosition will be called with the
		 * specified sample index. If the sampleIndex is equal or greater than
		 * GetSamplesCount, then the loop is ignored.
		 * When sampleIndex is less than GetSamplesCount, then Read should never
		 * return 0, or the read could be terminated.
		 */
		virtual void SetLoopStart(SampleOffset sampleIndex) = 0;

		//! \brief Get the last sample index.
		virtual SampleOffset GetLoopEnd() const = 0;

		//! \brief Set the end loop position.
		//! \param[in] sampleIndex Specify what is the last sample to read before to wrap.
		/** \details When the specified sampleIndex is reached, then the stream
		 * position needs to be wrapped to GetLoopStart.
		 * Should not be less or equal than 0 or greater than GetSamplesCount.
		 */
		virtual void SetLoopEnd(SampleOffset sampleIndex) = 0;

		//! \brief Decode the specified amount of data using GetDesc's format.
		//! \param[in] data Where the decoded audio source will be written.
		//! \param[in] offset Offset of data where to start to write data.
		//! \param[in] bytesCount Maximum number of bytes to write.
		//! \return Bytes written. 0 if the end of the stream has been reached.
		virtual int Read(void *data, int offset, int bytesCount) = 0;
	};
} }