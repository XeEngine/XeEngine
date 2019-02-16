#include "pch.h"
#include <XeSDK/XeStringSpan.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeString.h>
#include <string.h>
#include "XeStringInternal.h"

using namespace Xe;

StringSpan StringSpan::Empty("");

StringSpan::StringSpan() :
	StringSpan(Empty)
{}

StringSpan::StringSpan(const char* str) :
	m_Length(str ? strlen(str) : 0),
	m_Data(str ? str : "")
{ }

StringSpan::StringSpan(const char* str, int length) :
	m_Length(length),
	m_Data(str)
{
	ASSERT_POSITIVE(length);
}

StringSpan::StringSpan(const char* str, int maxLength, int length) :
	m_Length(length),
	m_Data(str)
{
	ASSERT_POSITIVE(length);
	if (maxLength < length)
	{
		throw std::invalid_argument(NAMEOF(length)" must be in the string range");
	}
}

void StringSpan::CheckRangeIndex(int startIndex, int length) const
{
	ASSERT_POSITION(startIndex);
	ASSERT_POSITIVE(length);
	if (startIndex + length > m_Length)
	{
		throw std::invalid_argument(NAMEOF(length)" must be in the string range");
	}
}

char StringSpan::operator [](int index) const
{
	ASSERT_CPOSITION(index, m_Length);
	return m_Data[index];
}

StringSpan StringSpan::operator + (int index) const
{
	return StringSpan(m_Data + index, m_Length - index);
}

StringSpan StringSpan::operator += (int index) const
{
	return *this + index;
}

bool StringSpan::operator == (const StringSpan& str) const
{
	return Compare(*this, str) == 0;
}

bool StringSpan::operator != (const StringSpan& str) const
{
	return Compare(*this, str) != 0;
}

bool StringSpan::operator > (const StringSpan& str) const
{
	return Compare(*this, str) > 0;
}

bool StringSpan::operator >= (const StringSpan& str) const
{
	return Compare(*this, str) >= 0;
}

bool StringSpan::operator < (const StringSpan& str) const
{
	return Compare(*this, str) < 0;
}

bool StringSpan::operator <= (const StringSpan& str) const
{
	return Compare(*this, str) <= 0;
}

int StringSpan::GetLength() const { return m_Length; }

const char* StringSpan::GetData() const { return m_Data; }

bool StringSpan::IsEmpty() const
{
	return m_Length == 0;
}

bool StringSpan::IsEmptyOrWhitespace() const
{
	for (int i = 0; i < m_Length; ++i)
	{
		switch (m_Data[i])
		{
		case '\t':
		case '\n':
		case '\r':
		case ' ':
			break;
		default:
			return false;
		}
	}

	return true;
}

bool StringSpan::StartsWith(const StringSpan& str) const
{
	return IndexOf(str) == 0;
}

bool StringSpan::EndsWith(const StringSpan& str) const
{
	return LastIndexOf(str) == m_Length - str.m_Length;
}

int StringSpan::IndexOf(char ch) const
{
	for (int i = 0; i < m_Length; i++)
	{
		if (m_Data[i] == ch)
			return i;
	}

	return -1;
}

int StringSpan::IndexOf(const StringSpan& str) const
{
	if (str.m_Length == 0)
		return 0;
	else if (str.m_Length == 1)
		return IndexOf(str[0]);

	for (int i = 0; i < m_Length - str.m_Length + 1; i++)
	{
		if (m_Data[i] == str.m_Data[0])
		{
			int j;
			for (j = 1; j < str.m_Length; j++)
			{
				if (m_Data[i + j] != str.m_Data[j])
				{
					j = -1;
					break;
				}
			}

			if (j >= 0)
				return i;
		}
	}

	return -1;
}


int StringSpan::IndexOfAny(const StringSpan& chs) const
{
	int count = chs.GetLength();
	if (count == 1)
	{
		for (int i = 0; i < m_Length; i++)
		{
			if (m_Data[i] == chs[0])
				return i;
		}
	}
	else if (count > 1)
	{
		for (int i = 0; i < m_Length; i++)
		{
			for (int j = 0; j < count; j++)
			{
				if (m_Data[i] == chs[j])
					return i;
			}
		}
	}

	return -1;
}

int StringSpan::LastIndexOf(char ch) const
{
	for (int i = m_Length - 1; i >= 0; --i)
	{
		if (m_Data[i] == ch)
			return i;
	}

	return -1;
}

int StringSpan::LastIndexOf(const StringSpan& str) const
{
	if (str.m_Length == 0)
		return m_Length;
	else if (str.m_Length == 1)
		return LastIndexOf(str[0]);

	for (int i = m_Length - str.m_Length; i >= 0; --i)
	{
		if (m_Data[i] == str.m_Data[0])
		{
			int j;
			for (j = 1; j < str.m_Length; j++)
			{
				if (m_Data[i + j] != str.m_Data[j])
				{
					j = -1;
					break;
				}
			}

			if (j >= 0)
				return i;
		}
	}

	return -1;
}


int StringSpan::LastIndexOfAny(const StringSpan& chs) const
{
	int count = chs.GetLength();
	if (count == 1)
	{
		for (int i = m_Length - 1; i >= 0; i--)
		{
			if (m_Data[i] == chs[0])
				return i;
		}
	}
	else if (count > 1)
	{
		for (int i = m_Length - 1; i >= 0; i--)
		{
			for (int j = 0; j < count; j++)
			{
				if (m_Data[i] == chs[j])
					return i;
			}
		}
	}

	return -1;
}

StringSpan StringSpan::Substring(int startIndex) const
{
	return Substring(startIndex, m_Length - startIndex);
}

StringSpan StringSpan::Substring(int startIndex, int length) const
{
	if (length == 0)
		return Empty;

	CheckRangeIndex(startIndex, length);

	return StringSpan(m_Data + startIndex, length);
}

bool StringSpan::TryParse(bool& value) const
{
	auto trimmed = String(*this).Trim();
	if (StringSpan::CompareInsensitive(trimmed, "true") == 0)
		value = true;
	else if (StringSpan::CompareInsensitive(trimmed, "false") == 0)
		value = false;
	else
		return false;
	return true;
}

bool StringSpan::TryParse(int& value, int base) const
{
	const char* str = m_Data;
	int length = m_Length;
	int sign = +1;

	// Skip whitespaces
	while (Xe::String::IsSpace(*str))
	{
		str++;
		length--;
	}

	// Check sign
	if (*str == '-')
	{
		sign = -1;
		str++;
		length--;
	}
	else if (*str == '+')
	{
		sign = +1;
		str++;
		length--;
	}

	if (length == 0)
	{
		const char *tmp = str;
		while (*tmp != '\0')
		{
			tmp++;
			length++;
		}
	}

	value = 0;
	if (length > 0)
	{
		switch (base)
		{
		case 2:
			do
			{
				if (*str == '0' || *str == '1')
				{
					value <<= 1;
					value |= *str - '0';
				}
				else break;
				str++;
			} while (length--);
			break;
		case 8:
			do
			{
				if (*str >= '0' && *str <= '7')
				{
					value <<= 3;
					value |= *str - '0';
				}
				else break;
				str++;
			} while (length--);
			break;
		case 10:
			do
			{
				if (*str >= '0' && *str <= '9')
				{
					value *= 10;
					value += *str - '0';
				}
				else break;
				str++;
			} while (length--);
			break;
		case 16:
			do
			{
				if (*str >= '0' && *str <= '9')
				{
					value <<= 4;
					value |= *str - '0';
				}
				else if (*str >= 'A' && *str <= 'F')
				{
					value <<= 4;
					value |= (*str - 'A') + 10;
				}
				else if (*str >= 'a' && *str <= 'f')
				{
					value <<= 4;
					value |= (*str - 'a') + 10;
				}
				else break;
				str++;
			} while (length--);
			break;
		default:
			throw std::invalid_argument(NAMEOF(base)" must be 2, 8, 10 or 16");
		}
	}

	while (length--)
	{
		// Again, skip extra spaces
		if (!Xe::String::IsSpace(*str++))
		{
			return false;
		}
	}

	value *= sign;
	return true;
}

bool StringSpan::ParseBool(bool defaultValue) const
{
	bool value;
	return TryParse(value) ? value : defaultValue;
}

int StringSpan::ParseInt(int defaultValue, int base) const
{
	int value;
	return TryParse(value, base) ? value : defaultValue;
}

int StringSpan::Compare(const StringSpan& stra, const StringSpan& strb)
{
	return strncmp(stra.m_Data, strb.m_Data, Xe::Math::Max(stra.m_Length, strb.m_Length));
}

int StringSpan::CompareInsensitive(const StringSpan& stra, const StringSpan& strb)
{
#ifdef _WIN32
	return _strnicmp(stra.m_Data, strb.m_Data, Xe::Math::Max(stra.m_Length, strb.m_Length));
#else
	return strncasecmp(stra.m_Data, strb.m_Data, Xe::Math::Max(stra.m_Length, strb.m_Length));
#endif
}
