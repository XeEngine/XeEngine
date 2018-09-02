#include "pch.h"
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeIO.h>

namespace Xe {
    namespace IO {
        class CMemoryStream : public IMemoryStream {
            u8* m_start;
            u8* m_end;
            u8* m_cur;
            bool m_created;
            bool m_resizable;
            bool m_writable;

        public:
            CMemoryStream() {
                m_start = (u8*)nullptr;
                m_end = (u8*)nullptr;
                m_cur = (u8*)nullptr;
                m_created = false;
                m_resizable = false;
            }
            ~CMemoryStream() {
                if (m_created)
                    Memory::Free(m_start);
                m_start = (u8*)nullptr;
                m_end = (u8*)nullptr;
                m_cur = (u8*)nullptr;
                m_created = false;
                m_resizable = false;
            }

            bool Open(const void* memory, svar length, bool free) {
                m_start = (u8*)memory;
                m_end = m_start + length;
                m_cur = m_start;
                m_created = !free;
                m_resizable = false;
                m_writable = false;
                return true;
            }
            bool Open(void* memory, svar length, bool free) {
                m_start = (u8*)memory;
                m_end = m_start + length;
                m_cur = m_start;
                m_created = !free;
                m_resizable = false;
                m_writable = true;
                return true;
            }
            bool Create(svar length, bool resizable) {
                m_start = (u8*)Memory::AllocZero(length);
                if (m_start == nullptr)
                    return false;
                m_end = m_start + length;
                m_cur = m_start;
                m_created = true;
                m_resizable = false;
                return true;
            }

            void Flush() {
            }
            void* GetMemory() {
                return m_start;
            }
            s64 GetPosition() const {
                return m_cur - m_start;
            }
            RESULT SetPosition(s64 offset, IO::Seek seek)
            {
                switch (seek)
                {
                    case Seek_Set:
                        if (offset < 0 || (CanOverflow() && offset >= GetLength()))
                            return Error::IO_POSITION_INVALID;
                        m_cur = m_start + offset;
                        break;
                    case Seek_Cur:
                        if (m_cur - m_start + offset < 0 || (CanOverflow() && m_start - m_cur + offset >= GetLength()))
                            return Error::IO_POSITION_INVALID;
                        m_cur += offset;
                        break;
                    case Seek_End:
                        if (m_end - m_start + offset < 0 || (CanOverflow() && m_start - m_end + offset >= GetLength()))
                            return Error::IO_POSITION_INVALID;
                        m_cur = m_end + offset;
                        break;
                    default:
                        return Error::IO_SEEK_MODE_INVALID;
                }
                return Error::OK;
            }
            s64 GetLength() const {
                return m_end - m_start;
            }
            RESULT SetLength(s64 offset) {
                if (!CanWrite())
                    return Error::IO_READ_ONLY;
                if (!CanOverflow())
                    return Error::IO_NOT_RESIZED;
                u8* newmem = (u8*)Memory::Resize(m_start, (uvar)offset);
                if (newmem != nullptr)
                {
                    m_cur = newmem + (m_cur - m_start);
                    m_end = newmem + offset;
                    m_start = newmem;
                    return Error::OK;
                }
                return Error::GENERIC_ERROR;
            }
            bool CanRead() const {
                return true;
            }
            bool CanWrite() const {
                return m_writable;
            }
            bool CanSeek() const {
                return true;
            }
            bool CanOverflow() const {
                return CanWrite() && m_resizable;
            }

            svar Read(void* data, svar offset, s32 size) {
                svar remains = (svar)(GetLength() - GetPosition());
                if (size > remains)
                    size = remains;
                Memory::Copy((u8*)data + offset, m_cur, size);
                m_cur += size;
                return size;
            }
            svar Write(const void* data, svar offset, s32 size) {
                svar remains = (svar)(GetLength() - GetPosition());
                if (size > remains)
                {
                    if (CanOverflow())
                    {
                        m_end += (size - remains);
                        u8* newmem = (u8*)Memory::Resize(m_start, (uvar)GetLength());
                        if (newmem != nullptr)
                        {
                            m_cur = newmem + (m_cur - m_start);
                            m_end = newmem + GetLength();
                            m_start = newmem;
                        }
                        else
                        {
                            size = remains;
                        }
                    }
                    else
                    {
                        size = remains;
                    }
                }
                Memory::Copy(m_cur, (u8*)data + offset, size);
                m_cur += size;
                return size;
            }
        };

        IMemoryStream* IMemoryStream::New(void* memory, svar length, bool free)
        {
            CMemoryStream* p = new CMemoryStream;
            if (p != nullptr)
            {
                if (p->Open(memory, length, free) == false)
                {
                    delete p;
                    p = (CMemoryStream*)nullptr;
                }
            }
            return p;
        }
        IMemoryStream* IMemoryStream::New(const void* memory, svar length, bool free)
        {
            CMemoryStream* p = new CMemoryStream;
            if (p != nullptr)
            {
                if (p->Open(memory, length, free) == false)
                {
                    delete p;
                    p = (CMemoryStream*)nullptr;
                }
            }
            return p;
        }
        IMemoryStream* IMemoryStream::New(svar length, bool resizable)
        {
            CMemoryStream* p = new CMemoryStream;
            if (p != nullptr)
            {
                if (p->Create(length, resizable) == false)
                {
                    delete p;
                    p = (CMemoryStream*)nullptr;
                }
            }
            return p;
        }

    }
}
