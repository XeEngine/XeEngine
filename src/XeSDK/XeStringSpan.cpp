#include "pch.h"
#include <XeSDK/XeStringSpan.h>
#include <XeSDK/XeMath.h>
#include <string>
#include "XeStringInternal.h"

using namespace Xe;

StringSpan::StringSpan(const char* str) :
	m_Length(strlen(str)),
	m_Data(str)
{ }

StringSpan::StringSpan(const char* str, int length) :
	m_Length(length),
	m_Data(str)
{
	ASSERT_POSITIVE(length);
}

char StringSpan::operator [](int index) { return m_Data[index]; }

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