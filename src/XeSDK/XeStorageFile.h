#pragma once
#include <XeSDK/IStorage.h>

namespace Xe {
    namespace Storage {
        namespace File {
            RESULT Open(IO::IFileStream **stream, ctstring filename,
                        Access access, Mode mode);

            //! \brief check if the specified file exists
            //! \param[in] filename file's name
            //! \return true if it's found, else false
            bool Exists(ctstring filename);

            //! \brief delete specified file
            //! \param[in] filename file's name
            //! \return OK, FILE_NOT_FOUND, FILE_ALREADY_IN_USE
            RESULT Delete(ctstring filename);

            //! \brief move a file from a location to another, renaming it
            //! \param[in] filename to move; if it does not exists, FILE_NOT_FOUND will be returned
            //! \param[in] newfilename of filename; path must not exists or FILE_ALREADY_EXISTS will be returned
            //! \return OK, FILE_NOT_FOUND, FILE_ALREADY_EXISTS or GENERIC_ERROR
            //! \details This is used to rename a file too.
            RESULT Move(ctstring oldpath, ctstring newpath);
        }
    }
}