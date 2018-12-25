#include "pch.h"
#include "XeStorageFile.h"
#include <XeSDK/XeIO.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Storage {
	class FileStream : public IO::IFileStream {
		HANDLE m_handle;
		Access m_access;
		String m_filename;

		void Flush() {
			FlushFileBuffers(m_handle);
		}
		s64 GetPosition() const {
			LARGE_INTEGER l;
			l.QuadPart = 0;
			if (SetFilePointerEx(m_handle, l, &l, FILE_CURRENT) != 0)
				return l.QuadPart;
			else
				return -1;
		}
		RESULT SetPosition(s64 offset, IO::Seek seek = IO::Seek_Set) {
			LARGE_INTEGER l;
			l.QuadPart = offset;
			if (SetFilePointerEx(m_handle, l, nullptr, (DWORD)seek) != 0)
				return Error::OK;
			else {
				switch (seek) {
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
#if defined(PLATFORM_WIN32)
			LARGE_INTEGER l;
			return GetFileSizeEx(m_handle, &l) != 0 ?
				l.QuadPart : -1;
#else
			FILE_STANDARD_INFO info;
			return GetFileInformationByHandleEx(m_handle, FileStandardInfo, &info, sizeof(info)) != 0 ?
				info.EndOfFile.QuadPart : -1;
#endif
		}
		RESULT SetLength(s64 offset) {
			if (CanWrite()) {
				SetPosition(offset);
				return SetEndOfFile(m_handle) == TRUE ?
					Error::OK : Error::GENERIC_ERROR;
			}
			return Error::IO_READ_ONLY;
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
			DWORD n;
			if (ReadFile(m_handle, (u8*)data + offset, (DWORD)length, &n, nullptr) == FALSE)
				return -1;
			else
				return n;
		}
		svar Write(const void* data, svar offset, s32 length) {
			if (!CanWrite())
				return -1;
			DWORD n;
			if (WriteFile(m_handle, (u8*)data + offset, (DWORD)length, &n, nullptr) == FALSE)
				return -1;
			else
				return n;
		}

		const String& GetFileName() const
		{
			return m_filename;
		}

	public:
		FileStream(HANDLE handle, const String& filename, Access access) :
			m_handle(handle),
			m_access(access),
			m_filename(filename)
		{ }

		~FileStream() {
			CloseHandle(m_handle);
		}
	};

	bool File::Exists(ctstring filename)
	{
		WIN32_FILE_ATTRIBUTE_DATA attributeData;
		if (GetFileAttributesExA(filename, GetFileExInfoStandard, &attributeData))
			return (attributeData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
		return false;
	}
	RESULT File::Delete(ctstring filename)
	{
		if (DeleteFileA(filename) == 0)
		{
			DWORD err = GetLastError();
			switch (err)
			{
			case ERROR_FILE_NOT_FOUND:
				return Error::IO_NOT_FOUND;
			case ERROR_ACCESS_DENIED:
				return Error::IO_ALREADY_IN_USE;
			default:
				return Error::GENERIC_ERROR;
			}
		}
		return Xe::Error::OK;
	}
	RESULT File::Move(ctstring newfilename, ctstring oldfilename)
	{
		if (!Exists(oldfilename))
			return Error::IO_NOT_FOUND;
		if (Exists(newfilename))
			return Error::IO_ALREADY_EXISTS;
#if WINAPI_FAMILY != WINAPI_FAMILY_TV_TITLE
		// MoveFileExA cannot be linked when using Durango SDK
		if (MoveFileExA(oldfilename, newfilename, MOVEFILE_COPY_ALLOWED) == 0)
			return Error::OK;
#endif
		return Error::GENERIC_ERROR;
	}

	RESULT File::Open(IO::IFileStream **fstream, ctstring filename, Access access, Mode mode) {
		static const DWORD DESIDER_ACCESS[]{
			0,
			GENERIC_READ,
			GENERIC_WRITE,
			GENERIC_READ | GENERIC_WRITE
		};
		static const DWORD SHARE_MODE[]{
			0,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_SHARE_READ,
			0,
		};
		static const DWORD CREATION_DISPOSITION[]{
			CREATE_ALWAYS,
			CREATE_NEW,
			OPEN_ALWAYS,
			OPEN_EXISTING,
			0,
		};
#if defined(PLATFORM_WIN32)
		TCHAR tmpfilename[MAX_PATH + 1];
#endif

		*fstream = nullptr;
		switch (access) {
		case Access_Read:
		case Access_Write:
		case Access_ReadWrite:
			break;
		default:
			return Error::INVALID_PARAMETER;
		}
		switch (mode) {
		case Mode_Create:
		case Mode_CreateNew:
		case Mode_Open:
		case Mode_OpenIfExists:
		case Mode_Temporarily:
			break;
		default:
			return Error::INVALID_PARAMETER;
		}
		switch (mode) {
		case Mode_CreateNew:
			if (Exists(filename))
				return Error::IO_ALREADY_EXISTS;
			break;
		case Mode_OpenIfExists:
			if (!Exists(filename))
				return Error::IO_NOT_FOUND;
			break;
		case Mode_Temporarily:
#if defined(PLATFORM_WIN32)
		{
			TCHAR path[MAX_PATH + 1];
			DWORD ret = GetTempPath(MAX_PATH, path);
			if (ret >= MAX_PATH || ret == 0)
				return Error::GENERIC_ERROR;
			UINT uRet = GetTempFileName(path, _T(""), 0, tmpfilename);
			filename = tmpfilename;
		}
#else
			//filename = Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data();
			return Error::NOT_IMPLEMENTED;
#endif
			break;
		}

		HANDLE handle = INVALID_HANDLE_VALUE;
		DWORD dwShareMode;
		DWORD dwCreationDisposition;
		DWORD dwFlags;
		if (mode == Mode_Temporarily)
		{
			access = Access_ReadWrite;
			dwShareMode = 0;
			dwCreationDisposition = CREATE_ALWAYS;
			dwFlags = FILE_ATTRIBUTE_TEMPORARY;
		}
		else
		{
			dwShareMode = SHARE_MODE[access];
			dwCreationDisposition = CREATION_DISPOSITION[mode];
			dwFlags = 0;
		}
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		handle = CreateFile(filename,    // lpFileName
			DESIDER_ACCESS[access],            // dwDesiredAccess
			dwShareMode,                       // dwShareMode
			nullptr,                           // lpSecurityAttributes
			dwCreationDisposition,             // dwCreationDisposition
			dwFlags,                           // dwFlagsAndAttributes
			nullptr);                          // hTemplateFile
#else
		CREATEFILE2_EXTENDED_PARAMETERS exParams = { 0 };
		exParams.dwSize = sizeof(exParams);
		exParams.dwFileAttributes = dwFlags;

		if (sizeof(tchar) == sizeof(wchar_t))
		{
			handle = CreateFile2((wchar_t*)filename, DESIDER_ACCESS[access],
				dwShareMode, dwCreationDisposition, &exParams);
		}
		else
		{
			int filenameLength = Xe::String::GetLength(filename);
			wchar_t* filenameW = new wchar_t[filenameLength + 1];
			mbstowcs(filenameW, filename, filenameLength + 1);

			handle = CreateFile2(filenameW, DESIDER_ACCESS[access],
				dwShareMode, dwCreationDisposition, &exParams);

			delete[] filenameW;
		}
#endif
		if (handle != INVALID_HANDLE_VALUE) {
			*fstream = new FileStream(handle, String(filename), access);
			return Error::OK;
		}
		else
		{
			switch (GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
				return Xe::Error::IO_NOT_FOUND;
			case ERROR_ACCESS_DENIED:
			case ERROR_SHARING_VIOLATION:
				return access == Access_Read ? Xe::Error::IO_ALREADY_IN_USE : Xe::Error::IO_READ_ONLY;
			}
			return Xe::Error::GENERIC_ERROR;
		}
	}
} }
