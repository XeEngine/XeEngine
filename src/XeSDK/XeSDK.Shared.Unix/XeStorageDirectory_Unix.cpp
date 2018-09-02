#include "pch.h"
#include "XeStorageDirectory.h"

#include <XeSDK/XeStoragePath.h>
#include <XeSDK/XeIO.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeString.h>

#include <cerrno>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace Xe {
	namespace Storage {
		class CDirectory : public IDirectory {
			DIR *m_dir;
			tstring m_path;
			struct dirent *m_dirent;

			ctstring m_name;
			s64 m_length;
			bool m_isDir;

			tchar *m_pathbuf;
			svar m_pathlen;

		public:
			CDirectory(DIR *h, ctstring path) : m_dir(h) {
				m_pathlen = Xe::String::GetLength(path);
				m_path = new tchar[m_pathlen + 1];
				Memory::Copy(m_path, path, sizeof(tchar) * (m_pathlen + 1));
				m_pathbuf = new tchar[m_pathlen + lengthof(((dirent*)0)->d_name) + 1];
				Memory::Copy(m_pathbuf, path, sizeof(tchar) * (m_pathlen + 1));
				Next();
			}
			~CDirectory() {
				delete[] m_pathbuf;
				delete[] m_path;
				closedir(m_dir);
			}

			ctstring GetPath() const {
				return m_path;
			}
			bool Next() {
				m_dirent = readdir(m_dir);
				if (m_dirent == nullptr) {
					m_name = (ctstring)nullptr;
					m_isDir = false;
					m_length = -1;
				}
				m_name = m_dirent->d_name;
				Memory::Copy(m_pathbuf + m_pathlen, m_dirent->d_name, lengthof(((dirent*)0)->d_name));
				struct stat status;
				if (stat(m_pathbuf, &status) == 0) {
					m_isDir = S_ISDIR(status.st_mode);
					m_length = m_isDir ? -1LL : status.st_size;
				}
				else {
					m_isDir = m_dirent->d_type == DT_DIR;
					m_length = -1LL;
				}
				return true;
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

extern Xe::RESULT Storage_Unix_GetError();

bool Xe::Storage::Directory::Create(ctstring path) {
	if (Exists(path))
		return false;
#if defined(PLATFORM_UNICODE)
	if (_wmkdir(path) == 0)
#else
	if (mkdir(path, (S_ISUID | S_ISGID | S_ISVTX) | (S_IRWXU | S_IRWXG | S_IRWXO)) == 0)
#endif
		return Error::OK;
	return Storage_Unix_GetError();
}
bool Xe::Storage::Directory::Exists(ctstring path) {
	struct stat s;
	int err = stat(path, &s);
	if (-1 == err) {
		if (errno == ENOENT)
			return false;
		else
			return false;
	}
	else {
		if (S_ISDIR(s.st_mode))
			return true;
		else
			return false;
	}
}
Xe::RESULT Xe::Storage::Directory::Delete(ctstring path) {
	if (!Exists(path))
		return Error::IO_NOT_FOUND;
	return rmdir(path) == 0 ? Error::OK : Storage_Unix_GetError();
}
Xe::RESULT Xe::Storage::Directory::Move(ctstring path, ctstring newpath) {
	if (!Exists(path))
		return Error::IO_NOT_FOUND;
	if (Exists(newpath))
		return Error::IO_ALREADY_EXISTS;
#if defined(PLATFORM_ANDROID)
#else
	if (rename(path, newpath) == 0)
		return Error::OK;
#endif
	return Error::GENERIC_ERROR;
}
Xe::RESULT Xe::Storage::Directory::GetCurrent(tstring path, uvar length) {
#if defined(PLATFORM_UNICODE)
	if (_wgetcwd(path, length) != nullptr)
#else
	if (getcwd(path, length) != nullptr)
#endif
		return Error::OK;
	return Storage_Unix_GetError();
}
Xe::RESULT Xe::Storage::Directory::SetCurrent(ctstring path) {
	if (!Exists(path))
		return Error::IO_NOT_FOUND;
#if defined(PLATFORM_UNICODE)
	if (_wchdir(path) == 0)
#else
	if (chdir(path) == 0)
#endif
		return Error::OK;
	return Storage_Unix_GetError();
}

Xe::RESULT Xe::Storage::Directory::Open(IDirectory **directory, ctstring path) {
	*directory = (IDirectory*)nullptr;
	if (Path::IsValid(path))
		return Error::IO_PATH_INVALID;
	if (!Exists(path))
		return Error::IO_NOT_FOUND;
	DIR *dir = opendir(path);
	if (dir == nullptr)
		return Error::GENERIC_ERROR;
	*directory = new CDirectory(dir, path);
	return Error::OK;
}