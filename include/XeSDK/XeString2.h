#pragma once
#include <XeSDK/XeString2.h>

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

		//! \brief Create a string from a C-style string
		//! \param[in] string to copy
		String2(const char* string);

		//! \brief Create a string from a C-style string with a specified length
		//! \param[in] string String to copy
		//! \param[in] length Number of characters to copy
		String2(const char* string, int length);

		//! \brief Create a string from another string
		//! \param[in] string to copy
		String2(const String2& string);
		~String2();

		operator const char*() const;
		char& operator [](int index);
		String2& operator =(const char* str);
		String2& operator =(const String2& str);
		String2 operator +(const char* str) const;
		String2 operator +(const String2& str) const;
		String2 operator +=(const char* str) const;
		String2 operator +=(const String2& str) const;
		bool operator == (const char* str) const;
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

		bool StartsWith(const char* str) const;

		bool StartsWith(const String2& str) const;

		bool EndsWith(const char* str) const;

		bool EndsWith(const String2& str) const;

		int IndexOf(char ch) const;

		int IndexOf(const char* str) const;

		int IndexOf(const String2& str) const;

		int IndexOfAny(const char* chs) const;

		int IndexOfAny(const char* chs, int count) const;

		int LastIndexOf(char ch) const;

		int LastIndexOf(const char* str) const;

		int LastIndexOf(const String2& str) const;

		int LastIndexOfAny(const char* chs) const;

		int LastIndexOfAny(const char* chs, int count) const;

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

		String2 Append(const char* str) const;

		String2 Append(const String2& str) const;

		String2 Insert(int position, const char* str) const;

		String2 Insert(int position, const String2& str) const;

		String2 Remove(int startIndex) const;

		String2 Remove(int startIndex, int length) const;

		String2 Replace(char chDst, char chSrc) const;

		String2 Replace(const char* strDst, const char* strSrc) const;

		String2 Replace(const String2& strDst, const String2& strSrc) const;

		String2 PadLeft(int totalWidth, char ch = ' ') const;

		String2 PadRight(int totalWidth, char ch = ' ') const;

		String2 Trim() const;

		String2 TrimStart() const;

		String2 TrimEnd() const;

		// TODO SPLIT

		static int Compare(const String2& stra, const String2& strb);

		static int CompareInsensitive(const String2& stra, const String2& strb);

		template <size_t length>
		static String2 Join(char separator, const char* (&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(char separator, String2(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(const String2& separator, const char* (&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String2 Join(const String2& separator, String2(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		static String2 Join(char separator, const char** strs, int count);

		static String2 Join(char separator, const String2* strs, int count);

		static String2 Join(const String2& separator, const char** strs, int count);

		static String2 Join(const String2& separator, const String2* strs, int count);
	};
}