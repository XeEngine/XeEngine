#include "pch.h"
#ifdef PLATFORM_LIBC
#include <cstdio>
#endif
#include <XeSDK/XeString.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeCryptoFnv1a.h>
#include <string.h>
#include <cwchar>

#if defined(_MSC_VER)
#define SECURE_CRT
#endif

#define CAPACITY_MULTIPLIER	0x10

Xe::String::String() : String(CAPACITY_MULTIPLIER) {}
Xe::String::String(const String& string) {
	m_capacity = string.m_capacity;
	m_length = string.GetLength();
	m_str = (tchar*)Memory::Alloc(m_capacity * sizeof(tchar));
	Memory::Copy(m_str, string.m_str, m_length * sizeof(tchar));
	m_str[m_length] = '\0';
}
Xe::String::String(svar capacity)
{
	m_capacity = Math::Align(capacity, CAPACITY_MULTIPLIER);
	m_str = (tchar*)Memory::Alloc(m_capacity * sizeof(tchar));
	m_str[0] = '\0';
	m_length = 0;
}
Xe::String::String(const char* string, svar length)
{
	if (length < 0)
		length = String::GetLength(string);
	m_capacity = Math::Align(length + 1, CAPACITY_MULTIPLIER);
	m_str = (tchar*)Memory::Alloc(m_capacity * sizeof(tchar));
#ifdef PLATFORM_UNICODE
	for (svar i = 0; i < length; i++)
		m_str[i] = string[i];
#else
	Memory::Copy(m_str, string, length);
#endif
	m_str[length] = 0;
	m_length = String::GetLength(m_str);
}
Xe::String::String(const wchar_t* string, svar length)
{
	if (length < 0)
		length = GetLength(string);
	m_capacity = Math::Align(length + 1, CAPACITY_MULTIPLIER);
	m_str = (tchar*)Memory::Alloc(m_capacity * sizeof(tchar));
#ifdef PLATFORM_UNICODE
	Memory::Copy(m_str, string, length * sizeof(tchar));
#else
	// TODO: Replace with a UTF16 to UTF8 converter.
	// Note that UTF8 will need unarbitrary capacity for UTF16 convertion.
	for (svar i = 0; i < length; i++)
	{
		if (string[i] >= 0x80)
			m_str[i] = '?';
		else
			m_str[i] = (char)string[i];
	}
#endif
	m_str[length] = 0;
	m_length = GetLength(m_str);
}
Xe::String::String(const String& string, svar index, svar length)
{
	if (length <= 0 || index >= string.GetLength()) {
		m_capacity = CAPACITY_MULTIPLIER;
		m_length = 0;
		m_str = (tchar*)Memory::Alloc(m_capacity * sizeof(tchar));
		m_str[0] = '\0';
	}
	else {
		if (index + length > string.GetLength())
			m_length = string.GetLength() - index;
		else
			m_length = length;
		m_capacity = Math::Align(m_length + 1, CAPACITY_MULTIPLIER);
		m_str = (tchar*)Memory::Alloc(m_capacity * sizeof(tchar));
		Memory::Copy(m_str, string.GetData() + index, m_length);
		m_str[m_length] = '\0';
	}
}
Xe::String::~String()
{
	Memory::Free(m_str);
}

svar Xe::String::GetLength() const
{
	return m_length;
}
ctstring Xe::String::GetData() const
{
	return m_str;
}

Xe::String Xe::String::GetLower() const {
	return String(*this).ToLower();
}
Xe::String Xe::String::GetUpper() const {
	return String(*this).ToUpper();
}
const Xe::String& Xe::String::ToLower()
{
	tchar* strEnd = m_str + GetLength();
	for (tchar* str = m_str; str < strEnd; str++)
		if (*str >= 'A' && *str <= 'Z')
			*str += 0x20;
	return *this;
}
const Xe::String& Xe::String::ToUpper()
{
	tchar* strEnd = m_str + GetLength();
	for (tchar* str = m_str; str < strEnd; str++)
		if (*str >= 'a' && *str <= 'z')
			*str -= 0x20;
	return *this;
}

void Xe::String::SetCapacity(svar capacity)
{
	if (capacity > m_capacity)
	{
		m_capacity = Math::Align(capacity, CAPACITY_MULTIPLIER);
		m_str = (tchar*)Memory::Resize(m_str, m_capacity * sizeof(tchar));
	}
}
void Xe::String::SetCapacityZero(svar capacity)
{
	if (capacity > m_capacity)
	{
		Memory::Free(m_str);
		m_capacity = Math::Align(capacity, CAPACITY_MULTIPLIER);
		m_str = (tchar*)Memory::Alloc(capacity * sizeof(tchar));
	}
}

svar Xe::String::Compare(ctstring str, svar length, svar pos, CompareOptions options) const
{
	svar lenDst = GetLength();
	if (length < 0)
		length = GetLength(str);
	svar minLength = Math::Min(lenDst, length);
	svar result;

	if (options & Compare_IgnoreCase)
	{
		tchar c1;
		tchar c2;
		ctstring strDst = GetData();
		ctstring strSrc = str;
		for (svar i = 0; i < minLength; i++)
		{
			c1 = *strDst++;
			if (c1 >= 'A' && c1 <= 'Z')
				c1 += 0x20;
			c2 = *strSrc++;
			if (c2 >= 'A' && c2 <= 'Z')
				c2 += 0x20;
			result = c1 - c2;
			if (result != 0)
				return result;
		}
		result = 0;
	}
	else
	{
		result = Memory::Compare(GetData(), 0, str, 0, minLength);
	}
	if ((options & Compare_IgnoreLength) == 0)
	{
		if (result == 0)
		{
			if (lenDst > length)
				result = GetData()[length];
			else if (lenDst < length)
				result = -str[lenDst];
		}
	}
	return result;
}
svar Xe::String::Compare(const String& str, svar length, svar pos, CompareOptions options) const
{
	if (length < 0 || length > str.GetLength())
		length = str.GetLength();
	svar overflow = length - str.GetLength() - pos;
	if (overflow < 0)
		length += overflow;
	return Compare(str.GetData(), length, pos, options);
}
void Xe::String::Append(ctstring str, svar pos, svar length)
{
	svar strLen = GetLength(str);
	if (length < 0)
		length = strLen;
	svar len = Math::Min(length + pos, strLen - pos);
	ctstring p = str + pos;
	SetCapacity(m_length + len + 1);
	while (len--)
		m_str[m_length++] = *p++;
	m_str[m_length] = '\0';
}
void Xe::String::Append(const String& str, svar pos, svar length)
{
	if (length < 0)
		length = str.GetLength();
	svar len = Math::Min(length + pos, str.GetLength() - pos);
	ctstring p = str.GetData() + pos;
	SetCapacity(m_length + len + 1);
	while (len--)
		m_str[m_length++] = *p++;
	m_str[m_length] = '\0';
}
void Xe::String::Copy(ctstring string, svar length)
{
	if (length < 0)
		length = GetLength(string);

	SetCapacityZero(length + 1);
	Memory::Copy(m_str, string, (length + 1) * sizeof(tchar));
	m_length = length;
}
void Xe::String::Copy(const String& string, svar length)
{
	Copy(string.GetData(), string.GetLength());
}

Xe::String Xe::String::Substring(svar start) const
{
	if (start <= 0)
		return *this;
	else if (start >= GetLength())
		return "";
	return String(GetData() + start, GetLength() - start);
}
Xe::String Xe::String::Substring(svar start, svar length) const
{
	if (start <= 0)
	{
		start = 0;
		if (length >= GetLength())
			return *this;
	}
	else if (start + length > GetLength())
		length += GetLength() - (start + length);
	if (start >= GetLength() || length <= 0)
		return "";
	return String(GetData() + start, length);
}

Xe::String::operator ctstring() const
{
	return GetData();
}
tchar& Xe::String::operator [](svar index)
{
	return m_str[index];
}
const Xe::String& Xe::String::operator = (const char* str)
{
	Copy(str);
	return *this;
}
const Xe::String& Xe::String::operator = (const wchar_t* str)
{
	Copy(str);
	return *this;
}
const Xe::String& Xe::String::operator = (const String& str)
{
	Copy(str);
	return *this;
}
bool Xe::String::operator == (ctstring str) const
{
	return Compare(str) == 0;
}
bool Xe::String::operator == (const String& str) const
{
	return Compare(str) == 0;
}
bool Xe::String::operator != (ctstring str) const
{
	return Compare(str) != 0;
}
bool Xe::String::operator != (const String& str) const
{
	return Compare(str) != 0;
}
bool Xe::String::operator > (ctstring str) const
{
	return Compare(str) > 0;
}
bool Xe::String::operator > (const String& str) const
{
	return Compare(str) > 0;
}
bool Xe::String::operator < (ctstring str) const
{
	return Compare(str) < 0;
}
bool Xe::String::operator < (const String& str) const
{
	return Compare(str) < 0;
}
bool Xe::String::operator >= (ctstring str) const
{
	return Compare(str) >= 0;
}
bool Xe::String::operator >= (const String& str) const
{
	return Compare(str) >= 0;
}
bool Xe::String::operator <= (ctstring str) const
{
	return Compare(str) <= 0;
}
bool Xe::String::operator <= (const String& str) const
{
	return Compare(str) <= 0;
}
Xe::String Xe::String::operator + (const char* str) const
{
	svar strLen = String::GetLength(str);
	String s(this->GetLength() + strLen);
	s.Append(GetData(), 0, GetLength());
	s.Append(str, 0, strLen);
	return s;
}
Xe::String Xe::String::operator + (const wchar_t* str) const
{
	svar strLen = String::GetLength(str);
	String s(this->GetLength() + strLen);
	s.Append(GetData(), 0, GetLength());
	s.Append(str, 0, strLen);
	return s;
}
Xe::String Xe::String::operator + (const String& str) const
{
	String s(this->GetLength() + str.GetLength());
	s.Append(GetData(), 0, GetLength());
	s.Append(str.GetData(), 0, str.GetLength());
	return s;
}
const Xe::String& Xe::String::operator += (const char* src)
{
	Append(src, 0);
	return *this;
}
const Xe::String& Xe::String::operator += (const wchar_t* src)
{
	Append(src, 0);
	return *this;
}
const Xe::String& Xe::String::operator += (const String& src)
{
	Append(src, 0);
	return *this;
}

template <typename T> T *_ToLower(T *str, svar length)
{
	while (length--)
		str[length] = Xe::String::ToLower(str[length]);
	return str;
}
template <typename T> T *_ToUpper(T *str, svar length)
{
	while (length--)
		str[length] = Xe::String::ToUpper(str[length]);
	return str;
}
template <typename T> svar Atoi(const T *str, svar length, svar base)
{
	svar sign = +1;

	// Skip whitespaces
	while (Xe::String::IsSpace(*str))
		str++;

	// Check sign
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
	{
		sign = +1;
		str++;
	}

	if (base == 0)
	{
		if (*str == '0')
		{
			str++;
			switch (*str)
			{
			case 'b':
				// Binary
				base = 2;
				break;
			case 'x':
				// Hexadecimal
				base = 16;
				break;
			default:
				// Octal
				base = 8;
				while (*str == '0') str++;
			}
		}
		else
			// Decimal
			base = 10;
	}
	if (length == 0)
	{
		const T *tmp = str;
		while (*tmp != '\0')
		{
			tmp++;
			length++;
		}
	}

	svar value = 0;
	switch (base)
	{
	case 2:
		while (length--)
		{
			if (*str == '0' || *str == '1')
			{
				value <<= 1;
				value |= *str - '0';
			}
			str++;
		}
		break;
	case 8:
		while (length--)
		{
			if (*str >= '0' && *str <= '7')
			{
				value <<= 3;
				value |= *str - '0';
			}
			str++;
		}
		break;
	case 10:
		while (length--)
		{
			if (*str >= '0' && *str <= '9')
			{
				value *= 10;
				value += *str - '0';
			}
			str++;
		}
		break;
	case 16:
		while (length--)
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
			str++;
		}
		break;
	}
	return value * sign;
}
template <typename T> svar Itoa(svar value, T *str, svar base)
{
	static const T digits[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	};

	svar strLength = 0;
	if (base == 0)
		base = 10;
	if (base == 10)
	{
		if (value < 0)
		{
			if (str != nullptr)
				*str++ = '-';
			strLength++;
			value = -value;
		}
	}
	if (str != 0)
	{
		T *tmp = str;
		do
		{
			*tmp++ = digits[value % base];
			value /= base;
			strLength++;
		} while (value != 0);
		for (svar i = strLength / 2 - 1; i >= 0; i--)
		{
			T c = str[i];
			str[i] = str[strLength - i - 1];
			str[strLength - i - 1] = c;
		}
	}
	else
	{
		do
		{
			value /= base;
			strLength++;
		} while (value != 0);
	}
	return strLength;
}

#define CHPROP_ISSPACE 0x01
#define CHPROP_ISDIGIT 0x04
#define CHPROP_ISALPHA 0x02
#define CHPROP_ISXDIGIT 0x08
#define CHPROP_ISUPPER 0x10
#define CHPROP_ISLOWER 0x20
#define CHPROP_ISALNUM 0x40

static const u8 _xe_string_chprop[0x100] = {
	/*0x00  */ 0,
	/*0x01  */ 0,
	/*0x02  */ 0,
	/*0x03  */ 0,
	/*0x04  */ 0,
	/*0x05  */ 0,
	/*0x06  */ 0,
	/*0x07  */ 0,
	/*0x08  */ 0,
	/*0x09  */ CHPROP_ISSPACE,
	/*0x0a  */ CHPROP_ISSPACE,
	/*0x0b  */ CHPROP_ISSPACE,
	/*0x0c  */ CHPROP_ISSPACE,
	/*0x0d  */ CHPROP_ISSPACE,
	/*0x0e  */ 0,
	/*0x0f  */ 0,
	/*0x10  */ 0,
	/*0x11  */ 0,
	/*0x12  */ 0,
	/*0x13  */ 0,
	/*0x14  */ 0,
	/*0x15  */ 0,
	/*0x16  */ 0,
	/*0x17  */ 0,
	/*0x18  */ 0,
	/*0x19  */ 0,
	/*0x1a  */ 0,
	/*0x1b  */ 0,
	/*0x1c  */ 0,
	/*0x1d  */ 0,
	/*0x1e  */ 0,
	/*0x1f  */ 0,
	/*0x20  */ CHPROP_ISSPACE,
	/*0x21  */ 0,
	/*0x22  */ 0,
	/*0x23  */ 0,
	/*0x24  */ 0,
	/*0x25  */ 0,
	/*0x26  */ 0,
	/*0x27  */ 0,
	/*0x28  */ 0,
	/*0x29  */ 0,
	/*0x2a  */ 0,
	/*0x2b  */ 0,
	/*0x2c  */ 0,
	/*0x2d  */ 0,
	/*0x2e  */ 0,
	/*0x2f  */ 0,
	/*0x30  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x31  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x32  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x33  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x34  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x35  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x36  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x37  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x38  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x39  */ CHPROP_ISDIGIT | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x3a  */ 0,
	/*0x3b  */ 0,
	/*0x3c  */ 0,
	/*0x3d  */ 0,
	/*0x3e  */ 0,
	/*0x3f  */ 0,
	/*0x40  */ 0,
	/*0x41  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x42  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x43  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x44  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x45  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x46  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x47  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x48  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x49  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x4a  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x4b  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x4c  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x4d  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x4e  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x4f  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x50  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x51  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x52  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x53  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x54  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x55  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x56  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x57  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x58  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x59  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x5a  */ CHPROP_ISALPHA | CHPROP_ISUPPER | CHPROP_ISALNUM,
	/*0x5b  */ 0,
	/*0x5c  */ 0,
	/*0x5d  */ 0,
	/*0x5e  */ 0,
	/*0x5f  */ 0,
	/*0x60  */ 0,
	/*0x61  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x62  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x63  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x64  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x65  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x66  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM | CHPROP_ISXDIGIT,
	/*0x67  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x68  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x69  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x6a  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x6b  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x6c  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x6d  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x6e  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x6f  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x70  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x71  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x72  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x73  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x74  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x75  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x76  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x77  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x78  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x79  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x7a  */ CHPROP_ISALPHA | CHPROP_ISLOWER | CHPROP_ISALNUM,
	/*0x7b  */ 0,
	/*0x7c  */ 0,
	/*0x7d  */ 0,
	/*0x7e  */ 0,
	/*0x7f  */ 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};


bool Xe::String::IsSpace(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISSPACE);
}
bool Xe::String::IsSpace(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISSPACE);
}
bool Xe::String::IsAlpha(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISALPHA);
}
bool Xe::String::IsAlpha(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISALPHA);
}
bool Xe::String::IsDigit(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISDIGIT);
}
bool Xe::String::IsDigit(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISDIGIT);
}
bool Xe::String::IsAlnum(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISALNUM);
}
bool Xe::String::IsAlnum(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISALNUM);
}
bool Xe::String::IsXDigit(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISXDIGIT);
}
bool Xe::String::IsXDigit(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISXDIGIT);
}
bool Xe::String::IsLower(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISLOWER);
}
bool Xe::String::IsLower(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISLOWER);
}
bool Xe::String::IsUpper(char c)
{
	return !!(_xe_string_chprop[(u8)c] & CHPROP_ISUPPER);
}
bool Xe::String::IsUpper(wchar_t c)
{
	return !!(_xe_string_chprop[c & 0xFF] & CHPROP_ISUPPER);
}
char Xe::String::ToLower(char c)
{
	return IsUpper(c) ? c + ('a' - 'A') : c;
}
wchar_t Xe::String::ToLower(wchar_t c)
{
	return IsUpper(c) ? c + (L'a' - L'A') : c;
}
char *Xe::String::ToLower(char *str, svar length)
{
	return _ToLower<char>(str, length);
}
wchar_t *Xe::String::ToLower(wchar_t *str, svar length)
{
	return _ToLower<wchar_t>(str, length);
}
char Xe::String::ToUpper(char c)
{
	return IsLower(c) ? c - (L'a' - L'A') : c;
}
wchar_t Xe::String::ToUpper(wchar_t c)
{
	return IsLower(c) ? c - (L'a' - L'A') : c;
}
char *Xe::String::ToUpper(char *str, svar length)
{
	return _ToUpper<char>(str, length);
}
wchar_t *Xe::String::ToUpper(wchar_t *str, svar length)
{
	return _ToUpper<wchar_t>(str, length);
}

int Xe::String::WideToChar(wchar_t ch)
{
#ifdef PLATFORM_LIBC
	return wctob(ch);
#else
	if (ch < 128U)
		return ch;
	return 255;
#endif
}

svar Xe::String::Atoi(const char *str, svar length, svar base)
{
	return ::Atoi<char>(str, length, base);
}
svar Xe::String::Atoi(const wchar_t *str, svar length, svar base)
{
	return ::Atoi<wchar_t>(str, length, base);
}
svar Xe::String::Itoa(svar value, char *str, svar base)
{
	return ::Itoa<char>(value, str, base);
}
svar Xe::String::Itoa(svar value, wchar_t *str, svar base)
{
	return ::Itoa<wchar_t>(value, str, base);
}

template <typename T> const T *Xe_FindCharacter(const T *string, T ch)
{
	if (string == nullptr)
		return (const T*)nullptr;
	while (*string && *string != ch)
		string++;
	return *string == ch ? string : (const T*)nullptr;
}
const char *Xe::String::FindCharacter(const char *string, char ch)
{
#ifdef PLATFORM_LIBC
	return strchr(string, ch);
#else
	return ::Xe_FindCharacter<char>(string, ch);
#endif
}
const wchar_t *Xe::String::FindCharacter(const wchar_t *string, wchar_t ch)
{
#ifdef PLATFORM_LIBCW
	return wcschr(string, ch);
#else
	return ::Xe_FindCharacter<wchar_t>(string, ch);
#endif
}
template <typename T> const T *Xe_FindLastCharacter(const T *string, T ch)
{
	if (string == nullptr)
		return (const T*)nullptr;
	const T *start = string;
	while (*string++);
	while (--string != start && *string != ch);
	return *string == ch ? string : (const T*)nullptr;
}
const char *Xe::String::FindLastCharacter(const char *string, char ch)
{
#ifdef PLATFORM_LIBC
	return strrchr(string, ch);
#else
	return ::Xe_FindLastCharacter<char>(string, ch);
#endif
}
const wchar_t *Xe::String::FindLastCharacter(const wchar_t *string, wchar_t ch)
{
#ifdef PLATFORM_LIBC
	return wcsrchr(string, ch);
#else
	return ::Xe_FindLastCharacter<wchar_t>(string, ch);
#endif
}

template <typename T> svar Xe_GetLength(const T *string)
{
	const T *pStr;
	for (pStr = string; *pStr; ++pStr);
	return (svar)(pStr - string);
}
svar Xe::String::GetLength(const char* str)
{
#ifdef PLATFORM_LIBC
	return (svar)strlen(str);
#else
	return Xe_GetLength(str);
#endif
}
svar Xe::String::GetLength(const wchar_t* str)
{
#ifdef PLATFORM_LIBC
	return (svar)wcslen(str);
#else
	return Xe_GetLength(str);
#endif
}
svar Xe::String::GetNLength(const char* str, svar length)
{
	if (str == nullptr)
		return -1;
	return strnlen(str, length);
}
svar Xe::String::GetNLength(const wchar_t* str, svar length)
{
	if (str == nullptr)
		return -1;
	svar len;
	for (len = 0; len < length; len++, str++) {
		if (!*str)
			break;
	}
	return len;
}

svar Xe::String::Copy(char* dst, const char* src, svar length)
{
#if defined(SECURE_CRT)
	if (strcpy_s(dst, length, src) != 0)
		return 0;
	return GetLength(src);
#else
	if (dst == nullptr || length <= 0)
		return 0;

	svar strSrcLength = GetLength(src);
	if (strSrcLength < 0 || length <= strSrcLength)
	{
		dst[0] = '\0';
		return 0;
	}
	strcpy(dst, src);
	return strSrcLength;
#endif
}
svar Xe::String::Copy(wchar_t* dst, const wchar_t* src, svar length)
{
#if defined(SECURE_CRT)
	if (wcscpy_s(dst, length, src) != 0)
		return 0;
	return GetLength(src);
#else
	if (dst == nullptr || length <= 0)
		return 0;

	svar strSrcLength = GetLength(src);
	if (strSrcLength < 0 || length <= strSrcLength)
	{
		dst[0] = L'\0';
		return 0;
	}
	wcscpy(dst, src);
	return strSrcLength;
#endif
}
svar Xe::String::Copy(char* dst, const wchar_t* src, svar length)
{
	if (dst == nullptr || length <= 0)
		return 0;

	svar strSrcLength = GetLength(src);
	if (strSrcLength < 0 || length <= strSrcLength)
	{
		dst[0] = '\0';
		return 0;
	}
	for (svar i = 0; i < length; i++)
		*dst++ = WideToChar(*src++);
	return strSrcLength;
}
svar Xe::String::Copy(wchar_t* dst, const char* src, svar length)
{
	if (dst == nullptr || length <= 0)
		return 0;

	svar strSrcLength = GetLength(src);
	if (strSrcLength < 0 || length <= strSrcLength)
	{
		dst[0] = L'\0';
		return 0;
	}
	for (svar i = 0; i < length; i++, src++)
		*dst++ = (wchar_t)*src;
	return strSrcLength;
}

svar Xe::String::NCopy(tchar* dst, ctstring src, svar length)
{
	svar l = GetLength(src);
	if (l < 0 || dst == nullptr)
		return 0;
#if defined(SECURE_CRT)
#ifdef PLATFORM_UNICODE
	wcsncpy_s(dst, length, src, length);
#else
	strncpy_s(dst, length, src, length);
#endif
#else
#ifdef PLATFORM_UNICODE
	wcsncpy(dst, src, length);
#else
	strncpy(dst, src, length);
#endif
#endif
	return (length < l ? length : l);
}
u64 Xe::String::GetHashFNV1a(const char *str, svar length)
{
	return Xe::Crypto::Fnv1a64(str, length);
}
u64 Xe::String::GetHashFNV1a(const wchar_t *str, svar length)
{
	return Xe::Crypto::Fnv1a64((const char*)str, length * sizeof(wchar_t));
}