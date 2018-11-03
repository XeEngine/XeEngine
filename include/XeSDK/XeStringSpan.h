#pragma once

namespace Xe
{
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

		char operator [](int index);
		StringSpan& operator =(const StringSpan& str);
		bool operator == (const StringSpan& str) const;
		bool operator != (const StringSpan& str) const;
		bool operator > (const StringSpan& str) const;
		bool operator >= (const StringSpan& str) const;
		bool operator < (const StringSpan& str) const;
		bool operator <= (const StringSpan& str) const;

		int GetLength() const;

		const char* GetData() const;
	};
}