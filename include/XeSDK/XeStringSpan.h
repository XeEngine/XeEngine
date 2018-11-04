#pragma once

namespace Xe
{
	//! \brief Holds a memory reference to a string
	class StringSpan
	{
	protected:
		int m_Length;
		const char* m_Data;
	public:
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

		//! \brief Length of the string
		int GetLength() const;

		//! \brief Get the raw pointer
		//! \return Raw pointer, could not terminate with a null-terminator
		const char* GetData() const;

		//! \brief Check if the string is empty
		//! \return True if GetLength is 0
		//! \sa GetLength
		bool IsEmpty() const;

		//! \brief Try to parse the string to the specified value
		//! \param value Parsed value
		//! \param base Base used for parsing; can be 2, 8, 10 or 16
		//! \return True if the value has been parsed with success; otherwise false
		bool TryParse(int& value, int base = 10);

		//! \brief Parse the string to the specified value,
		//! \param defaultValue Default value if the parse fails
		//! \param base Base used for parsing; can be 2, 8, 10 or 16
		//! \return The parsed value or the default one
		int Parse(int defaultValue = 0, int base = 10);
	};
}