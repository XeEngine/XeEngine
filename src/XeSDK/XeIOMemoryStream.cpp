#include "pch.h"
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeIO.h>
#include <XeSDK/XeMath.h>

namespace Xe { namespace IO {
    class CMemoryStream : public IMemoryStream {
        u8* m_Start;
        u8* m_End;
		u8* m_CapacityEnd;
        u8* m_Current;
        bool m_Created;
        bool m_Resizable;
        bool m_Writable;

    public:
        CMemoryStream() :
			m_Start((u8*)nullptr),
			m_End((u8*)nullptr),
			m_CapacityEnd((u8*)nullptr),
			m_Current((u8*)nullptr),
			m_Created(false),
			m_Resizable(false),
			m_Writable(false)
		{ }

        ~CMemoryStream()
		{
            if (m_Created)
                Memory::Free(m_Start);

            m_Start = (u8*)nullptr;
            m_End = (u8*)nullptr;
            m_Current = (u8*)nullptr;
            m_Created = false;
            m_Resizable = false;
        }

        bool Open(const void* memory, svar length, bool free)
		{
            m_Start = (u8*)memory;
            m_End = m_Start + length;
            m_Current = m_Start;
            m_Created = !free;
            m_Resizable = false;
            m_Writable = false;
            return true;
        }

        bool Open(void* memory, svar length, bool free)
		{
            m_Start = (u8*)memory;
            m_End = m_Start + length;
            m_Current = m_Start;
            m_Created = !free;
            m_Resizable = false;
            m_Writable = true;
            return true;
        }

        bool Create(svar capacity, bool resizable)
		{
			RequireCapacity(capacity);
			m_End = m_Start + capacity;

            m_Created = true;
            m_Resizable = resizable;
			m_Writable = true;
            return true;
        }

        void Flush()
		{
			SetCapacity(GetLength());
        }

        void* GetMemory()
		{
            return m_Start;
        }

        s64 GetPosition() const
		{
            return m_Current - m_Start;
        }

        RESULT SetPosition(s64 offset, IO::Seek seek)
        {
            switch (seek)
            {
                case Seek_Set:
                    if (offset < 0)
                        return Error::IO_POSITION_INVALID;
                    m_Current = m_Start + offset;
                    break;
                case Seek_Cur:
                    if (m_Current - m_Start + offset < 0)
                        return Error::IO_POSITION_INVALID;
                    m_Current += offset;
                    break;
                case Seek_End:
                    if (m_End - m_Start + offset < 0)
                        return Error::IO_POSITION_INVALID;
                    m_Current = m_End + offset;
                    break;
                default:
                    return Error::IO_SEEK_MODE_INVALID;
            }

            return Error::OK;
        }

        s64 GetLength() const
		{
            return m_End - m_Start;
        }

        RESULT SetLength(s64 offset)
		{
            if (!CanWrite())
                return Error::IO_READ_ONLY;

            if (!CanOverflow())
                return Error::IO_NOT_RESIZED;

			RequireCapacity(offset);
			m_End = m_Start + offset;

            return Error::OK;
        }

		size_t GetCapacity() const
		{
			return m_CapacityEnd - m_Start;
		}

		bool SetCapacity(size_t capacity)
		{
			u8* newMemCapacity = (u8*)Memory::Resize(m_Start, capacity);
			LOGFA(!!newMemCapacity);

			if (!newMemCapacity)
				return false;

			m_Current = newMemCapacity + GetPosition();
			m_End = newMemCapacity + GetLength();
			m_CapacityEnd = newMemCapacity + capacity;
			m_Start = newMemCapacity;
			return true;
		}

		bool RequireCapacity(size_t requiredCapacity)
		{
			auto currentCapacity = Xe::Math::Max<size_t>(GetCapacity(), 256);
			while (currentCapacity < requiredCapacity)
				currentCapacity *= 2;

			if (currentCapacity > GetCapacity())
			{
				return SetCapacity(currentCapacity);
			}

			return true;
		}

        bool CanRead() const
		{
            return true;
        }

        bool CanWrite() const
		{
            return m_Writable;
        }

        bool CanSeek() const
		{
            return true;
        }

        bool CanOverflow() const
		{
            return CanWrite() && m_Resizable;
        }

        svar Read(void* data, svar offset, s32 size)
		{
			if (!CanRead())
				return 0;

            svar remains = (svar)(GetLength() - GetPosition());
			remains = Xe::Math::Max(0, remains);

            if (size > remains)
                size = remains;

			if (size > 0)
			{
				Memory::Copy((u8*)data + offset, m_Current, size);
				m_Current += size;
			}

            return size;
        }

        svar Write(const void* data, svar offset, s32 size)
		{
			if (!CanWrite() || size <= 0)
				return 0;

			auto position = GetPosition();
			auto length = GetLength();
            auto remains = length - position;

			if (size > remains)
			{
				if (CanOverflow())
				{
					RequireCapacity(position + size);
					m_End = m_Start + position + size;
				}
				else
				{
					size = remains;
				}
			}

			if (size > 0)
			{
				Memory::Copy(m_Current, (u8*)data + offset, size);
				m_Current += size;
			}
                
			return size;
        }
    };

    IMemoryStream* IMemoryStream::From(void* memory, svar length, bool free)
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
    IMemoryStream* IMemoryStream::From(const void* memory, svar length, bool free)
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

} }
