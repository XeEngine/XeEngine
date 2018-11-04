#include "pch.h"
#include <XeSDK/XeStringSpan.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeString.h>
#include <string>
#include "XeStringInternal.h"

using namespace Xe;

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

char StringSpan::operator [](int index) const { return m_Data[index]; }

StringSpan StringSpan::operator + (int index) const
{
	return StringSpan(m_Data + index, m_Length - index);
}

StringSpan StringSpan::operator += (int index) const
{
	return *this + index;
}

StringSpan& StringSpan::operator =(const StringSpan& str)
{
	m_Length = str.m_Length;
	m_Data = str.m_Data;
	return *this;
}

bool StringSpan::operator == (const StringSpan& str) const
{
	return strncmp(m_Data, str.m_Data, Xe::Math::Min(m_Length, str.m_Length)) == 0;
}

bool StringSpan::operator != (const StringSpan& str) const
{
	return strncmp(m_Data, str.m_Data, Xe::Math::Min(m_Length, str.m_Length)) != 0;
}

bool StringSpan::operator > (const StringSpan& str) const
{
	return strncmp(m_Data, str.m_Data, Xe::Math::Min(m_Length, str.m_Length)) > 0;
}

bool StringSpan::operator >= (const StringSpan& str) const
{
	return strncmp(m_Data, str.m_Data, Xe::Math::Min(m_Length, str.m_Length)) >= 0;
}

bool StringSpan::operator < (const StringSpan& str) const
{
	return strncmp(m_Data, str.m_Data, Xe::Math::Min(m_Length, str.m_Length)) < 0;
}

bool StringSpan::operator <= (const StringSpan& str) const
{
	return strncmp(m_Data, str.m_Data, Xe::Math::Min(m_Length, str.m_Length)) <= 0;
}

int StringSpan::GetLength() const { return m_Length; }

const char* StringSpan::GetData() const { return m_Data; }

bool StringSpan::IsEmpty() const
{
	return m_Length == 0;
}

bool StringSpan::TryParse(int& value, int base)
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

int StringSpan::Parse(int defaultValue, int base)
{
	int value;
	return TryParse(value, base) ? value : defaultValue;
}