#include "pch.h"
#include <XeSDK/XeString.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMemory.h>
#include <string.h>
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

namespace Xe
{
	const char String_Digits_Lowercase[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
	};

	const char String_Digits_Uppercase[]
	{
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	};

	const char String_Digits_Uppercase_Negative[]
	{
		'F', 'E', 'D', 'C', 'B', 'A', '9', '8',
		'7', '6', '5', '4', '3', '2', '1',
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	};
}

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
	Base(Xe_FastHeap_Alloc<char>(length + 1), length),
	m_String(*this)
{
	ASSERT_POSITIVE(length);
	((char*)m_Data)[length] = '\0';
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
	String((StringSpan)string)
{ }

String::~String()
{
	Xe_FastHeap_Free<char>((char*)m_Data);
}

String::operator const char*() const
{
	return m_Data;
}

String& String::operator =(const StringSpan& str)
{
	// For performance reasons, m_Data will not be trimmed
	if (str.m_Length > m_Length)
	{
		Xe_FastHeap_Free<char>((char*)m_Data);
		m_Data = Xe_FastHeap_Alloc<char>(str.m_Length + 1);
	}

	m_Length = str.m_Length;
	strncpy((char*)m_Data, str.m_Data, str.m_Length);
	return *this;
}

String String::operator +(const StringSpan& str) const
{
	return Append(str);
}

String String::operator +=(const StringSpan& str) const { return *this + str; }

String String::ToUpper() const
{
	String str(m_Length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		((char*)str.m_Data)[i] = toupper(m_Data[i]);
	}

	return str;
}

String String::ToLower() const
{
	String str(m_Length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		((char*)str.m_Data)[i] = tolower(m_Data[i]);
	}

	return str;
}

String String::Append(const StringSpan& str) const
{
	String strOut(m_Length + str.m_Length);
	Xe::Memory::Copy((char*)strOut.m_Data, m_Data, m_Length);
	Xe::Memory::Copy((char*)strOut.m_Data + m_Length, str.m_Data, str.m_Length);
	return strOut;
}

String String::Insert(int position, const StringSpan& str) const
{
	ASSERT_POSITIVE(position);
	if (position > m_Length)
		ASSERT_POSITION(position);

	String strOut(m_Length + str.m_Length);
	Xe::Memory::Copy((char*)strOut.m_Data, m_Data, position);
	Xe::Memory::Copy((char*)strOut.m_Data + position, str.m_Data, str.m_Length);
	Xe::Memory::Copy((char*)strOut.m_Data + position + str.m_Length, m_Data + position, m_Length - position);
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
		Xe::Memory::Copy((char*)strOut.m_Data, m_Data, startIndex);
		Xe::Memory::Copy((char*)strOut.m_Data + startIndex, m_Data + startIndex + length, m_Length - startIndex - length);
		return strOut;
	}
}

String String::Replace(char chOld, char chNew) const
{
	String str(m_Length);
	for (int i = 0; i < m_Length; ++i)
		((char*)str.m_Data)[i] = m_Data[i] == chOld ? chNew : m_Data[i];
	return str;
}

String String::Replace(const StringSpan& strOld, const StringSpan& strNew) const
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
			char *strFound = strstr((char*)m_Data + index, strOld.m_Data);
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
			memcpy((char*)str.m_Data + str.m_Length, m_Data + index, newIndex - index);
			str.m_Length += newIndex - index;
		}

		if (strNew.m_Length > 0)
		{
			memcpy((char*)str.m_Data + str.m_Length, strNew.m_Data, strNew.m_Length);
			str.m_Length += strNew.m_Length;
		}

		index = strOld.m_Length + newIndex;
	}

	if (index < m_Length)
	{
		memcpy((char*)str.m_Data + str.m_Length, m_Data + m_Length - (newStrLength - str.m_Length), newStrLength - str.m_Length);
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
		((char*)str.m_Data)[i] = ch;
	Xe::Memory::Copy((char*)str.m_Data + totalWidth, m_Data, m_Length);

	return str;
}

String String::PadRight(int totalWidth, char ch) const
{
	ASSERT_POSITIVE(totalWidth);
	if (totalWidth == 0)
		return *this;

	String str(m_Length + totalWidth);
	Xe::Memory::Copy((char*)str.m_Data, m_Data, m_Length);
	for (int i = 0; i < totalWidth; i++)
		((char*)str.m_Data)[m_Length + i] = ch;
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

int String::ToUpper(int ch)
{
	return toupper(ch);
}

int String::ToLower(int ch)
{
	return tolower(ch);
}

int String::Compare(const StringSpan& stra, const StringSpan& strb)
{
	return StringSpan::Compare(stra, strb);
}

int String::CompareInsensitive(const StringSpan& stra, const StringSpan& strb)
{
	return StringSpan::CompareInsensitive(stra, strb);
}

String String::Join(char separator, const StringSpan* strs, int count)
{
	// TODO optimize
	char s[] = { separator, '\0' };
	return Join(StringSpan(s), strs, count);
}

String String::Join(const StringSpan& separator, const StringSpan* strs, int count)
{
	// TODO optimize
	Xe::String str;
	for (int i = 0; i < count - 1; i++)
	{
		str = StringSpan(str + strs[i] + separator);
	}

	return str + strs[count - 1];
}

String String::FromInt(int value, int base)
{
	return base == 10 ?
		FromSigned<int>(value, base) :
		FromUnsigned<unsigned int>(value, base);
}

String String::FromUInt(unsigned int value, int base)
{
	return FromUnsigned<unsigned int>(value, base);
}

String String::FromLong(long value, int base)
{
	return FromSigned<long>(value, base);
}

String String::FromULong(unsigned long value, int base)
{
	return FromUnsigned<unsigned long>(value, base);
}