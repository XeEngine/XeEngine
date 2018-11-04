#pragma once
#include <XeSDK/XeString.h>
#include <XeSDK/XeStringSpan.h>

namespace Xe
{
	//! \brief Immutable object that creates a new string from an exising one
	/** \details By default, for performance reasons, all the comparison between
	 * strings are case-sensitive, unless specified by the method name or the
	 * comparison parameters.
	 */
	class String
	{
		int m_Length;
		char* m_Data;

		//! \brief Pre-allocate a string and append the terminator at the end
		//! \param[in] length Capacity of string
		String(int length);

		//! \brief Check if the specified range is valid, throwing an exception if not
		void CheckRangeIndex(int startIndex, int length) const;
	public:
		//! \brief Definition of an empty string
		static String Empty;

		//! \brief Create an empty string
		String();

		//! \brief Create a string from a StringSpan
		//! \param[in] string to copy
		String(const StringSpan& string);
		
		//! \brief Create a string from another string
		//! \param[in] string to copy
		String(const String& string);

		//! \brief Deallocate the string from the memory
		~String();

		//! \brief Get the raw string pointer as C-like string
		//! \sa GetData
		operator const char*() const;

		//! \brief Get a StringSpan object from the current string
		operator StringSpan() const;

		//! \brief Raw access to the string, char by char
		char operator [](int index) const;
		
		//! \brief Assign a StringSpan to the existing string
		/** \details This is the only exception where the object is mutable.
		 * For performance reason, is preferable to re-use the existing
		 * allocated memory instead to free and allocate it once again.
		 */
		String& operator =(const StringSpan& str);

		//! \brief Assign a StringSpan to the existing string
		/** \details This is the only exception where the object is mutable.
		 * For performance reason, is preferable to re-use the existing
		 * allocated memory instead to free and allocate it once again.
		 */
		String& operator =(const String& str);
		
		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		template <size_t length>
		String operator +(const char(&str)[length])
		{
			return *this + StringSpan(str);
		}

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		String operator +(const StringSpan& str) const;

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		String operator +(const String& str) const;

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		template <size_t length>
		String operator +=(const char(&str)[length])
		{
			return *this + StringSpan(str);
		}

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		String operator +=(const StringSpan& str) const;

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		String operator +=(const String& str) const;

		//! \brief Check for equality using a case sensitive comparation
		//! \param str String to compare
		//! \sa Compare
		template <size_t length>
		bool operator ==(const char(&str)[length])
		{
			return *this == String(str);
		}

		//bool operator == (const StringSpan& str) const;

		//! \brief Check for equality using a case sensitive comparation
		//! \param str String to compare
		//! \sa Compare
		bool operator == (const String& str) const;

		//! \brief Check for unequality using a case sensitive comparation
		//! \param str String to compare
		//! \sa Compare
		bool operator != (const String& str) const;

		//! \brief Check if the string is greater than the specified string
		//! \param str String to compare
		//! \sa Compare
		bool operator > (const String& str) const;

		//! \brief Check if the string is greater or equal than the specified string
		//! \param str String to compare
		//! \sa Compare
		bool operator >= (const String& str) const;

		//! \brief Check if the string is less than the specified string
		//! \param str String to compare
		//! \sa Compare
		bool operator < (const String& str) const;

		//! \brief Check if the string is less or equal than the specified string
		//! \param str String to compare
		//! \sa Compare
		bool operator <= (const String& str) const;

		//! \brief Length of the string
		//! \return Length of the string, excluding the terminator '\0'
		int GetLength() const;

		//! \brief Get a C-like string
		//! \return Raw pointer
		const char* GetData() const;

		//! \brief Check if the string is empty
		//! \return True if GetLength is 0
		//! \sa GetLength
		bool IsEmpty() const;

		//! \brief Check if the string is empty or contains white spaces
		//! \return True if GetLength is 0 or if the string contains only ' '
		//! \sa GetLength
		bool IsEmptyOrWhitespace() const;

		//! \brief Check if the string start with the specified string
		/** \details Examples:
		 * "Hello world".StartsWith("Hello") = true
		 * "Hello world".StartsWith("hello") = false
		 * "Hello world".StartsWith("ello") = false
		 */
		bool StartsWith(const StringSpan& str) const;

		//! \brief Check if the string start with the specified string
		/** \details Examples:
		 * "Hello world".StartsWith("Hello") = true
		 * "Hello world".StartsWith("hello") = false
		 * "Hello world".StartsWith("ello") = false
		 */
		bool StartsWith(const String& str) const;

		//! \brief Check if the string ends with the specified string
		/** \details Examples:
		 * "Hello world".StartsWith("world") = true
		 * "Hello world".StartsWith("worl") = false
		 * "Hello world".StartsWith("World") = false
		 */
		bool EndsWith(const StringSpan& str) const;

		//! \brief Check if the string ends with the specified string
		/** \details Examples:
		 * "Hello world".StartsWith("world") = true
		 * "Hello world".StartsWith("worl") = false
		 * "Hello world".StartsWith("World") = false
		 */
		bool EndsWith(const String& str) const;

		//! \brief Find the index of the first specified character's instance
		//! \param ch Character to find
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa LastIndexOf
		int IndexOf(char ch) const;

		//! \brief Find the index of the first specified string's instance
		//! \param str String to find
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa LastIndexOf
		int IndexOf(const StringSpan& str) const;

		//! \brief Find the index of the first specified string's instance
		//! \param str String to find
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa LastIndexOf
		int IndexOf(const String& str) const;

		//! \brief Find the index of the first instance between the specified characters
		//! \param chs Group of characters, where at least one of them should match
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa LastIndexOf
		int IndexOfAny(const StringSpan& chs) const;

		//! \brief Find the index of the last specified character's instance
		//! \param ch Character to find
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa IndexOf
		int LastIndexOf(char ch) const;

		//! \brief Find the index of the last specified string's instance
		//! \param str String to find
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa IndexOf
		int LastIndexOf(const StringSpan& str) const;

		//! \brief Find the index of the last specified string's instance
		//! \param str String to find
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa IndexOf
		int LastIndexOf(const String& str) const;

		//! \brief Find the index of the last instance between the specified characters
		//! \param chs Group of characters, where at least one of them should match
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa IndexOf
		int LastIndexOfAny(const StringSpan& chs) const;

		//! \brief Create the same string, but where every letter is capital
		String ToUpper() const;

		//! \brief Create the same string, but where every letter is lowercase
		String ToLower() const;

		//! \brief Create a sub-string
		//! \param startIndex The new string will start from the specified index
		String Substring(int startIndex) const;

		//! \brief Create a sub-string
		//! \param startIndex The new string will start from the specified index
		//! \param length Length of the characters to take
		String Substring(int startIndex, int length) const;

		//! \brief Append a string
		//! \param str String to append
		String Append(const StringSpan& str) const;

		//! \brief Append a string
		//! \param str String to append
		String Append(const String& str) const;

		//! \brief Insert a string to the specified position
		//! \param position Position to insert the specified string
		//! \param str String to append
		String Insert(int position, const StringSpan& str) const;

		//! \brief Insert a string to the specified position
		//! \param position Position to insert the specified string
		//! \param str String to append
		String Insert(int position, const String& str) const;

		//! \brief Remove the specified string content
		//! \param startIndex Remove from the specified index
		String Remove(int startIndex) const;

		//! \brief Remove the specified string content
		//! \param startIndex Remove from the specified index
		//! \param length Remove the specified amount of characters
		String Remove(int startIndex, int length) const;

		//! \brief Replace by character
		//! \param chOld Character to be replaced
		//! \param chNew Character replaced
		String Replace(char chOld, char chNew) const;

		//! \brief Replace by string
		//! \param strOld String to be replaced
		//! \param strNew String replaced
		String Replace(const StringSpan& strOld, const StringSpan& strNew) const;

		//! \brief Replace by string
		//! \param strOld String to be replaced
		//! \param strNew String replaced
		String Replace(const String& strOld, const String& strNew) const;

		//! \brief Insert the specified amount of characters at the beginning
		//! \param count Number of character to insert 
		//! \param ch The character to insert
		//! \sa PadRight
		String PadLeft(int count, char ch = ' ') const;

		//! \brief Append the specified amount of characters at the end
		//! \param count Number of character to append 
		//! \param ch The character to insert
		//! \sa PadLeft
		String PadRight(int count, char ch = ' ') const;

		//! \brief Remove all the spaces from the beginning and the end of the string
		//! \sa TrimStart, TrimEnd
		String Trim() const;

		//! \brief Remove all the spaces at the beginning of the string
		//! \sa TrimStart, TrimEnd
		String TrimStart() const;

		//! \brief Remove all the spaces at the end of the string
		//! \sa TrimStart, TrimEnd
		String TrimEnd() const;

		// TODO SPLIT
		
		//! \brief Check if the character is a space or not
		//! \param[in] ch Character to analyze
		//! \return true if the condition is verified
		static bool IsSpace(int ch);

		//! \brief Check if the character is alphabetic or not
		//! \param[in] ch Character to analyze
		//! \return true if the condition is verified
		static bool IsAlpha(int ch);

		//! \brief Check if the character is a number or not
		//! \param[in] ch Character to analyze
		//! \return true if the condition is verified
		static bool IsDecimal(int ch);

		//! \brief Check if the character is alphanumeric or not
		//! \param[in] ch Character to analyze
		//! \return true if the condition is verified
		static bool IsAlphaNum(int ch);

		//! \brief Check if the character is an hexadecimal value or not
		//! \param[in] ch Character to analyze
		//! \return true if the condition is verified
		static bool IsHexadecimal(int ch);

		//! \brief Check if character is a lower character
		//! \param[in] ch Character to analyze
		//! \return true if condition is verified
		static bool IsLower(int ch);

		//! \brief Check if character is an upper character
		//! \param[in] ch Character to analyze
		//! \return true if condition is verified
		static bool IsUpper(int ch);

		//! \brief Get the length of the specified C-like string
		//! \return Length of the string until the null terminator
		static int GetLength(const char* str);

		static int Compare(const StringSpan& stra, const StringSpan& strb);
		static int Compare(const String& stra, const StringSpan& strb);
		static int Compare(const StringSpan& stra, const String& strb);
		static int Compare(const String& stra, const String& strb);

		static int CompareInsensitive(const StringSpan& stra, const StringSpan& strb);
		static int CompareInsensitive(const StringSpan& stra, const String& strb);
		static int CompareInsensitive(const String& stra, const StringSpan& strb);
		static int CompareInsensitive(const String& stra, const String& strb);

		template <size_t length>
		static String Join(char separator, const StringSpan (&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String Join(char separator, String(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String Join(const StringSpan& separator, const StringSpan(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String Join(const String& separator, const StringSpan(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String Join(const StringSpan& separator, String(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <size_t length>
		static String Join(const String& separator, String(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		static String Join(char separator, const StringSpan* strs, int count);

		static String Join(char separator, const String* strs, int count);

		static String Join(const StringSpan& separator, const StringSpan* strs, int count);

		static String Join(const String& separator, const StringSpan* strs, int count);

		static String Join(const StringSpan& separator, const String* strs, int count);

		static String Join(const String& separator, const String* strs, int count);
	};
}