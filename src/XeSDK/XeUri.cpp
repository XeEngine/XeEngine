#include "pch.h"
#include <XeSDK/XeUri.h>
#include <XeSDK/XeMath.h>
#include <stdexcept>

using namespace Xe;

const char c_SchemaSeparator = ':';
const char c_UserInfoTerminator = '@';
const char c_UserPasswordTerminator = ':';
const char c_PortSeparator = ':';
const char c_PathSeparator = '/';
const char c_QuerySeparator = '?';
const char c_FragmentSeparator = '#';
const StringSpan c_AllowedSchemeChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-.";
const char c_AuthoritySeparator[] = { c_PathSeparator, c_QuerySeparator, c_FragmentSeparator };

bool Uri::IsSchemaValid(const StringSpan& schema)
{
	for (int i = 0; i < schema.GetLength(); ++i)
	{
		if (schema.Substring(i, 1).IndexOfAny(c_AllowedSchemeChars) < 0)
			return false;
	}

	return true;
}

Uri::Uri(const StringSpan& uri) :
	OriginalString(uri)
{
	auto schemaSeparatorIndex = OriginalString.IndexOf(c_SchemaSeparator);
	if (schemaSeparatorIndex < 0)
	{
		throw std::invalid_argument("Invalid URI: The format of the URI could not be determined.");
	}

	m_Scheme = OriginalString.Substring(0, schemaSeparatorIndex);
	if (m_Scheme.GetLength() == 0 || String::IsDecimal(m_Scheme[0]) || !IsSchemaValid(m_Scheme))
	{
		throw std::invalid_argument("Invalid URI: The URI scheme is not valid.");
	}

	auto authoritySeparatorIndex = schemaSeparatorIndex + 1;
	if (OriginalString.GetLength() >= authoritySeparatorIndex + 2 &&
		OriginalString[authoritySeparatorIndex + 0] == '/' &&
		OriginalString[authoritySeparatorIndex + 1] == '/')
	{
		// Path does contain Authority
		authoritySeparatorIndex += 2;
		auto subOriginalString = OriginalString.Substring(authoritySeparatorIndex);

		int authorityEndIndex = OriginalString.GetLength();
		for (int i = 0; i < lengthof(c_AuthoritySeparator); ++i)
		{
			auto foundIndex = subOriginalString.IndexOf(c_AuthoritySeparator[i]);
			if (foundIndex >= 0)
				authorityEndIndex = Math::Min(authorityEndIndex, foundIndex + authoritySeparatorIndex);
		}

		m_Authority = OriginalString.Substring(authoritySeparatorIndex, authorityEndIndex - authoritySeparatorIndex);
	}
	else
	{
		m_Authority = String::Empty;
	}

	if (m_Authority.GetLength() > 0)
	{
		auto userInfoTerminatorIndex = m_Authority.IndexOf(c_UserInfoTerminator);
		if (userInfoTerminatorIndex > 0)
		{
			auto userNameTerminatorIndex = m_Authority.IndexOf(c_UserPasswordTerminator);
			userNameTerminatorIndex = userNameTerminatorIndex >= 0 ? Math::Min(userNameTerminatorIndex, authoritySeparatorIndex - 1) : authoritySeparatorIndex - 1;
			m_UserInfo = m_Authority.Substring(0, userNameTerminatorIndex);
		}
		else
		{
			m_UserInfo = String::Empty;
		}

		m_HostAndPort = m_Authority.Substring(userInfoTerminatorIndex + 1);

		auto portSeparatorIndex = m_HostAndPort.LastIndexOf(c_PortSeparator) + 1;
		if (portSeparatorIndex > 0 && portSeparatorIndex < m_HostAndPort.GetLength())
		{
			int port;
			if (m_HostAndPort.Substring(portSeparatorIndex).TryParse(port) == false ||
				port < 0 || port >= 65536)
				throw std::invalid_argument("Invalid URI: Invalid port specified.");

			m_Port = port;
		}
		else
		{
			m_Port = -1;
		}

		m_Host = portSeparatorIndex > 0 ? m_HostAndPort.Substring(0, portSeparatorIndex - 1) : m_HostAndPort;
	}
	else
	{
		m_UserInfo = String::Empty;
		m_Port = -1;
		m_Host = String::Empty;
	}

	auto pathStartIndex = authoritySeparatorIndex;
	auto fullPath = OriginalString.Substring(pathStartIndex + m_Authority.GetLength());
	if (fullPath.GetLength() > 0)
	{
		auto fragmentIndex = fullPath.IndexOf(c_FragmentSeparator);
		auto queryIndex = fullPath.IndexOf(c_QuerySeparator);
		auto pathTerminator = Math::Min(fragmentIndex >= 0 ? fragmentIndex : fullPath.GetLength(), queryIndex >= 0 ? queryIndex : fullPath.GetLength());
		m_Path = fullPath.Substring(0, pathTerminator);

		if (queryIndex >= 0 && (fragmentIndex >= 0 ? queryIndex < fragmentIndex : true))
		{
			if (fragmentIndex >= 0)
			{
				m_Query = fullPath.Substring(queryIndex, fragmentIndex - queryIndex);
			}
			else
			{
				m_Query = fullPath.Substring(queryIndex);
			}
		}
		else
		{
			m_Query = String::Empty;
		}

		if (fragmentIndex >= 0)
		{
			m_Fragment = fullPath.Substring(fragmentIndex);
		}
		else
		{
			m_Fragment = String::Empty;
		}
	}
	else if (m_Authority.GetLength() > 0)
	{
		m_Path = "/";
	}
	else
	{
		m_Path = String::Empty;
	}
}

const StringSpan& Uri::GetScheme() const { return m_Scheme; }

const StringSpan& Uri::GetAuthority() const { return m_Authority; }

const StringSpan& Uri::GetUserInfo() const { return m_UserInfo; }

const StringSpan& Uri::GetHost() const { return m_Host; }

int Uri::GetPort() const { return m_Port; }

const StringSpan& Uri::GetPath() const { return m_Path; }

const StringSpan& Uri::GetQuery() const { return m_Query; }

const StringSpan& Uri::GetFragment() const { return m_Fragment; }

const StringSpan& Uri::GetHostAndPort() const { return m_HostAndPort; }