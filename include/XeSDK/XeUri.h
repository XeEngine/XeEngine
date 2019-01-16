#include <XeSDK/XeString.h>

namespace Xe
{
	//! Uniform Resource Identifier
	//! \details https://tools.ietf.org/html/rfc3986
	class Uri
	{
		StringSpan m_Scheme;
		StringSpan m_Authority;
		StringSpan m_UserInfo;
		StringSpan m_Host;
		StringSpan m_Path;
		StringSpan m_Query;
		StringSpan m_Fragment;
		StringSpan m_HostAndPort;
		int m_Port;

	public:
		static bool IsSchemaValid(const StringSpan& schema);

		const String OriginalString;

		Uri(const StringSpan& uri);
		
		//! RFC 3986, 3.1
		const StringSpan& GetScheme() const;

		//! RFC 3986, 3.2, different from Microsoft implementation
		const StringSpan& GetAuthority() const;

		//! RFC 3986, 3.2.1
		const StringSpan& GetUserInfo() const;

		//! RFC 3986, 3.2.2
		const StringSpan& GetHost() const;

		//! RFC 3986, 3.2.3
		int GetPort() const;

		//! RFC 3986, 3.3
		const StringSpan& GetPath() const;

		//! RFC 3986, 3.4
		const StringSpan& GetQuery() const;

		//! RFC 3986, 3.5
		const StringSpan& GetFragment() const;

		const StringSpan& GetHostAndPort() const;
	};
}