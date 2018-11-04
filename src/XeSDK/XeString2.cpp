#include "pch.h"
#include <XeSDK/XeString2.h>
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMemory.h>
#include <string>
#include "XeStringInternal.h"

using namespace Xe;

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

String2 String2::Empty("");

String2::String2(int length) :
	m_Length(length),
	m_Data(Xe_FastHeap_Alloc<char>(length + 1))
{
	ASSERT_POSITIVE(length);
	m_Data[length] = '\0';
}

String2::String2() :
	String2(Empty)
{ }

String2::String2(const StringSpan& string) :
	String2(string.GetLength())
{
	Xe::Memory::Copy((char*)m_Data, string.GetData(), m_Length * sizeof(char));
}

String2::String2(const String2& string) :
	String2(StringSpan(string.m_Data, string.m_Length))
{ }

String2::~String2()
{
	Xe_FastHeap_Free<char>(m_Data);
}

void String2::CheckRangeIndex(int startIndex, int length) const
{
	ASSERT_POSITION(startIndex);
	ASSERT_POSITIVE(length);
	if (startIndex + length > m_Length)
	{
		throw std::invalid_argument(NAMEOF(length)" must be in the string range");
	}
}

String2::operator const char*() const
{
	return m_Data;
}

String2::operator StringSpan() const
{
	return StringSpan(m_Data, m_Length);
}

char String2::operator [](int index)
{
	ASSERT_POSITION(index);
	return m_Data[index];
}

String2& String2::operator =(const StringSpan& str)
{
	// TODO optimize
	return *this = String2(str);
}

String2& String2::operator =(const String2& str)
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

String2 String2::operator +(const StringSpan& str) const
{
	return Append(str);
}

String2 String2::operator +(const String2& str) const
{
	return Append(str);
}

String2 String2::operator +=(const StringSpan& str) const { return *this + str; }
String2 String2::operator +=(const String2& str) const { return *this + str; }
//bool String2::operator == (const StringSpan& str) const { return Compare(*this, str) == 0; }
bool String2::operator == (const String2& str) const { return Compare(*this, str) == 0; }
bool String2::operator != (const String2& str) const { return Compare(*this, str) != 0; }
bool String2::operator > (const String2& str) const { return Compare(*this, str) > 0; }
bool String2::operator >= (const String2& str) const { return Compare(*this, str) >= 0; }
bool String2::operator < (const String2& str) const { return Compare(*this, str) < 0; }
bool String2::operator <= (const String2& str) const { return Compare(*this, str) <= 0; }

int String2::GetLength() const { return m_Length; }

const char* String2::GetData() const { return m_Data; }

bool String2::IsEmpty() const
{
	return GetLength() == 0;
}

bool String2::IsEmptyOrWhitespace() const
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

bool String2::StartsWith(const StringSpan& str) const
{
	return IndexOf(str) == 0;
}

bool String2::StartsWith(const String2& str) const
{
	return IndexOf(str) == 0;
}

bool String2::EndsWith(const StringSpan& str) const
{
	return LastIndexOf(str) == GetLength() - str.GetLength();
}

bool String2::EndsWith(const String2& str) const
{
	return LastIndexOf(str) == GetLength() - str.GetLength();
}

int String2::IndexOf(char ch) const
{
	for (int i = 0; i < m_Length; i++)
	{
		if (m_Data[i] == ch)
			return i;
	}

	return -1;
}

int String2::IndexOf(const StringSpan& str) const
{
	// TODO optimize
	return IndexOf(String2(str));
}

int String2::IndexOf(const String2& str) const
{
	char* r = strstr(m_Data, str.m_Data);
	if (r)
	{
		return (int)(r - m_Data);
	}
	return -1;
}

int String2::IndexOfAny(const StringSpan& chs) const
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

int String2::LastIndexOf(char ch) const
{
	for (int i = m_Length - 1; i >= 0; --i)
	{
		if (m_Data[i] == ch)
			return i;
	}

	return -1;
}

int String2::LastIndexOf(const StringSpan& str) const
{
	// TODO optimize
	return LastIndexOf(String2(str));
}

int String2::LastIndexOf(const String2& str) const
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

int String2::LastIndexOfAny(const StringSpan& chs) const
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

String2 String2::ToUpper() const
{
	String2 str(m_Length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		str.m_Data[i] = toupper(m_Data[i]);
	}

	return str;
}

String2 String2::ToLower() const
{
	String2 str(m_Length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		str.m_Data[i] = tolower(m_Data[i]);
	}

	return str;
}

String2 String2::Substring(int startIndex) const
{
	return Substring(startIndex, m_Length - startIndex);
}

String2 String2::Substring(int startIndex, int length) const
{
	CheckRangeIndex(startIndex, length);

	String2 str(length);
	for (int i = 0; i < str.m_Length; ++i)
	{
		str.m_Data[i] = m_Data[startIndex + i];
	}

	return str;
}

String2 String2::Append(const StringSpan& str) const
{
	// TODO optimize
	return Append(String2(str));
}

String2 String2::Append(const String2& str) const
{
	String2 strOut(m_Length + str.m_Length);
	Xe::Memory::Copy(strOut.m_Data, m_Data, m_Length);
	Xe::Memory::Copy(strOut.m_Data + m_Length, str.m_Data, str.m_Length);
	return strOut;
}

String2 String2::Insert(int position, const StringSpan& str) const
{
	// TODO optimize
	return Insert(position, String2(str));
}

String2 String2::Insert(int position, const String2& str) const
{
	ASSERT_POSITIVE(position);
	if (position > m_Length)
		ASSERT_POSITION(position);

	String2 strOut(m_Length + str.m_Length);
	Xe::Memory::Copy(strOut.m_Data, m_Data, position);
	Xe::Memory::Copy(strOut.m_Data + position, str.m_Data, str.m_Length);
	Xe::Memory::Copy(strOut.m_Data + position + str.m_Length, m_Data + position, m_Length - position);
	return strOut;
}

String2 String2::Remove(int startIndex) const
{
	return Remove(startIndex, m_Length - startIndex);
}

String2 String2::Remove(int startIndex, int length) const
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
		return String2(m_Data + length);
	}
	else if (startIndex + length == m_Length)
	{
		// Remove at the end
		return String2(StringSpan(m_Data, m_Length - length));
	}
	else
	{
		// Remove between
		String2 strOut(m_Length - length);
		Xe::Memory::Copy(strOut.m_Data, m_Data, startIndex);
		Xe::Memory::Copy(strOut.m_Data + startIndex, m_Data + startIndex + length, m_Length - startIndex - length);
		return strOut;
	}
}

String2 String2::Replace(char chOld, char chNew) const
{
	String2 str(m_Length);
	for (int i = 0; i < m_Length; ++i)
		str.m_Data[i] = m_Data[i] == chOld ? chNew : m_Data[i];
	return str;
}

String2 String2::Replace(const StringSpan& strOld, const StringSpan& strNew) const
{
	// TODO optimize
	return Replace(Xe::String2(strOld), Xe::String2(strNew));
}

String2 String2::Replace(const String2& strOld, const String2& strNew) const
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

	String2 str(newStrLength);
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

String2 String2::PadLeft(int totalWidth, char ch) const
{
	ASSERT_POSITIVE(totalWidth);
	if (totalWidth == 0)
		return *this;

	String2 str(m_Length + totalWidth);
	for (int i = 0; i < totalWidth; i++)
		str.m_Data[i] = ch;
	Xe::Memory::Copy(str.m_Data + totalWidth, m_Data, m_Length);

	return str;
}

String2 String2::PadRight(int totalWidth, char ch) const
{
	ASSERT_POSITIVE(totalWidth);
	if (totalWidth == 0)
		return *this;

	String2 str(m_Length + totalWidth);
	Xe::Memory::Copy(str.m_Data, m_Data, m_Length);
	for (int i = 0; i < totalWidth; i++)
		str.m_Data[m_Length + i] = ch;
	return str;
}

String2 String2::Trim() const
{
	// TODO optimize
	return TrimStart().TrimEnd();
}

String2 String2::TrimStart() const
{
	int startIndex;
	for (startIndex = 0; startIndex < m_Length && m_Data[startIndex] == ' '; ++startIndex);
	return String2(StringSpan(m_Data + startIndex, m_Length - startIndex));
}

String2 String2::TrimEnd() const
{
	int endIndex;
	for (endIndex = m_Length - 1; endIndex >= 0 && m_Data[endIndex] == ' '; --endIndex);
	return String2(StringSpan(m_Data, endIndex + 1));
	return String2();
}

int String2::Compare(const StringSpan& stra, const StringSpan& strb)
{
	return strncmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String2::Compare(const String2& stra, const StringSpan& strb)
{
	return strncmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String2::Compare(const StringSpan& stra, const String2& strb)
{
	return strncmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String2::Compare(const String2& stra, const String2& strb)
{
	return strncmp(stra.m_Data, strb.m_Data, Xe::Math::Min(stra.m_Length, strb.m_Length));
}

int String2::CompareInsensitive(const StringSpan& stra, const StringSpan& strb)
{
	return _strnicmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String2::CompareInsensitive(const StringSpan& stra, const String2& strb)
{
	return _strnicmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String2::CompareInsensitive(const String2& stra, const StringSpan& strb)
{
	return _strnicmp(stra.GetData(), strb.GetData(), Math::Min(stra.GetLength(), strb.GetLength()));
}

int String2::CompareInsensitive(const String2& stra, const String2& strb)
{
	return _strnicmp(stra.m_Data, strb.m_Data, Xe::Math::Min(stra.m_Length, strb.m_Length));
}

String2 String2::Join(char separator, const StringSpan* strs, int count)
{
	// TODO optimize
	char s[] = { separator, '\0' };
	return Join(String2(s), strs, count);
}

String2 String2::Join(char separator, const String2* strs, int count)
{
	char s[] = { separator, '\0' };
	return Join(String2(s), strs, count);
}

String2 String2::Join(const StringSpan& separator, const StringSpan* strs, int count)
{
	// TODO optimize
	String2* _strs = new String2[count];
	for (int i = 0; i < count; i++)
	{
		_strs[i] = String2(strs[i]);
	}

	return Join(separator, _strs, count);
}

String2 String2::Join(const String2& separator, const StringSpan* strs, int count)
{
	// TODO optimize
	String2* _strs = new String2[count];
	for (int i = 0; i < count; i++)
	{
		_strs[i] = String2(strs[i]);
	}

	return Join(separator, _strs, count);
}

String2 String2::Join(const StringSpan& separator, const String2* strs, int count)
{
	// TODO optimize
	Xe::String2 str;
	for (int i = 0; i < count - 1; i++)
	{
		str = str + strs[i] + separator;
	}

	return str + strs[count - 1];
}


String2 String2::Join(const String2& separator, const String2* strs, int count)
{
	// TODO optimize
	Xe::String2 str;
	for (int i = 0; i < count - 1; i++)
	{
		str = str + strs[i] + separator;
	}

	return str + strs[count - 1];
}
