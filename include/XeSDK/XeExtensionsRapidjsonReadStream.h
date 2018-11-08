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
		void Put(Ch) { RAPIDJSON_ASSERT(false); }
		void Flush() { RAPIDJSON_ASSERT(false); }
		Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
		size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

		// For encoding detection only.
		const Ch* Peek4() const
		{
			if (m_Stream->Read(&m_LastReadChar, 0, 4) != 4)
			{
				m_LastReadChar = 0;
			}
			
			return &m_LastReadChar;
		}
	};
}