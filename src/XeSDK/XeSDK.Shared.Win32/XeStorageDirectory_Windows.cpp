#include "pch.h"
#include "XeStorageDirectory.h"
#include <XeSDK/XeIO.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeStoragePath.h>

namespace Xe {
	namespace Storage {
		class CDirectory : public IDirectory {
			HANDLE m_handle;
			WIN32_FIND_DATA m_findData;
			tstring m_path;

			ctstring m_name;
			s64 m_length;
			bool m_isDir;

			bool Query(IObject **obj, Xe::UID id) {
				switch (id) {
				case Xe::Storage::IDirectory::ID:
				case IObject::ID:
					this->AddRef();
					*obj = this;
					return true;
				}
				*obj = (IObject*)nullptr;
				return false;
			}
		public:
			CDirectory(HANDLE h, WIN32_FIND_DATA &findData, ctstring path) :
				m_handle(h), m_findData(findData) {
				svar len = Xe::String::GetLength(path);
				m_path = new tchar[len + 1];
				Xe::Memory::Copy(m_path, path, sizeof(tchar) * (len + 1));
			}
			~CDirectory() {
				delete[] m_path;
				FindClose(m_handle);
			}

			ctstring GetPath() const {
				return m_path;
			}
			bool Next() {
				if (FindNextFile(m_handle, &m_findData) != 0) {
					m_name = m_findData.cFileName;
					m_isDir = (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
					m_length = ((s64)m_findData.nFileSizeHigh * ((s64)MAXDWORD + 1)) + (s64)m_findData.nFileSizeLow;
					return true;
				}
				else {
					m_name = nullptr;
					m_isDir = false;
					m_length = -1;
					return false;
				}
			}
			ctstring GetName() const {
				return m_name;
			}
			bool IsDirectory() const {
				return m_isDir;
			}
			s64 GetLength() const {
				return m_length;
			}
		};
	}
}

bool Xe::Storage::Directory::Create(ctstring path) {
	if (CreateDirectory(path, nullptr) != 0)
		return Error::OK;
	switch (GetLastError()) {
	case ERROR_ALREADY_EXISTS:
		return Error::IO_ALREADY_EXISTS;
	case ERROR_PATH_NOT_FOUND:
		return Error::IO_PATH_INVALID;
	}
	return Error::GENERIC_ERROR;
}
bool Xe::Storage::Directory::Exists(ctstring path) {
	WIN32_FILE_ATTRIBUTE_DATA attributeData;
	if (GetFileAttributesEx(path, GetFileExInfoStandard, &attributeData))
		return (attributeData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	return false;
}
Xe::RESULT Xe::Storage::Directory::Delete(ctstring path) {
	if (!Exists(path))
		return Error::IO_NOT_FOUND;
	if (RemoveDirectory(path) != 0)
		return Error::OK;
	switch (GetLastError()) {
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
		return Error::IO_PATH_INVALID;
	case ERROR_ACCESS_DENIED:
		return Error::IO_ACCESS_DENIED;
	case ERROR_DIR_NOT_EMPTY:
		return Error::IO_NOT_EMPTY;
	}
	return Error::GENERIC_ERROR;
}
Xe::RESULT Xe::Storage::Directory::Move(ctstring path, ctstring newpath) {
	return Error::NOT_IMPLEMENTED;
}
Xe::RESULT Xe::Storage::Directory::GetCurrent(tstring path, uvar length) {
#if defined(PLATFORM_WIN32)
	if (GetCurrentDirectory((DWORD)length, path) != 0)
		return Error::OK;
	return Error::INVALID_PARAMETER;
#else
	return Error::NOT_IMPLEMENTED;
	//String::Copy(path, Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data(), length);
	return Error::OK;
#endif
}
Xe::RESULT Xe::Storage::Directory::SetCurrent(ctstring path) {
	if (!Exists(path))
		return Error::IO_NOT_FOUND;
#if defined(PLATFORM_WIN32)
	if (SetCurrentDirectory(path) != 0)
		return Error::OK;
	switch (GetLastError()) {
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
		return Error::IO_PATH_INVALID;
	case ERROR_ACCESS_DENIED:
		return Error::IO_ACCESS_DENIED;
	case ERROR_DIR_NOT_EMPTY:
		return Error::IO_NOT_EMPTY;
	}
	return Error::GENERIC_ERROR;
#else
	return Error::NOT_IMPLEMENTED;
#endif
}

Xe::RESULT Xe::Storage::Directory::Open(IDirectory **directory, ctstring path) {
	tstring str;
	svar len = String::GetLength(path);
	bool alloc;
	if (len > 0)
	{
		if (!Path::IsValid(path))
			return Error::IO_PATH_INVALID;
		svar index = len;
		str = new tchar[len + 3];
		Memory::Copy(str, path, sizeof(tchar) * len);
		switch (str[index - 1])
		{
		case '\\':
		case '/':
			break;
		default:
			str[index++] = '\\';
		}
		str[index++] = '*';
		str[index++] = '\0';
		alloc = true;
	}
	else {
		str = _T("*");
		alloc = false;
	}

	RESULT r;
	WIN32_FIND_DATA findData;
	HANDLE h = FindFirstFileEx(str, (FINDEX_INFO_LEVELS)1/*FindExInfoBasic*/, &findData, FindExSearchNameMatch, nullptr, 0);
	if (h != INVALID_HANDLE_VALUE) {
		*directory = new CDirectory(h, findData, path);
		r = Error::OK;
	}
	else {
		r = Error::GENERIC_ERROR;
	}

	if (alloc)
		delete[] str;
	return r;
}