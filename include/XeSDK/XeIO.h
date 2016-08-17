#pragma once
#include <XeSDK/IIOStream.h>

namespace Xe
{
	namespace Error {
		//! \brief file cannot be processed because system doesn't find it
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0001, IO_NOT_FOUND);
		//! \brief file cannot be processed because it's already in use
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0002, IO_ALREADY_IN_USE);
		//! \brief file cannot be opened because it's read only and you are trying to open it for writing
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0003, IO_READ_ONLY);
		//! \brief file is found from the system and it cannot be processed
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0004, IO_ALREADY_EXISTS);
		//! \brief file's reading process failed
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0005, IO_READ_FAILED);
		//! \brief file's writing process failed
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0006, IO_WRITE_FAILED);
		//! \brief seek position is invalid, usually when it's < 0
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0007, IO_POSITION_INVALID);
		//! \brief seek mode is invalid
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0008, IO_SEEK_MODE_INVALID);
		//! \brief file was not resized
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x0009, IO_NOT_RESIZED);
		//! \brief file path is invalid
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x000A, IO_PATH_INVALID);
		//! \brief specified length is invalid
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x000B, IO_LENGTH_INVALID);
		//! \brief specified length is invalid
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x000C, IO_ACCESS_DENIED);
		//! \brief directory is not empty
		XE_CREATE_RESULT(true, 0, HM_SDK, LM_STORAGEFILE, 0x000C, IO_NOT_EMPTY);
	}

	//! \brief Input / output management
	namespace IO
	{
		//! \brief copy a stream into another
		//! \param[out] dst output stream
		//! \param[in] src input stream
		/** \details this static method copy the content of
		* in stream into out stream; the in and out position
		* should be specified before the copy; both stream
		* position will be moved by in->GetLenth() -
		* out->GetPosition(); the length of the stream
		* will be automatically calculated
		*/
		void Copy(IStream *dst, IStream *src);

		//! \brief copy a stream into another
		//! \param[out] dst output stream
		//! \param[in] src input stream
		//! \param[in] length how bytes to copy
		/** \details this static method copy the content of
		* in stream into out stream; the in and out position
		* should be specified before the copy; both stream
		* position will be moved by in->GetLenth() -
		* out->GetPosition();
		*/
		void Copy(IStream *dst, IStream *src, s64 length);
	}
}