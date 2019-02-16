#pragma once
#include <XeSDK/XeString.h>
#include <XeSDK/XeStringSpan.h>

namespace Xe
{
	//! \brief Immutable object that creates a new string from an exising one
	/** \details Each String object makes a memory allocation. It is suitable if
	 * intent is to create a string that persists in the heap and that it will
	 * be automatically freed, but can be expensive when using string operations.
	 * The object can be used as a C-like string with a null terminator.
	 * By default, for performance reasons, all the comparison between
	 * strings are case-sensitive, unless specified by the method name or the
	 * comparison parameters.
	 */
	 //! \sa StringSpan
	class String : public StringSpan
	{
		typedef StringSpan Base;

		//! \brief Pre-allocate a string and append the terminator at the end
		//! \param[in] length Capacity of string
		String(int length);
	public:
		//! \brief Definition of an empty string
		static String Empty;

		//! \brief Create an empty string
		String();

		template <unsigned maxLength>
		String(const char(&str)[maxLength]) :
			String(StringSpan(str, maxLength, maxLength - 1))
		{ }

		template <unsigned maxLength>
		String(const char(&str)[maxLength], int length) :
			String(StringSpan(str, maxLength, length))
		{ }

		//! \brief Create a string from a C-style string
		//! \param[in] string to copy
		String(const char* str);

		//! \brief Create a string from an existing, but trimming it
		//! \param[in] string to copy
		//! \param[in] length new length
		String(const char* str, int length);

		//! \brief Create a string from a StringSpan
		//! \param[in] string to copy
		String(const StringSpan& string);

		//! \brief Create a string from another string
		//! \param[in] string to copy
		String(const String& string);

		//! \brief Create a string from another string
		//! \param[in] string to copy
		String(String&& string) noexcept;

		//! \brief Deallocate the string from the memory
		~String() noexcept;

		//! \brief Get the raw string pointer as C-like string
		//! \sa GetData
		operator const char*() const;

		String& operator =(const char* str);

		//! \brief Assign a StringSpan to the existing string
		/** \details This is the only exception where the object is mutable.
		 * For performance reason, is preferable to re-use the existing
		 * allocated memory instead to free and allocate it once again.
		 */
		String& operator =(const StringSpan& str);

		String& operator =(const String& str);

		String& operator =(String&& other) noexcept;

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		String operator +(const StringSpan& str) const;

		//! \brief Append the specified string
		//! \param str String to append
		//! \sa Append
		String operator +=(const StringSpan& str) const;

		//! \brief Create the same string, but where every letter is capital
		String ToUpper() const;

		//! \brief Create the same string, but where every letter is lowercase
		String ToLower() const;

		//! \brief Append a string
		//! \param str String to append
		String Append(const StringSpan& str) const;

		//! \brief Insert a string to the specified position
		//! \param position Position to insert the specified string
		//! \param str String to append
		String Insert(int position, const StringSpan& str) const;

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

		//! \brief Convert a character to upper-case
		static int ToUpper(int ch);

		//! \brief Convert a character to lower-case
		static int ToLower(int ch);

		static int Compare(const StringSpan& stra, const StringSpan& strb);

		static int CompareInsensitive(const StringSpan& stra, const StringSpan& strb);

		template <unsigned length>
		static String Join(char separator, const StringSpan (&strs)[length])
		{
			return Join(separator, strs, length);
		}

		template <unsigned length>
		static String Join(const StringSpan& separator, const StringSpan(&strs)[length])
		{
			return Join(separator, strs, length);
		}

		static String Join(char separator, const StringSpan* strs, int count);

		static String Join(const StringSpan& separator, const StringSpan* strs, int count);

		//! \brief Returns a string from the specified value
		//! \param value Value to strinfigy
		//! \param base Integer base; the valid values are 2, 8, 10 and 16
		static String FromInt(int value, int base = 10);

		//! \brief Returns a string from the specified value
		//! \param value Value to strinfigy
		//! \param base Integer base; the valid values are 2, 8, 10 and 16
		static String FromUInt(unsigned int value, int base = 10);

		//! \brief Returns a string from the specified value
		//! \param value Value to strinfigy
		//! \param base Integer base; the valid values are 2, 8, 10 and 16
		static String FromLong(long value, int base = 10);

		//! \brief Returns a string from the specified value
		//! \param value Value to strinfigy
		//! \param base Integer base; the valid values are 2, 8, 10 and 16
		static String FromULong(unsigned long value, int base = 10);
	};
}