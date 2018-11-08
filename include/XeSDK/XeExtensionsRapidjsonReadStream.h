#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filewritestream.h>
#include <XeSDK/IIoStream.h>

namespace rapidjson
{
	class XeReadStream
	{
		Xe::IO::IStream* m_Stream;
		mutable int m_LastReadChar;
	public:
		typedef char Ch;    //!< Character type (byte).

		XeReadStream(Xe::IO::IStream* pStream) :
			m_Stream(pStream)
		{
			RAPIDJSON_ASSERT(!!pStream);
			RAPIDJSON_ASSERT(pStream->CanRead());
			m_Stream->AddRef();

			if (m_Stream->Read(&m_LastReadChar, 0, 1) == 0)
			{
				m_LastReadChar = '\0';
			}
		}

		~XeReadStream()
		{
			m_Stream->Release();
		}


		Ch Peek() const
		{
			return m_LastReadChar;
		}

		Ch Take()
		{
			Ch c = m_LastReadChar;
			m_Stream->Read(&m_LastReadChar, 0, 1);
			return c;
		}

		size_t Tell() const
		{
			return m_Stream->GetPosition();
		}

		// Not implemented
		void Put(Ch ch) { RAPIDJSON_ASSERT(false); }
		void Flush() { m_Stream->Flush(); }
		Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
		size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

		// For encoding detection only.
		const Ch* Peek4() const
		{
			if (m_Stream->Read(&m_LastReadChar, 0, 4) != 4)
			{
				m_LastReadChar = 0;
			}

			return (Ch*)&m_LastReadChar;
		}
	};

	class XeWriteStream
	{
		Xe::IO::IStream* m_Stream;
	public:
		typedef char Ch;    //!< Character type (byte).

		XeWriteStream(Xe::IO::IStream* pStream) :
			m_Stream(pStream)
		{
			RAPIDJSON_ASSERT(!!pStream);
			RAPIDJSON_ASSERT(pStream->CanWrite());
			m_Stream->AddRef();
		}

		~XeWriteStream()
		{
			m_Stream->Release();
		}

		void Put(Ch ch)
		{
			m_Stream->Write(&ch, 0, sizeof(ch));
		}

		void Flush()
		{
			m_Stream->Flush();
		}

		Ch Peek() const { RAPIDJSON_ASSERT(false); }
		Ch Take() { RAPIDJSON_ASSERT(false); }
		size_t Tell() const { RAPIDJSON_ASSERT(false); }
		Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
		size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }
		const Ch* Peek4() const { RAPIDJSON_ASSERT(false); }
	};
}