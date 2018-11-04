#pragma once
#include <XeSDK/XeString2.h>
#include <XeSDK/XeStringSpan.h>

namespace Xe
{
	class String2
	{
		int m_Length;
		char* m_Data;

		//! \brief Pre-allocate a string and append the terminator at the end
		//! \param[in] length Capacity of string
		String2(int length);

		void CheckRangeIndex(int startIndex, int length) const;
	public:
		//! \brief Definition of an empty string
		static String2 Empty;

		//! \brief Create an empty string
		String2();

		//! \brief Create a string from a StringSpan
		//! \param[in] string to copy
		String2(const StringSpan& string);
		
		//! \brief Create a string from another string
		//! \param[in] string to copy
		String2(const String2& string);

		~String2();

		operator const char*() const;
		operator StringSpan() const;
		char operator [](int index);
		String2& operator =(const StringSpan& str);
		String2& operator =(const String2& str);
		
		template <size_t length>
		String2 operator +(const char(&str)[length])
		{
			return *this + StringSpan(str);
		}

		String2 operator +(const StringSpan& str) const;

		String2 operator +(const String2& str) const;

		template <size_t length>
		String2 operator +=(const char(&str)[length])
		{
			return *this + StringSpan(str);
		}

		String2 operator +=(const StringSpan& str) const;

		String2 operator +=(const String2& str) const;

		template <size_t length>
		bool operator ==(const char(&str)[length])
		{
			return *this == String2(str);
		}

		//bool operator == (const StringSpan& str) const;

		bool operator == (const String2& str) const;

		bool operator != (const String2& str) const;

		bool operator > (const String2& str) const;

		bool operator >= (const String2& str) const;

		bool operator < (const String2& str) const;

		bool operator <= (const String2& str) const;

		//! \brief Length of the string
		//! \return Length of the string, excluding the terminator '\0'
		int GetLength() const;

		//! \brief Get a C-like string
		//! \return pointer
		const char* GetData() const;

		bool IsEmpty() const;

		bool IsEmptyOrWhitespace() const;

		bool StartsWith(const StringSpan& str) const;

		bool StartsWith(const String2& str) const;

		bool EndsWith(const StringSpan& str) const;

		bool EndsWith(const String2& str) const;

		int IndexOf(char ch) const;

		int IndexOf(const StringSpan& str) const;

		int IndexOf(const String2& str) const;

		int IndexOfAny(const StringSpan& chs) const;

		int LastIndexOf(char ch) const;

		int LastIndexOf(const StringSpan& str) const;

		int LastIndexOf(const String2& str) const;

		int LastIndexOfAny(const StringSpan& chs) const;

		//! \brief Get the string as uppercase
		/** \details From "TeSt" to "TEST".
		 */
		String2 ToUpper() const;

		//! \brief Get the string as lowercase
		/** \details From "TeSt" to "test".
		 */
		String2 ToLower() const;

		String2 Substring(int startIndex) const;

		String2 Substring(int startIndex, int length) const;

		String2 Append(const StringSpan& str) const;

		String2 Append(const String2& str) const;

		String2 Insert(int position, const StringSpan& str) const;

		String2 Insert(int position, const String2& str) const;

		String2 Remove(int startIndex) const;

		String2 Remove(int startIndex, int length) const;

		String2 Replace(char chDst, char chSrc) const;

		String2 Replace(const StringSpan& strDst, const StringSpan& strSrc) const;

		String2 Replace(const String2& strDst, const String2& strSrc) const;

		String2 PadLeft(int totalWidth, char ch = ' ') const;

		String2 PadRight(int totalWidth, char ch = ' ') const;

		String2 Trim() const;

		String2 TrimStart() const;

		String2 TrimEnd() const;

		// TODO SPLIT

		static int Compare(const StringSpan& stra, const StringSpan& strb);
		static int Compare(const String2& stra, const StringSpan& strb);
		static int Compare(const StringSpan& stra, const String2& strb);
		static int Compare(const String2& stra, const String2& strb);

		static int CompareInsensitive(const StringSpan& stra, const StringSpan& strb);
		static int CompareInsensitive(const StringSpan& stra, const String2& strb);
		static int CompareInsensitive(const String2& stra, const StringSpan& strb);
		static int CompareInsensitive(const String2& stra, const String2& strb);

		template <size_t length>
		static String2 Join(char separator, const StringSpan (&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(char separator, String2(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(const StringSpan& separator, const StringSpan(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(const String2& separator, const StringSpan(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(const StringSpan& separator, String2(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(const String2& separator, String2(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		static String2 Join(char separator, const StringSpan* strs, int count);

		static String2 Join(char separator, const String2* strs, int count);

		static String2 Join(const StringSpan& separator, const StringSpan* strs, int count);

		static String2 Join(const String2& separator, const StringSpan* strs, int count);

		static String2 Join(const StringSpan& separator, const String2* strs, int count);

		static String2 Join(const String2& separator, const String2* strs, int count);
	};
}