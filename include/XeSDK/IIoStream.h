#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeError.h>

namespace Xe { namespace IO {
	//! \brief seeking inside the stream
	enum Seek
	{
		//! \brief move from the beginning of the stream.
		//! \details only positive position allowed
		Seek_Set = 0,
		//! \brief move from the current position of the stream.
		//! \details positive and negative position are allowed.
		Seek_Cur = 1,
		//! \brief move from the end of the stream.
		//! \details positive and negative position can be specified.
		Seek_End = 2,
	};

	//! \brief manage generic input / output streams
	struct IStream : public IObject
	{
		//! \brief interface id
		static const UID ID = 0xa68eb1d3541549dbULL;

		//! \brief commit changes if a stream was opened for writing
		/** \details when a stream is opened for writing, not all written data
		* is directly written to the file, but it's stored in a buffer to
		* improve performances; this function write the current buffer and
		* save the changes; it's automatically called when the stream
		* is released. Not all implementations of stream supports
		* caching and flushing.
		*/
		virtual void Flush() = 0;

		//! \brief get current stream's position
		/** \return stream's position; if IsSeekable is false, then
			* return value must be -1.
			*/
		virtual s64 GetPosition() const = 0;

		//! \brief move the position between the current stream
		//! \param[in] offset position to set or move; it can be a negative value to move backward through the stream
		//! \param[in] seek seeking mode through the file
		/** \details you can specify a position greater than the stream's
			* size; in that case, with Write, the file will be streched only
			* IsOverflowable is true. It fails if IsSeekable is false.
			*/
		virtual RESULT SetPosition(s64 offset, IO::Seek seek = IO::Seek_Set) = 0;

		//! \brief get the stream's length
		/** \return stream's length; if IsSeekable is false, then
			* return value must be -1.
			*/
		virtual s64 GetLength() const = 0;

		//! \brief expand or cut the length of current stream, if it's re-sizable
		//! \param[in] offset new length to set
		virtual RESULT SetLength(s64 offset) = 0;

		//! \brief check if the current stream is readable
		//! \return true if it's readable, else false
		virtual bool CanRead() const = 0;

		//! \brief check if the current stream is writable
		//! \return true if it's writable, else false
		virtual bool CanWrite() const = 0;

		//! \brief check if the current stream support seeking
		//! \return true if seeking, get position and get length is supported
		virtual bool CanSeek() const = 0;

		//! \brief check if the current stream is overflowable
		//! \return true if it's overflowable, else false
		virtual bool CanOverflow() const = 0;

		//! \brief read the content from the current stream
		//! \param[in,out] data memory pointer where the data will be written
		//! \param[in] offset memory pointer's position
		//! \param[in] length how bytes to read
		//! \return read bytes
		virtual svar Read(void* data, svar offset, s32 length) = 0;

		//! \brief write the content to the current stream
		//! \param[in] data memory pointer where the data will be copied
		//! \param[in] offset memory pointer's position
		//! \param[in] length how bytes to read
		//! \return wrote bytes
		virtual svar Write(const void* data, svar offset, s32 length) = 0;
	};
} }