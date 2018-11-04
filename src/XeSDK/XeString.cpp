#include "pch.h"
#include <XeSDK/XeString.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMemory.h>
#include <string>
#include "XeStringInternal.h"

using namespace Xe;
#define CHPROP_SPACE 0x01
#define CHPROP_DIGIT 0x04
#define CHPROP_ALPHA 0x02
#define CHPROP_HEX 0x08
#define CHPROP_UPPER 0x10
#define CHPROP_LOWER 0x20
#define CHPROP_ALPHANUM 0x40

static const u8 gc_XeStringCharProp[0x100] = {
	/*0x00*/ 0,
	/*0x01*/ 0,
	/*0x02*/ 0,
	/*0x03*/ 0,
	/*0x04*/ 0,
	/*0x05*/ 0,
	/*0x06*/ 0,
	/*0x07*/ 0,
	/*0x08*/ 0,
	/*0x09*/ CHPROP_SPACE,
	/*0x0a*/ CHPROP_SPACE,
	/*0x0b*/ CHPROP_SPACE,
	/*0x0c*/ CHPROP_SPACE,
	/*0x0d*/ CHPROP_SPACE,
	/*0x0e*/ 0,
	/*0x0f*/ 0,
	/*0x10*/ 0,
	/*0x11*/ 0,
	/*0x12*/ 0,
	/*0x13*/ 0,
	/*0x14*/ 0,
	/*0x15*/ 0,
	/*0x16*/ 0,
	/*0x17*/ 0,
	/*0x18*/ 0,
	/*0x19*/ 0,
	/*0x1a*/ 0,
	/*0x1b*/ 0,
	/*0x1c*/ 0,
	/*0x1d*/ 0,
	/*0x1e*/ 0,
	/*0x1f*/ 0,
	/*0x20*/ CHPROP_SPACE,
	/*0x21*/ 0,
	/*0x22*/ 0,
	/*0x23*/ 0,
	/*0x24*/ 0,
	/*0x25*/ 0,
	/*0x26*/ 0,
	/*0x27*/ 0,
	/*0x28*/ 0,
	/*0x29*/ 0,
	/*0x2a*/ 0,
	/*0x2b*/ 0,
	/*0x2c*/ 0,
	/*0x2d*/ 0,
	/*0x2e*/ 0,
	/*0x2f*/ 0,
	/*0x30*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x31*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x32*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x33*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x34*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x35*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x36*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x37*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x38*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x39*/ CHPROP_DIGIT | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x3a*/ 0,
	/*0x3b*/ 0,
	/*0x3c*/ 0,
	/*0x3d*/ 0,
	/*0x3e*/ 0,
	/*0x3f*/ 0,
	/*0x40*/ 0,
	/*0x41*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x42*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x43*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x44*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x45*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x46*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x47*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x48*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x49*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x4a*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x4b*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x4c*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x4d*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x4e*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x4f*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x50*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x51*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x52*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x53*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x54*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x55*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x56*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x57*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x58*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x59*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x5a*/ CHPROP_ALPHA | CHPROP_UPPER | CHPROP_ALPHANUM,
	/*0x5b*/ 0,
	/*0x5c*/ 0,
	/*0x5d*/ 0,
	/*0x5e*/ 0,
	/*0x5f*/ 0,
	/*0x60*/ 0,
	/*0x61*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x62*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x63*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x64*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x65*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x66*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM | CHPROP_HEX,
	/*0x67*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x68*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x69*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x6a*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x6b*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x6c*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x6d*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x6e*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x6f*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x70*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x71*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x72*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x73*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x74*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x75*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x76*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x77*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x78*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x79*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x7a*/ CHPROP_ALPHA | CHPROP_LOWER | CHPROP_ALPHANUM,
	/*0x7b*/ 0,
	/*0x7c*/ 0,
	/*0x7d*/ 0,
	/*0x7e*/ 0,
	/*0x7f*/ 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// TODO replace this with a serious fast heap
template <typename T>
T* Xe_FastHeap_Alloc(int length)
{
	return (T*)Xe::Memory::Alloc(length);
}

// TODO replace this with a serious fast heap
template <typename T>
void Xe_FastHeap_Free(T* data)
{
	Xe::Memory::Free((void*)data);
}

String String::Empty("");

String::String(int length) :
	m_Length(length),
	m_Data(Xe_FastHeap_Alloc<char>(length + 1))
{
	ASSERT_POSITIVE(length);
	m_Data[length] = '\0';
}

String::String() :
	String(Empty)
{ }

String::String(const StringSpan& string) :
	String(string.GetLength())
{
	Xe::Memory::Copy((char*)m_Data, string.GetData(), m_Length * sizeof(char));
}

String::String(const String& string) :
	String(StringSpan(string.m_Data, string.m_Length))
{ }

String::~String()
{
	Xe_FastHeap_Free<char>(m_Data);
}

void String::CheckRangeIndex(int startIndex, int length) const
{
	ASSERT_POSITION(startIndex);
	ASSERT_POSITIVE(length);
	if (startIndex + length > m_Length)
	{
		throw std::invalid_argument(NAMEOF(length)" must be in the string range");
	}
}

String::operator const char*() const
{
	return m_Data;
}

String::operator StringSpan() const
{
	return StringSpan(m_Data, m_Length);
}

char String::operator [](int index) const
{
	ASSERT_POSITION(index);
	return m_Data[index];
}

String& String::operator =(const StringSpan& str)
{
	// TODO optimize
	return *this = String(str);
}

String& String::operator =(const String& str)
{
	// For performance reasons, m_Data will not be trimmed
	if (str.m_Length > m_Length)
	{
		Xe_FastHeap_Free<char>(m_Data);
		m_Data = Xe_FastHeap_Alloc<char>(str.m_Length + 1);
	}

	m_Length = str.m_Length;
	strncpy(m_Data, str.m_Data, str.m_Length);
	return *this;
}

String String::operator +(const StringSpan& str) const
{
	return Append(str);
}

String String::operator +(const String& str) const
{
	return Append(str);
}

String String::operator +=(const StringSpan& str) const { return *this + str; }
String String::operator +=(const String& str) const { return *this + str; }
//bool String::operator == (const StringSpan& str) const { return Compare(*this, str) == 0; }
bool String::operator == (const String& str) const { return Compare(*this, str) == 0; }
bool String::operator != (const String& str) const { return Compare(*this, str) != 0; }
bool String::operator > (const String& str) const { return Compare(*this, str) > 0; }
bool String::operator >= (const String& str) const { return Compare(*this, str) >= 0; }
bool String::operator < (const String& str) const { return Compare(*this, str) < 0; }
bool String::operator <= (const String& str) const { return Compare(*this, str) <= 0; }

int String::GetLength() const { return m_Length; }

const char* String::GetData() const { return m_Data; }

bool String::IsEmpty() const
{
	return GetLength() == 0;
}

bool String::IsEmptyOrWhitespace() const
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

bool String::StartsWith(const StringSpan& str) const
{
	return IndexOf(str) == 0;
}

bool String::StartsWith(const String& str) const
{
	return IndexOf(str) == 0;
}

bool String::EndsWith(const StringSpan& str) const
{
	return LastIndexOf(str) == GetLength() - str.GetLength();
}

bool String::EndsWith(const String& str) const
{
	return LastIndexOf(str) == GetLength() - str.GetLength();
}

int String::IndexOf(char ch) const
{
	for (int i = 0; i < m_Length; i++)
	{
		if (m_Data[i] == ch)
			return i;
	}

	return -1;
}

int String::IndexOf(const StringSpan& str) const
{
	// TODO optimize
	return IndexOf(String(str));
}

int String::IndexOf(const String& str) const
{
	char* r = strstr(m_Data, str.m_Data);
	if (r)
	{
		return (int)(r - m_Data);
	}
	return -1;
}

int String::IndexOfAny(const StringSpan& chs) const
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

int String::LastIndexOf(char ch) const
{
	for (int i = m_Length - 1; i >= 0; --i)
	{
		if (m_Data[i] == ch)
			return i;
	}

	return -1;
}

int String::LastIndexOf(const StringSpan& str) const
{
	// TODO optimize
	return LastIndexOf(String(str));
}

int String::LastIndexOf(const String& str) const
{
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

int String::LastIndexOfAny(const StringSpan& chs) const
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

String String::ToUpper() const
{
	String str(m_Length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		str.m_Data[i] = toupper(m_Data[i]);
	}

	return str;
}

String String::ToLower() const
{
	String str(m_Length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		str.m_Data[i] = tolower(m_Data[i]);
	}

	return str;
}

String String::Substring(int startIndex) const
{
	return Substring(startIndex, m_Length - startIndex);
}

String String::Substring(int startIndex, int length) const
{
	CheckRangeIndex(startIndex, length);

	String str(length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		str.m_Data[i] = m_Data[startIndex + i];
	}

	return str;
}

String String::Append(const StringSpan& str) const
{
	// TODO optimize
	return Append(String(str));
}

String String::Append(const String& str) const
{
	String strOut(m_Length + str.m_Length);
	Xe::Memory::Copy(strOut.m_Data, m_Data, m_Length);
	Xe::Memory::Copy(strOut.m_Data + m_Length, str.m_Data, str.m_Length);
	return strOut;
}

String String::Insert(int position, const StringSpan& str) const
{
	// TODO optimize
	return Insert(position, String(str));
}

String String::Insert(int position, const String& str) const
{
	ASSERT_POSITIVE(position);
	if (position > m_Length)
		ASSERT_POSITION(position);

	String strOut(m_Length + str.m_Length);
	Xe::Memory::Copy(strOut.m_Data, m_Data, position);
	Xe::Memory::Copy(strOut.m_Data + position, str.m_Data, str.m_Length);
	Xe::Memory::Copy(strOut.m_Data + position + str.m_Length, m_Data + position, m_Length - position);
	return strOut;
}

String String::Remove(int startIndex) const
{
	return Remove(startIndex, m_Length - startIndex);
}

String String::Remove(int startIndex, int length) const
{
	CheckRangeIndex(startIndex, length);
	if (length == 0)
	{
		// Do not remove anything
		return *this;
	}
	else if (startIndex == 0)
	{
		// Remove at the beginning
		return String(m_Data + length);
	}
	else if (startIndex + length == m_Length)
	{
		// Remove at the end
		return String(StringSpan(m_Data, m_Length - length));
	}
	else
	{
		// Remove between
		String strOut(m_Length - length);
		Xe::Memory::Copy(strOut.m_Data, m_Data, startIndex);
		Xe::Memory::Copy(strOut.m_Data + startIndex, m_Data + startIndex + length, m_Length - startIndex - length);
		return strOut;
	}
}

String String::Replace(char chOld, char chNew) const
{
	String str(m_Length);
	for (int i = 0; i < m_Length; ++i)
		str.m_Data[i] = m_Data[i] == chOld ? chNew : m_Data[i];
	return str;
}

String String::Replace(const StringSpan& strOld, const StringSpan& strNew) const
{
	// TODO optimize
	return Replace(Xe::String(strOld), Xe::String(strNew));
}

String String::Replace(const String& strOld, const String& strNew) const
{
	if (strOld.m_Length == 0)
	{
		throw std::invalid_argument(NAMEOF(strOld)" cannot be empty");
	}

	// Calculate the new length first
	int index = 0, count = 0;
	int newStrLength = m_Length;

	if (strNew.m_Length != strOld.m_Length)
	{
		while (index < m_Length - strOld.m_Length)
		{
			char *strFound = strstr(m_Data + index, strOld.m_Data);
			if (strFound != NULL)
			{
				newStrLength += strNew.m_Length - strOld.m_Length;
				index = (int)(strFound - m_Data) + 1;
				count++;
			}
			else
				break;
		}

		LOGFA(newStrLength >= 0);
	}

	String str(newStrLength);
	str.m_Length = 0;

	index = 0;
	for (int i = 0; i < count; i++)
	{
		int newIndex = strstr(m_Data + index, strOld.m_Data) - m_Data;
		if (newIndex > index)
		{
			memcpy(str.m_Data + str.m_Length, m_Data + index, newIndex - index);
			str.m_Length += newIndex - index;
		}

		if (strNew.m_Length > 0)
		{
			memcpy(str.m_Data + str.m_Length, strNew.m_Data, strNew.m_Length);
			str.m_Length += strNew.m_Length;
		}

		index = strOld.m_Length + newIndex;
	}

	if (index < m_Length)
	{
		memcpy(str.m_Data + str.m_Length, m_Data + m_Length - (newStrLength - str.m_Length), newStrLength - str.m_Length);
		str.m_Length += newStrLength - str.m_Length;
	}
	
	LOGFA(str.m_Length == newStrLength);

	return str;
}

String String::PadLeft(int totalWidth, char ch) const
{
	ASSERT_POSITIVE(totalWidth);
	if (totalWidth == 0)
		return *this;

	String str(m_Length + totalWidth);
	for (int i = 0; i < totalWidth; i++)
		str.m_Data[i] = ch;
	Xe::Memory::Copy(str.m_Data + totalWidth, m_Data, m_Length);

	return str;
}

String String::PadRight(int totalWidth, char ch) const
{
	ASSERT_POSITIVE(totalWidth);
	if (totalWidth == 0)
		return *this;

	String str(m_Length + totalWidth);
	Xe::Memory::Copy(str.m_Data, m_Data, m_Length);
	for (int i = 0; i < totalWidth; i++)
		str.m_Data[m_Length + i] = ch;
	return str;
}

String String::Trim() const
{
	// TODO optimize
	return TrimStart().TrimEnd();
}

String String::TrimStart() const
{
	int startIndex;
	for (startIndex = 0; startIndex < m_Length && m_Data[startIndex] == ' '; ++startIndex);
	return String(StringSpan(m_Data + startIndex, m_Length - startIndex));
}

String String::TrimEnd() const
{
	int endIndex;
	for (endIndex = m_Length - 1; endIndex >= 0 && m_Data[endIndex] == ' '; --endIndex);
	return String(StringSpan(m_Data, endIndex + 1));
	return String();
}

bool String::IsSpace(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_SPACE);
}

bool String::IsAlpha(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_ALPHA);
}

bool String::IsDecimal(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_DIGIT);
}

bool String::IsAlphaNum(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_ALPHANUM);
}

bool String::IsHexadecimal(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_HEX);
}

bool String::IsLower(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_LOWER);
}

bool String::IsUpper(int ch)
{
	return !!(gc_XeStringCharProp[(u8)ch] & CHPROP_UPPER);
}

int String::GetLength(const char* str)
{
	return strlen(str);
}

int String::Compare(const StringSpan& stra, const StringSpan& strb)
{
	return strncmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String::Compare(const String& stra, const StringSpan& strb)
{
	return strncmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String::Compare(const StringSpan& stra, const String& strb)
{
	return strncmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String::Compare(const String& stra, const String& strb)
{
	return strncmp(stra.m_Data, strb.m_Data, Xe::Math::Min(stra.m_Length, strb.m_Length));
}

int String::CompareInsensitive(const StringSpan& stra, const StringSpan& strb)
{
	return _strnicmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String::CompareInsensitive(const StringSpan& stra, const String& strb)
{
	return _strnicmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String::CompareInsensitive(const String& stra, const StringSpan& strb)
{
	return _strnicmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String::CompareInsensitive(const String& stra, const String& strb)
{
	return _strnicmp(stra.m_Data, strb.m_Data, Xe::Math::Min(stra.m_Length, strb.m_Length));
}

String String::Join(char separator, const StringSpan* strs, int count)
{
	// TODO optimize
	char s[] = { separator, '\0' };
	return Join(String(s), strs, count);
}

String String::Join(char separator, const String* strs, int count)
{
	char s[] = { separator, '\0' };
	return Join(String(s), strs, count);
}

String String::Join(const StringSpan& separator, const StringSpan* strs, int count)
{
	// TODO optimize
	String* _strs = new String[count];
	for (int i = 0; i < count; i++)
	{
		_strs[i] = String(strs[i]);
	}

	return Join(separator, _strs, count);
}

String String::Join(const String& separator, const StringSpan* strs, int count)
{
	// TODO optimize
	String* _strs = new String[count];
	for (int i = 0; i < count; i++)
	{
		_strs[i] = String(strs[i]);
	}

	return Join(separator, _strs, count);
}

String String::Join(const StringSpan& separator, const String* strs, int count)
{
	// TODO optimize
	Xe::String str;
	for (int i = 0; i < count - 1; i++)
	{
		str = str + strs[i] + separator;
	}

	return str + strs[count - 1];
}


String String::Join(const String& separator, const String* strs, int count)
{
	// TODO optimize
	Xe::String str;
	for (int i = 0; i < count - 1; i++)
	{
		str = str + strs[i] + separator;
	}

	return str + strs[count - 1];
}
