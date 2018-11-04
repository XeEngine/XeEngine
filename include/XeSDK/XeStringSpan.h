#pragma once

namespace Xe
{
	//! \brief Holds a memory reference to a string
	/** \details This high-performance object holds a reference to an existing
	 * string and does not perform any memory allocation. It does calculate the
	 * size of the string from the beginning, improving security and performance.
	 * The raw pointer cannot be used as C-like string, since the object does
	 * not modify the original string when the length changes. Also if the
	 * original string has been allocated to the heap, this will not
	 * automatically free it on object destruction.
	 */
	//! \sa String
	class String;
	class StringSpan
	{
		friend class String;
	protected:
		int m_Length;
		const char* m_Data;
	public:
		//! \brief Definition of an empty string
		static StringSpan Empty;

		template <size_t length>
		StringSpan(const char(&str)[length]) :
			StringSpan(str, length - 1)
		{ }

		//! \brief Create a string from a C-style string
		//! \param[in] string to copy
		StringSpan(const char* str);

		//! \brief Create a string from a C-style string with a specified length
		//! \param[in] string String to copy
		//! \param[in] length Number of characters to copy
		StringSpan(const char* str, int length);

		//! \brief Raw access to the string, char by char
		char operator [](int index) const;

		StringSpan operator + (int index) const;

		StringSpan operator += (int index) const;

		//! \brief Assign a StringSpan to the existing one
		StringSpan& operator =(const StringSpan& str);

		//! \brief Check for equality using a case sensitive comparation
		//! \param str String to compare
		bool operator == (const StringSpan& str) const;

		//! \brief Check for unequality using a case sensitive comparation
		//! \param str String to compare
		bool operator != (const StringSpan& str) const;

		//! \brief Check if the string is greater than the specified string
		//! \param str String to compare
		bool operator > (const StringSpan& str) const;

		//! \brief Check if the string is greater or equal than the specified string
		//! \param str String to compare
		bool operator >= (const StringSpan& str) const;

		//! \brief Check if the string is less than the specified string
		//! \param str String to compare
		bool operator < (const StringSpan& str) const;

		//! \brief Check if the string is less or equal than the specified string
		//! \param str String to compare
		bool operator <= (const StringSpan& str) const;

		//! \brief Get the length of the string
		int GetLength() const;

		//! \brief Get the raw pointer
		//! \return Raw pointer, could not terminate with a null-terminator
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

		//! \brief Check if the string ends with the specified string
		/** \details Examples:
		 * "Hello world".StartsWith("world") = true
		 * "Hello world".StartsWith("worl") = false
		 * "Hello world".StartsWith("World") = false
		 */
		bool EndsWith(const StringSpan& str) const;

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

		//! \brief Find the index of the last instance between the specified characters
		//! \param chs Group of characters, where at least one of them should match
		//! \return Between 0 and GetLength - 1 if found, else -1
		//! \sa IndexOf
		int LastIndexOfAny(const StringSpan& chs) const;

		//! \brief Try to parse the string to the specified value
		//! \param value Parsed value
		//! \param base Base used for parsing; can be 2, 8, 10 or 16
		//! \return True if the value has been parsed with success; otherwise false
		bool TryParse(bool& value) const;

		//! \brief Try to parse the string to the specified value
		//! \param value Parsed value
		//! \param base Base used for parsing; can be 2, 8, 10 or 16
		//! \return True if the value has been parsed with success; otherwise false
		bool TryParse(int& value, int base = 10) const;

		//! \brief Parse the string to the specified value,
		//! \param defaultValue Default value if the parse fails
		//! \param base Base used for parsing; can be 2, 8, 10 or 16
		//! \return The parsed value or the default one
		bool ParseBool(bool defaultValue = false) const;

		//! \brief Parse the string to the specified value,
		//! \param defaultValue Default value if the parse fails
		//! \param base Base used for parsing; can be 2, 8, 10 or 16
		//! \return The parsed value or the default one
		int ParseInt(int defaultValue = 0, int base = 10) const;

		static int Compare(const StringSpan& stra, const StringSpan& strb);

		static int CompareInsensitive(const StringSpan& stra, const StringSpan& strb);
	};
}