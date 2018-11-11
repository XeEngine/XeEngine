#pragma once

#define ASSERT_POSITIVE(x) \
	if (x < 0) { throw std::invalid_argument("\""#x"\" cannot be a negative number"); }

#define ASSERT_CPOSITION(x, l) { \
	ASSERT_POSITIVE(x); \
	if (x >= l) { throw std::invalid_argument("\""#x"\" is greater than the length of the string"); } }

#define ASSERT_CPOSITION_ALT(x, l) { \
	ASSERT_POSITIVE(x); \
	if (x > l) { throw std::invalid_argument("\""#x"\" is greater than the length of the string"); } }

#define ASSERT_POSITION(x) ASSERT_CPOSITION(x, m_Length)

namespace Xe
{
	extern const char String_Digits_Lowercase[];
	extern const char String_Digits_Uppercase[];
	extern const char String_Digits_Uppercase_Negative[];

	template <typename T>
	Xe::String FromSigned(T value, int base)
	{
		// If the value is 0, returns 0 instead of an empty string
		if (value == 0)
		{
			return Xe::String("0");
		}

		char str[64]; // Max chars possible for a ulong with base 2
		int len = 0;

		// Only base 10 has the minus character
		bool isNegative = false;
		if (base == 10 && value < 0)
		{
			str[len++] = '-';
			isNegative = true;
		}

		do
		{
			// Hacky way for parse negative numbers
			str[len++] = String_Digits_Uppercase_Negative[15 + (value % base)];
			value /= base;
		} while (value != 0);

		// Reverse the string
		if (isNegative)
		{
			for (int i = 1; i < (len + 1) / 2; i++)
			{
				char tmp = str[i];
				str[i] = str[len - i];
				str[len - i] = tmp;
			}
		}
		else
		{
			for (int i = 0; i < len / 2; i++)
			{
				char tmp = str[i];
				str[i] = str[len - i - 1];
				str[len - i - 1] = tmp;
			}
		}

		return Xe::String(Xe::StringSpan(str, len));
	}

	template <typename T>
	Xe::String FromUnsigned(T value, int base)
	{
		// If the value is 0, returns 0 instead of an empty string
		if (value == 0)
		{
			return Xe::String("0");
		}

		char str[64]; // Max chars possible for a ulong with base 2
		int len = 0;

		do
		{
			// Hacky way for parse negative numbers
			str[len++] = String_Digits_Uppercase[value % base];
			value /= base;
		} while (value != 0);

		// Reverse the string
		for (int i = 0; i < len / 2; i++)
		{
			char tmp = str[i];
			str[i] = str[len - i - 1];
			str[len - i - 1] = tmp;
		}

		return Xe::String(Xe::StringSpan(str, len));
	}
}