#include "pch.h"
#include "XeStorageFile.h"
#include <XeSDK/XeIO.h>
#include <XeSDK/XeString.h>

#include <cerrno>
#include <unistd.h>
#include <sys/stat.h>

Xe::RESULT Storage_Unix_GetError()
{
	switch (errno)
	{
	case EFAULT:
	case EINVAL:
		return Xe::Error::INVALID_PARAMETER;
	case EROFS:
	case EACCES:
	case EBUSY:
		return Xe::Error::IO_ACCESS_DENIED;
	case EEXIST:
		return Xe::Error::IO_ALREADY_EXISTS;
	case ELOOP:
	case ENAMETOOLONG:
	case ENOENT:
	case ENOTDIR:
		return Xe::Error::IO_PATH_INVALID;
	case ENOTEMPTY:
		return Xe::Error::IO_NOT_EMPTY;
	default:
		return Xe::Error::GENERIC_ERROR;
	}
}

#define GET_FILE(H) ((*(InternalFileHandle*)handle).file)
#define GET_FILENO(H) (fileno(GET_FILE(H)))

namespace Xe { namespace Storage {
	class FileStream : public IO::IFileStream {
		FILE *m_file;
		int m_desc;
		Access m_access;
		String m_filename;

		void Flush() {
			fflush(m_file);
		}
		s64 GetPosition() const {
			return (s64)lseek64(m_desc, 0, IO::Seek_Cur);
		}
		RESULT SetPosition(s64 offset, IO::Seek seek = IO::Seek_Set) {
			if (lseek64(m_desc, offset, seek) >= 0)
				return Error::OK;
			else
			{
				switch (seek)
				{
				case IO::Seek_Set:
					if (offset < 0LL)
						return Error::IO_POSITION_INVALID;
					break;
				case IO::Seek_Cur:
					if (GetPosition() + offset < 0LL)
						return Error::IO_POSITION_INVALID;
					break;
				case IO::Seek_End:
					if (GetLength() + offset < 0LL)
						return Error::IO_POSITION_INVALID;
					break;
				default:
					return Error::IO_SEEK_MODE_INVALID;
				}
				return Error::GENERIC_ERROR;
			}
		}
		s64 GetLength() const {
			struct stat buf;
			fstat(m_desc, &buf);
			return buf.st_size;
		}
		RESULT SetLength(s64 offset) {
			svar err;
#ifdef SECURE_CRT
			err = _chsize_s(m_desc, offset);
#else
			err = ftruncate(m_desc, offset);
#endif
			switch (err) {
			case 0:
				return Xe::Error::OK;
			case EBADF:
				return Xe::Error::IO_READ_ONLY;
			case EINVAL:
				return Xe::Error::IO_LENGTH_INVALID;
			}
			return Xe::Error::GENERIC_ERROR;
		}
		bool CanRead() const {
			return (m_access & Storage::Access_Read) != 0;
		}
		bool CanWrite() const {
			return (m_access & Storage::Access_Write) != 0;
		}
		bool CanSeek() const {
			return true;
		}
		bool CanOverflow() const {
			return CanWrite();
		}
		svar Read(void* data, svar offset, s32 length) {
			if (!CanRead())
				return -1;
			size_t n;
			n = fread((u8*)data + offset, 1, length, m_file);
			if (n == 0 && length != 0)
				return Xe::Error::IO_READ_FAILED;
			return n;
		}
		svar Write(const void* data, svar offset, s32 length) {
			if (!CanWrite())
				return -1;
			size_t n;
			n = fwrite((u8*)data + offset, 1, length, m_file);
			if (n == 0 && length != 0)
				return Xe::Error::IO_READ_FAILED;
			return n;
		}

		const String& GetFileName() const
		{
			return m_filename;
		}

	public:
		FileStream(FILE *file, ctstring filename, Access access) :
			m_file(file), m_desc(fileno(file)), m_access(access) {
			svar len = String::GetLength(filename);
			m_filename = new tchar[len + 1];
			String::NCopy(m_filename, filename, len + 1);
		}
		~FileStream() {
			fclose(m_file);
			delete[] m_filename;
		}
	};

	bool File::Exists(ctstring filename)
	{
		struct stat ret;
		return stat(filename, &ret) >= 0 ? true : false;
	}
	RESULT File::Delete(ctstring filename)
	{
		if (remove(filename) != 0)
		{
			if (Exists(filename))
				return Error::IO_ALREADY_IN_USE;
			return Error::IO_NOT_FOUND;
		}
		return Error::OK;
	}
	RESULT File::Move(ctstring oldpath, ctstring newpath)
	{
		if (!Exists(oldpath))
			return Error::IO_NOT_FOUND;
		if (Exists(newpath))
			return Error::IO_ALREADY_EXISTS;
		if (rename(oldpath, newpath) == 0)
			return Error::OK;
		return Storage_Unix_GetError();
	}

	RESULT File::Open(IO::IFileStream **fstream, ctstring filename, Access access, Mode mode) {
		tchar strMode[4];
		tstring pStrMode = strMode;
		bool bRead;

		*fstream = (IO::IFileStream*)nullptr;
		switch (mode) {
		case Mode_Create:
		case Mode_CreateNew:
			*pStrMode++ = 'w';
			bRead = false;
			break;
		case Mode_Open:
		case Mode_OpenIfExists:
			*pStrMode++ = 'r';
			bRead = true;
			break;
		default:
			return Error::INVALID_PARAMETER;
		}
		*pStrMode++ = 'b';

		switch (access) {
		case Access_Read:
			if (!bRead)
				*pStrMode++ = '+';
			break;
		case Access_Write:
			if (bRead)
				pStrMode[-2] = 'w';
			break;
		case Access_ReadWrite:
			if (bRead)
				*pStrMode++ = '+';
			break;
		default:
			return Error::INVALID_PARAMETER;
		}
		*pStrMode = '\0';

		switch (mode) {
		case Mode_Create:
			break;
		case Mode_CreateNew:
			if (Exists(filename))
				return Error::IO_ALREADY_EXISTS;
			break;
		case Mode_Open:
			break;
		case Mode_OpenIfExists:
			if (!Exists(filename))
				return Error::IO_NOT_FOUND;
			break;
		case Mode_Temporarily:
			*fstream = new FileStream(tmpfile(), "tmp", Access_ReadWrite);
			break;
		}

		FILE *file;
#ifdef SECURE_CRT
#ifdef PLATFORM_UNICODE
		if (_wfopen_s(&file, filename, strMode) != 0)
			file = NULL;
#else
		if (fopen_s(&file, filename, strMode) != 0)
			file = NULL;
#endif
#else
#ifdef PLATFORM_UNICODE
		file = _wfopen(filename, strMode);
#else
		file = fopen(filename, strMode);
#endif
#endif
		if (file == NULL) {
			if (access == Access_Read)
				return Error::IO_ALREADY_IN_USE;
			else
				return Error::IO_READ_ONLY;
		}
		*fstream = new FileStream(file, filename, access);
		return Xe::Error::OK;
	}
} }