#include "pch.h"
#include <XeSDK/XeStorage.h>
#include "XeStorageFile.h"
#include "XeStorageDirectory.h"

namespace Xe {
    namespace Storage {
        bool GetStorage(IStorage **pStorage, StorageType type) {
            class CStorage : public IStorage {
            public:
                bool Query(IObject **obj, UID id) {
                    switch (id) {
                        case IStorage::ID:
                        case IObject::ID:
                            *obj = this;
                            AddRef();
                            return true;
                    }
                    *obj = nullptr;
                    return false;
                }

                Access GetAccess() const {
                    return Access_ReadWrite;
                }
                RESULT FileOpen(IO::IFileStream **stream, ctstring filename,
                                Access access, Mode mode) {
                    return Xe::Storage::File::Open(stream, filename, access, mode);
                }
                bool FileExists(ctstring filename) {
                    return File::Exists(filename);
                }
                RESULT FileDelete(ctstring filename) {
                    return File::Delete(filename);
                }
                RESULT FileMove(ctstring oldpath, ctstring newpath) {
                    return File::Move(oldpath, newpath);
                }
                bool DrectoryCreate(ctstring path) {
                    return Directory::Create(path);
                }
                bool DirectoryExists(ctstring path) {
                    return Directory::Exists(path);
                }
                RESULT DirectoryDelete(ctstring path) {
                    return Directory::Delete(path);
                }
                RESULT DirectoryMove(ctstring oldpath, ctstring newpath) {
                    return Directory::Move(oldpath, newpath);
                }
                RESULT DirectoryOpen(IDirectory** ppDirectory, ctstring path) {
                    return Directory::Open(ppDirectory, path);
                }
            };
            *pStorage = new CStorage;
            return true;
        }
    }
}