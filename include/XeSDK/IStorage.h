#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeError.h>
#include <XeSDK/IIOFileStream.h>
#include <XeSDK/IStorageDirectory.h>

namespace Xe
{
	//! \brief Input / output from files and file systems.
	namespace Storage
	{
		//! \brief Access permission to a storage item.
		enum Access {
			//! \brief Allow read data.
			Access_Read = 1,
			//! \brief Allow write and modify data.
			Access_Write = 2,
			//! \brief Allow both read and write are permitted.
			Access_ReadWrite = 3,
		};
		//! \brief Describe how to open a file
		enum Mode {
			//! \brief Always create a new file
			/** \details If a file with the same name exists, it will be
			* replaced with a 0 length file.
			*/
			Mode_Create,
			//! \brief Create a file only if it not exists
			/** \details If a file with the same name already exists, an
			* error will occur
			*/
			Mode_CreateNew,
			//! \brief Always open a file
			/** \details If a file with the same name already exists, it
			* will be opened. If it does not exists it will be created.
			*/
			Mode_Open,
			//! \brief Open a file only if it exists
			/** \details If the file does not exists, it will be returned
			* an error.
			*/
			Mode_OpenIfExists,
			//! \brief Create a temporarily file
			/** \details Create a file that will be automatically deleted
			* when it's closed. Due to caching system, a temporarily file
			* can live in memory without hard drive writing if the size
			* is small enough. filename and access parameters are ignored.
			*/
			Mode_Temporarily
		};

		class IStorage : public IObject
		{
		public:
			static const UID ID = 0x3e2a04d5874b4483ULL;

			//! \brief Get the access permission of current storage item.
			virtual Access GetAccess() const = 0;

			//! \brief Open a file.
			virtual RESULT FileOpen(IO::IFileStream **stream, ctstring filename,
				Access access, Mode mode) = 0;

			//! \brief Check if the specified file exists.
			//! \param[in] filename file's name
			//! \return true if it's found, else false
			virtual bool FileExists(ctstring filename) = 0;

			//! \brief Delete specified file.
			//! \param[in] filename file's name
			//! \return OK, FILE_NOT_FOUND, FILE_ALREADY_IN_USE
			virtual RESULT FileDelete(ctstring filename) = 0;

			//! \brief Move a file from a location to another, renaming it.
			//! \param[in] oldpath to move; if it does not exists, FILE_NOT_FOUND will be returned
			//! \param[in] newpath of path; path must not exists or FILE_ALREADY_EXISTS will be returned
			//! \return OK, FILE_NOT_FOUND, FILE_ALREADY_EXISTS or GENERIC_ERROR
			//! \details This is used to rename a file too.
			virtual RESULT FileMove(ctstring oldpath, ctstring newpath) = 0;

			//! \brief Create a directory.
			//! \param[in] path of the folder to create.
			//! \return false if an error occurred or directory already exists.
			virtual bool DrectoryCreate(ctstring path) = 0;

			//! \brief Check if a directory exists.
			virtual bool DirectoryExists(ctstring path) = 0;

			//! \brief Delete an empty directory.
			virtual RESULT DirectoryDelete(ctstring path) = 0;

			//! \brief Move or rename a directory.
			virtual RESULT DirectoryMove(ctstring oldpath, ctstring newpath) = 0;

			//! \brief Open a directory to enumerate its content.
			virtual RESULT DirectoryOpen(IDirectory** ppDirectory, ctstring path) = 0;
		};
	}
}