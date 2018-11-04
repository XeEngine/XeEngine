#include "pch.h"
#include <XeSDK/XeStorageContainerXefs.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeIO.h>
#include <XeSDK/XeCryptoFnv1a.h>

namespace Xe {
    namespace Storage {
        namespace Container {

            // 1 HASH
            // 2 FILENAME
            // 4 ARCHIVE INFORMATION
            // 8
            // 10 CREATION TIME
            // 20 MODIFIED TIME
            // 40 LAST ACCESS TIME
            // 80
            // 100 LZ77 COMPRESSION
            // 1000 ENCRYPT TYPE 1
            // 10000 ALIGN 512
            // 20000 ALIGN 1024
            // 30000 ALIGN 2048
            class CXefs : public Xefs {
                enum Content {
                    Content_Hash = 0x00000001,
                    Content_Filename = 0x00000002,
                };
                struct Lba {
                    s32 off;
                    s32 len;
                };

                bool m_isValid;
                IO::IStream *m_stream;
                u32 m_header;
                u32 m_flags;
                s32 m_count;
                u32 m_reserved;

                Lba *m_lba;
                u64 *m_hash;
                Xe::String *m_filename;

                class FileStream : public IO::IFileStream {
                    IO::IStream *m_stream;
                    Access m_access;
                    ctstring m_filename;
                    s64 m_offset;
                    s64 m_length;

                    s64 m_pos;

                    void Flush() {
                        m_stream->Flush();
                    }
                    s64 GetPosition() const {
                        return m_pos;
                    }
                    RESULT SetPosition(s64 offset, IO::Seek seek = IO::Seek_Set) {
                        s64 oldpos = GetPosition();
                        switch (seek) {
                            case IO::Seek_Set:
                                m_pos = offset;
                                break;
                            case IO::Seek_Cur:
                                m_pos += offset;
                                break;
                            case IO::Seek_End:
                                m_pos = GetLength() + offset;
                                break;
                            default:
                                return Error::IO_SEEK_MODE_INVALID;
                        }
                        if (m_pos < 0LL) {
                            m_pos = oldpos;
                            return Error::IO_POSITION_INVALID;
                        }
                        return Error::OK;
                    }
                    s64 GetLength() const {
                        return m_length;
                    }
                    RESULT SetLength(s64 offset) {
                        if (!CanOverflow())
                            return Error::IO_WRITE_FAILED;
                        return Error::NOT_IMPLEMENTED;
                    }
                    bool CanRead() const {
                        return (m_access & Access_Read) != 0;
                    }
                    bool CanWrite() const {
                        return (m_access & Access_Write) != 0;
                    }
                    bool CanSeek() const {
                        return true;
                    }
                    bool CanOverflow() const {
                        return false;
                    }
                    svar Read(void* data, svar offset, s32 length) {
                        if (!CanRead())
                            return Error::IO_READ_FAILED;
                        if (GetPosition() + length > GetLength())
                            length = (s32)(GetLength() - GetPosition());
                        m_stream->SetPosition(m_offset + m_pos);
                        length = m_stream->Read(data, offset, length);
                        m_pos += length;
                        return length;
                    }
                    svar Write(const void* data, svar offset, s32 length) {
                        if (!CanWrite())
                            return Error::IO_WRITE_FAILED;
                        if (GetPosition() + length > GetLength())
                            length = (s32)(GetLength() - GetPosition());
                        m_stream->SetPosition(m_offset + m_pos);
                        length = m_stream->Write(data, offset, length);
                        m_pos += length;
                        return length;
                    }
                    ctstring GetFilename() const {
                        return m_filename;
                    }
                public:
                    FileStream(IO::IStream *stream, ctstring filename, s64 offset, s64 length, Access access) :
                            m_stream(stream), m_access(access), m_filename(filename),
                            m_offset(offset), m_length(length), m_pos(0) {
                        m_stream->AddRef();
                    }
                    ~FileStream() {
                        m_stream->Release();
                    }
                };

                svar HashSearchLinear(u64 hash) const {
                    for (svar i = 0; i < m_count; i++) {
                        if (m_hash[i] == hash)
                            return true;
                    }
                    return false;
                }
                svar ObtainIndexFromFilename(ctstring filename) const {
                    /* Questo metodo permette di ritornare l'indice di un file
                     * a partire dal suo nome o percorso. Per fare in modo che
                     * la ricerca abbia effetto, all'interno del File System
                     * deve essere conteneuto almeno l'Hash o i Filenames.
                     */
                    if (!(m_flags & (Content_Hash | Content_Filename)))
                        return -1;

                    /* da notare che il file system � case-sensitive, ma
                     * i percorsi in UNICODE verranno ritrasportati in
                     * UTF-8 se PLATFORM_UNICODE � definito.
                     */
                    svar len = StringSpan(filename).GetLength();
                    const char *fname = new char[len + 1];
#if defined(PLATFORM_UNICODE)
                    for (svar i = 0; i < len; i++) {
							((char*)fname)[i] = String::WideToChar(filename[i]);
						}
						((char*)fname)[len] = '\0';
#else
                    fname = filename;
#endif

                    /* se Content_Hash non � specificato, significa che la
                     * tabella degli hash non � pre-calcolata all'interno del
                     * file, ma comunque calcolata all'apertura del FileSystem
                     * per motivi di performance sulla ricerca di un file.
                     */
                    u64 hash = Xe::Crypto::Fnv1a64(fname, len);

                    /* Ricordiamoci di rilasciare la stringa nel caso il nome
                     * del file venga convertito da UNICODE ad UTF-8
                     */
#if defined(PLATFORM_UNICODE)
                    delete[] fname;
#endif

                    /** E' tutto pronto, si parte per la ricerca dell'hash!
                     * Richiamare HashSearchBinary solo se m_hash � un array
                     * ordinato in modo crescente.
                     */
                    return HashSearchLinear(hash);
                }

                Access GetAccess() const {
                    return Access_Read;
                }
                RESULT FileOpen(IO::IFileStream **stream, ctstring filename,
                                Access access, Mode mode)
                {
                    switch (access) {
                        case Access_Read:
                            break;
                        case Access_Write:
                        case Access_ReadWrite:
                            return Error::IO_READ_ONLY;
                        default:
                            return Error::INVALID_PARAMETER;
                    }
                    switch (mode) {
                        case Mode_Create:
                        case Mode_CreateNew:
                        case Mode_Open:
                        case Mode_Temporarily:
                            return Error::IO_READ_ONLY;
                        case Mode_OpenIfExists:
                            break;
                        default:
                            return Error::INVALID_PARAMETER;
                    }
                    svar index = ObtainIndexFromFilename(filename);
                    if (index < 0)
                        return Error::IO_NOT_FOUND;
                    *stream = new FileStream(m_stream, filename, m_lba[index].off, m_lba[index].len, access);
                    return Error::OK;
                }
                bool FileExists(ctstring filename) {
                    return ObtainIndexFromFilename(filename) >= 0;
                }
                RESULT FileDelete(ctstring filename) {
                    return Error::IO_READ_ONLY;
                }
                RESULT FileMove(ctstring oldpath, ctstring newpath) {
                    return Error::IO_READ_ONLY;
                }
                bool DrectoryCreate(ctstring path) {
                    return false;
                }
                bool DirectoryExists(ctstring path) {
                    return false;
                }
                RESULT DirectoryDelete(ctstring path) {
                    return Error::IO_READ_ONLY;
                }
                RESULT DirectoryMove(ctstring oldpath, ctstring newpath) {
                    return Error::IO_NOT_FOUND;
                }
                RESULT DirectoryOpen(IDirectory** ppDirectory, ctstring path) {
                    return Error::IO_NOT_FOUND;
                }

                void Delete() {
                    if (m_lba)
                        delete[] m_lba;
                    if (m_hash)
                        delete[] m_hash;
                    if (m_filename)
                        delete[] m_filename;
                    m_stream->Release();
                }
            public:
                CXefs(IO::IStream *stream) :
                        m_isValid(false),
                        m_stream(stream),
                        m_lba((Lba*)nullptr),
                        m_hash((u64*)nullptr),
                        m_filename((String*)nullptr)
                {
                    m_stream->AddRef();
                    if (stream->Read(&m_header, 0, sizeof(m_header)) != 4) return;
                    if (m_header != *(u32*)"XFS\x01") return;
                    if (stream->Read(&m_flags, 0, sizeof(m_header)) != 4) return;
                    if (stream->Read(&m_count, 0, sizeof(m_header)) != 4) return;
                    if (stream->Read(&m_reserved, 0, sizeof(m_header)) != 4) return;

                    m_lba = new Lba[m_count];
                    if (stream->Read(m_lba, 0, sizeof(Lba) * m_count) != sizeof(Lba) * m_count) {
                        Delete();
                        return;
                    }

                    if (m_flags & Content_Hash) {
                        m_hash = new u64[m_count];
                        if (m_hash == nullptr) {
                            Delete();
                            return;
                        }
                        if (stream->Read(m_hash, 0, sizeof(u64) * m_count) != sizeof(u64) * m_count) {
                            Delete();
                            return;
                        }
                    }
                    if (m_flags & Content_Filename) {
                        u8 len;
                        char tmp[0x100];
                        m_filename = new String[m_count];
                        if (m_filename == nullptr) {
                            Delete();
                            return;
                        }
                        stream->Read(tmp, 0, 4);
                        for (s32 i = 0; i < m_count; i++) {
                            if (stream->Read(&len, 0, 1) != 1) {
                                Delete();
                                return;
                            }
                            if (stream->Read(tmp, 0, len) != len) {
                                Delete();
                                return;
                            }
                            tmp[len] = '\0';
                            m_filename[i] = tmp;
                        }
                    }
                    m_isValid = true;
                }
                ~CXefs() {
                    Delete();
                }
                bool IsValid() const {
                    return m_isValid;
                }
            };

        }
    }
}

bool Xe::Storage::Container::Xefs::Open(Xefs **xfs, IO::IStream *stream)
{
    stream->AddRef();
    CXefs *_xfs = new CXefs(stream);
    if (_xfs->IsValid()) {
        stream->Release();
        *xfs = _xfs;
        return true;
    }
    *xfs = (Xefs*)nullptr;
    _xfs->Release();
    stream->Release();
    return false;
}