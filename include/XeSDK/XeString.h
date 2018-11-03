#pragma once
#include <XeSDK/IObject.h>
#include <string>
#include <XeSDK/XeMemory.h>

#undef _INTSIZEOF
#define _INTSIZEOF(n)           ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define GET_ARGS_ADDRESS(ap, v) ((void)(ap = (u8*)&v + _INTSIZEOF(v)))

namespace Xe
{
	class String : public IObject
	{
		tchar* m_str;
		svar m_length;
		svar m_capacity;

		void SetCapacity(svar);
		void SetCapacityZero(svar);
	public:
		enum CompareOptions
		{
			Compare_None = 0,
			Compare_IgnoreCase = 1,
			Compare_IgnoreLength = 2,
		};

		String();

		String(const String& string);

		//! \brief create an empty string with a specified initial capacity
		//! \param[in] capacity of string
		/** \details this constructor helps when an user knows about the
		* maximum length of the string, avoiding continuous resizes from
		* appending characters; if capacity will be overflowed, it will be
		* resized as usual.
		*/
		String(svar capacity);

		//! \brief create a string from a sequence of ASCII characters
		//! \param[in] string to copy
		//! \param[in] length of the input string; if < 0, the string terminator will be checked.
		String(const char* string, svar length = -1);

		//! \brief create a string from a sequence of UTF-16 little endian characters
		//! \param[in] string to copy
		//! \param[in] length of the input string; if < 0, the string terminator will be checked.
		String(const wchar_t* string, svar length = -1);

		//! \brief create a string from another string
		//! \param[in] string to copy
		String(const String& string, svar index, svar length);

		//! \brief deallocate the string
		~String();

		//! \brief get length of the string
		//! \return number of character minus '\0'
		svar GetLength() const;

		//! \brief get character's array pointer
		//! \return pointer
		ctstring GetData() const;

		//! \brief get the string as lowercase
		//! \return this
		String GetLower() const;

		//! \brief get the string as uppercase
		//! \return this
		String GetUpper() const;

		//! \brief convert uppercase letters to lowercase
		//! \return this
		const String& ToLower();

		//! \brief convert lowercase letters to uppercase
		//! \return this
		const String& ToUpper();

		//! \brief compare the current string with another one
		//! \param[in] str the string that will be compared
		//! \param[in] length of the input string; if < 0, the string terminator will be checked.
		//! \param[in] pos position of the first character to compare
		//! \param[in] options of the comparison
		//! \return < 0 if the current string is smaller, > 0 if it's greater, == 0 if it's equal
		svar Compare(ctstring str, svar length = -1, svar pos = 0, CompareOptions options = Compare_None) const;

		//! \brief compare the current string with another one
		//! \param[in] str the string that will be compared
		//! \param[in] length number of characters to compare from str
		//! \param[in] pos initial character's position of str
		//! \param[in] options of the comparison
		//! \return < 0 if the current string is smaller, > 0 if it's greater, == 0 if it's equal
		svar Compare(const String& str, svar length = -1, svar pos = 0, CompareOptions options = Compare_None) const;

		static svar CompareInsensitive(ctstring stra, ctstring strb);

		//! \brief insert at the end of current string, another specified string
		//! \param[in] str the string that will be concatenated
		//! \param[in] pos initial position of source string
		//! \param[in] length of characters to copy
		void Append(ctstring str, svar pos = 0, svar length = -1);

		//! \brief insert at the end of current string, another specified string
		//! \param[in] str the string that will be concatenated
		//! \param[in] pos initial position of source string
		//! \param[in] length of characters to copy
		void Append(const String& str, svar pos = 0, svar length = -1);

		//! \brief copy the content of another string into current string
		//! \param[in] string to copy
		//! \param[in] length of the input string without null terminator; if it's < 0 or > string.GetLength, the length is automatically calculated
		void Copy(ctstring string, svar length = -1);

		//! \brief copy the content of another string into current string
		//! \param[in] string to copy
		//! \param[in] length of the input string without null terminator; if it's < 0, the length is automatically calculated
		void Copy(const String& string, svar length = -1);

		//! \brief get only part of current string
		//! \param[in] start first character to take
		String Substring(svar start) const;

		//! \brief get only part of current string
		//! \param[in] start first character to take
		//! \param[in] length number of characters to extract
		String Substring(svar start, svar length) const;

		//////////////////////////////////////////////////////////////////////////
		// Operators

		//! \brief conversion into ctstring
		operator ctstring() const;

		//! \brief array access
		tchar& operator [](svar index);

		//! \brief assign a string
		//! \details same as Copy(str)
		const String& operator = (const char* str);

		//! \brief assign a string
		//! \details same as Copy(str)
		const String& operator = (const wchar_t* str);

		//! \brief assign a string
		//! \details same as Copy(str)
		const String& operator = (const String& str);

		//! \brief compare two strings
		//! \details same as Compare(str) == 0
		bool operator == (ctstring str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) == 0
		bool operator == (const String& str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) != 0
		bool operator != (ctstring str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) != 0
		bool operator != (const String& str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) > 0
		bool operator > (ctstring str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) > 0
		bool operator > (const String& str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) < 0
		bool operator < (ctstring str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) < 0
		bool operator < (const String& str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) >= 0
		bool operator >= (ctstring str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) >= 0
		bool operator >= (const String& str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) <= 0
		bool operator <= (ctstring str) const;

		//! \brief compare two strings
		//! \details same as Compare(str) <= 0
		bool operator <= (const String& str) const;

		//! \brief concatenate the current string with a second one
		//! \details same as Append(str, 0, -1)
		String operator + (const char* str) const;

		//! \brief concatenate the current string with a second one
		//! \details same as Append(str, 0, -1)
		String operator + (const wchar_t* str) const;

		//! \brief concatenate the current string with a second one
		//! \details same as Append(str, 0, -1)
		String operator + (const String& str) const;

		//! \brief concatenate the current string with a second one
		//! \details same as Append(str, 0, -1)
		const String& operator += (const char* str);

		//! \brief concatenate the current string with a second one
		//! \details same as Append(str, 0, -1)
		const String& operator += (const wchar_t* str);

		//! \brief concatenate the current string with a second one
		//! \details same as Append(str, 0, -1)
		const String& operator += (const String& str);


		//! \brief check if the character is a space or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsSpace(char c);
		//! \brief check if the character is a space or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsSpace(wchar_t c);

		//! \brief check if the character is alphabetic or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsAlpha(char c);
		//! \brief check if the character is alphabetic or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsAlpha(wchar_t c);

		//! \brief check if the character is a number or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsDigit(char c);
		//! \brief check if the character is a number or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsDigit(wchar_t c);

		//! \brief check if the character is alphanumeric or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsAlnum(char c);
		//! \brief check if the character is alphanumeric or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsAlnum(wchar_t c);

		//! \brief check if the character is an hexadecimal value or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsXDigit(char c);
		//! \brief check if the character is an hexadecimal value or not
		//! \param[in] c character to analyze
		//! \return true if the condition is verified
		static bool IsXDigit(wchar_t c);

		//! \brief check if character is a lower character
		//! \param[in] c character to analyze
		//! \return true if condition is verified
		static bool IsLower(char c);
		//! \brief check if character is a lower character
		//! \param[in] c character to analyze
		//! \return true if condition is verified
		static bool IsLower(wchar_t c);

		//! \brief check if character is an upper character
		//! \param[in] c character to analyze
		//! \return true if condition is verified
		static bool IsUpper(char c);
		//! \brief check if character is an upper character
		//! \param[in] c character to analyze
		//! \return true if condition is verified
		static bool IsUpper(wchar_t c);

		//! \brief convert a character to a lower character
		//! \param[in] c character to convert
		//! \return character as lower
		static char ToLower(char c);
		//! \brief convert a character to a lower character
		//! \param[in] c character to convert
		//! \return character as lower
		static wchar_t ToLower(wchar_t c);
		//! \brief convert a string to a lower string
		//! \param[in] str string to convert
		//! \param[in] length of the string
		//! \return str pointer
		static char *ToLower(char *str, svar length);
		//! \brief convert a string to a lower string
		//! \param[in] str string to convert
		//! \param[in] length of the string
		//! \return str pointer
		static wchar_t *ToLower(wchar_t *str, svar length);

		//! \brief convert a character to an upper character
		//! \param[in] c character to convert
		//! \return character as upper
		static char ToUpper(char c);
		//! \brief convert a character to an upper character
		//! \param[in] c character to convert
		//! \return character as upper
		static wchar_t ToUpper(wchar_t c);
		//! \brief convert a string to an upper string
		//! \param[in] str string to convert
		//! \param[in] length of the string
		//! \return str pointer
		static char *ToUpper(char *str, svar length);
		//! \brief convert a string to an upper string
		//! \param[in] str string to convert
		//! \param[in] length of the string
		//! \return str pointer
		static wchar_t *ToUpper(wchar_t *str, svar length);

		//! \brief convert a wide character to a single-byte character
		//! \param[in] ch wide character to convert
		//! \return character in single-byte format; -1 if it's impossible to convert it
		static int WideToChar(wchar_t ch);

		//! \brief convert a sequence of numbers into a integer value
		//! \param[in] str the input string; if it's NULL, the return value is 0
		//! \param[in] length the length of the string; if it's 0, the length is automatic
		//! \param[in] base the base (2, 8, 10, 16); if the base is 0, the base is automatic
		//! \return returns the converted value
		/** \details wen the base is automatic, some restrictions are applied:
		* when the string starts with 0b, the value is binary
		* when the string starts with 0x, the value is hexadecimal
		* when the string starts with 0 or 00, the value is octal
		* in all other cases, the value is decimal
		*/
		static svar Atoi(const char *str, svar length, svar base);
		//! \brief convert a sequence of numbers into a integer value
		//! \param[in] str the input string; if it's NULL, the return value is 0
		//! \param[in] length the length of the string; if it's 0, the length is automatic
		//! \param[in] base the base (2, 8, 10, 16); if the base is 0, the base is automatic
		//! \return returns the converted value
		/** \details wen the base is automatic, some restrictions are applied:
		* when the string starts with 0b, the value is binary
		* when the string starts with 0x, the value is hexadecimal
		* when the string starts with 0 or 00, the value is octal
		* in all other cases, the value is decimal
		*/
		static svar Atoi(const wchar_t *str, svar length, svar base);

		//! \brief convert an integer value to a sequence of numbers
		//! \param[in] value the input value
		//! \param[in] str the output string; if the output value is NULL, it will not processed
		//! \param[in] base the output base number; if it's 0 the base is set to 10
		//! \returns how character has been written
		/** \details the base supported are 2, 8, 10 and 16; you can set as output string
		* a NULL value to get only how characters will be used during the conversion;
		* it doesn't add the '\0' to the end of the string to help the concatenation,
		* but you can implement it doing str[Itoa(value, str, 10)] = '\0'.
		*/
		static svar Itoa(svar value, char *str, svar base);
		//! \brief convert an integer value to a sequence of numbers
		//! \param[in] value the input value
		//! \param[in] str the output string; if the output value is NULL, it will not processed
		//! \param[in] base the output base number; if it's 0 the base is set to 10
		//! \returns how character has been written
		/** \details the base supported are 2, 8, 10 and 16; you can set as output string
		* a NULL value to get only how characters will be used during the conversion;
		* it doesn't add the '\0' to the end of the string to help the concatenation,
		* but you can implement it doing str[Itoa(value, str, 10)] = '\0'.
		*/
		static svar Itoa(svar value, wchar_t *str, svar base);

		//! \brief locate the first occurrence of a character in a string
		//! \param[in] string to check
		//! \param[in] character to find
		//! \return position of first specified character found or nullptr
		/** \details scrolls given string from left to right and stops when
		 * when specified character is found, returning current pointer. If
		 * the character is not found, input string is nullptr or character
		 * to find is a null-terminating character, nullptr is returned.
		 */
		static const char *FindCharacter(const char *string, char character);
		//! \brief locate the first occurrence of a character in a string
		//! \param[in] string to check
		//! \param[in] character to find
		//! \return position of first specified character found or nullptr
		/** \details scrolls given string from left to right and stops when
		* when specified character is found, returning current pointer. If
		* the character is not found, input string is nullptr or character
		* to find is a null-terminating character, nullptr is returned.
		*/
		static const wchar_t *FindCharacter(const wchar_t *string, wchar_t character);

		//! \brief locate the last occurrence of a character in a string
		//! \param[in] string to check
		//! \param[in] character to find
		//! \return position of last specified character found or nullptr
		/** \details scrolls given string from left to right and stops when
		* a null-terminating character is found. If  the character is not
		* found, input string is nullptr or character to find is a
		* null-terminating character, nullptr is returned.
		*/
		static const char *FindLastCharacter(const char *string, char character);
		//! \brief locate the last occurrence of a character in a string
		//! \param[in] string to check
		//! \param[in] character to find
		//! \return position of last specified character found or nullptr
		/** \details scrolls given string from left to right and stops when
		 * a null-terminating character is found. If  the character is not
		 * found, input string is nullptr or character to find is a
		 * null-terminating character, nullptr is returned.
		 */
		static const wchar_t *FindLastCharacter(const wchar_t *string, wchar_t character);

		//! \brief get the length of an ASCII string
		//! \param string the string to calculate its length
		//! \return position of first '\0' character found
		/** \details the length doesn't include the end-string character;
		* it returns -1 if a nullptr string is passed
		*/
		//! \warning the function will be end when '\0' is found
		static svar GetLength(const char* string);

		//! \brief get the length of an UNICODE string
		//! \param string the string to calculate its length
		//! \return position of first '\0' character found
		/** \details the length doesn't include the end-string character;
		* it returns -1 if a nullptr string is passed
		*/
		//! \warning the function will be end when '\0' is found
		static svar GetLength(const wchar_t* string);

		//! \brief get the length of an ASCII string
		//! \param[in] string the string to calculate its length
		//! \param[in] length the maximum length of the string
		//! \return position of first '\0' character found or length param itself if it's not found
		/** \details the length doesn't include the end-string character; it accepts null pointers
		* it returns -1 if a nullptr string is passed
		*/
		static svar GetNLength(const char* string, svar length);

		//! \brief get the length of an UNICODE string
		//! \param[in] string the string to calculate its length
		//! \param[in] length the maximum length of the string
		//! \return position of first '\0' character found or length param itself if it's not found
		/** \details the length doesn't include the end-string character; it accepts null pointers
		* it returns -1 if a nullptr string is passed
		*/
		static svar GetNLength(const wchar_t* string, svar length);

		//! \brief copy an ASCII string into another
		//! \param[in] dst pointer to the destination array where the content is to be copied
		//! \param[in] src string to be copied
		//! \param[in] length of dst buffer length
		//! \return how characters was copied with success
		/** \details copy each character of source's array until the null character,
		* including it; if dst is nullptr or length is <= 0, the return value will
		* be 0 and nothing will be modified, if src is nullptr or length is less
		* than GetLength(src), the first character of dst will be set to 0 and
		* the return value will be 0
		*/
		static svar Copy(char* dst, const char* src, svar length);

		//! \brief copy an UNICODE string into another
		//! \param[in] dst pointer to the destination array where the content is to be copied
		//! \param[in] src string to be copied
		//! \param[in] length of dst buffer length
		//! \return how characters was copied with success
		/** \details copy each character of source's array until the null character,
		* including it; if dst is nullptr or length is <= 0, the return value will
		* be 0 and nothing will be modified, if src is nullptr or length is less
		* than GetLength(src), the first character of dst will be set to 0 and
		* the return value will be 0
		*/
		static svar Copy(wchar_t* dst, const wchar_t* src, svar length);

		//! \brief copy an ASCII string into an UNICODE string
		//! \param[in] dst pointer to the destination array where the content is to be copied
		//! \param[in] src string to be copied
		//! \param[in] length of dst buffer length
		//! \return how characters was copied with success
		/** \details copy each character of source's array until the null character,
		* including it; if dst is nullptr or length is <= 0, the return value will
		* be 0 and nothing will be modified, if src is nullptr or length is less
		* than GetLength(src), the first character of dst will be set to 0 and
		* the return value will be 0
		*/
		static svar Copy(wchar_t* dst, const char* src, svar length);

		//! \brief copy an UNICODE string into an ASCII string
		//! \param[in] dst pointer to the destination array where the content is to be copied
		//! \param[in] src string to be copied
		//! \param[in] length of dst buffer length
		//! \return how characters was copied with success
		/** \details copy each character of source's array until the null character,
		* including it; if dst is nullptr or length is <= 0, the return value will
		* be 0 and nothing will be modified, if src is nullptr or length is less
		* than GetLength(src), the first character of dst will be set to 0 and
		* the return value will be 0
		*/
		static svar Copy(char* dst, const wchar_t* src, svar length);

		//! \brief copy the first N character of a string into another
		//! \param[in] dst pointer to the destination array where the content is to be copied
		//! \param[in] src string to be copied
		//! \param[in] length number of maximum characters to be copied
		//! \return how characters was copied with success
		/** \details copy each character of source's array until the null character or
		* until the limit, including it; the null character will be copied only if n
		* is higher than the string length of the source string
		**/
		//! \warning Be sure to not pass an invalid/not initialized pointers
		static svar NCopy(char* dst, const char* src, svar length);

		//! \brief copy the first N character of a string into another
		//! \param[in] dst pointer to the destination array where the content is to be copied
		//! \param[in] src string to be copied
		//! \param[in] length number of maximum characters to be copied
		//! \return how characters was copied with success
		/** \details copy each character of source's array until the null character or
		* until the limit, including it; the null character will be copied only if n
		* is higher than the string length of the source string
		**/
		//! \warning Be sure to not pass an invalid/not initialized pointers
		static svar NCopy(wchar_t* dst, const wchar_t* src, svar length);

		//! \brief get the hash from a string
		//! \param[in] str string of characters or data to process
		//! \param[in] length of elements to process
		//! \return 64-bit hash of given input
		/** \details this hash function is based from a 64-bit version of
		* FNV-1a (Fowler–Noll–Vo). This algorithm has the advantage of
		* compute hashes with few computation power but it doesn't fit
		* on cryptographics. This can be used to calculate checksum or
		* hash tables.
		*/
		static u64 GetHashFNV1a(const char *str, svar length);
		//! \brief get the hash from a wide-charater string
		//! \param[in] str string of characters or 16-bit data to process
		//! \param[in] length of elements to process
		//! \return 64-bit hash of given input
		/** \details this hash function is based from a 64-bit version of
		* FNV-1a (Fowler–Noll–Vo). This algorithm has the advantage of
		* compute hashes with few computation power but it doesn't fit
		* on cryptographics. This can be used to calculate checksum or
		* hash tables.
		*/
		static u64 GetHashFNV1a(const wchar_t *str, svar length);

		//! \brief print formatted text to a string
		//! \param[in] dst pointer to the destination array where the content will be modified
		//! \param[in] length maximum length of dst array to avoid overflow
		//! \param[in] format to write on a string; can have arguments
		//! \param[in] ... arguments
		//! \return written characters
		/** \details the current method is based from *printf ANSI C function;
		* if format specifies symbol '%', an argument is captured to be written
		* %c: print a single character
		* %s: print a sequence of characters until '\0' terminator
		* %S: print an IObject that implements ISerializable
		* %i: print a signed integer number
		* %d: print a signed integer number
		* %b: print a signed integer binary number
		* %o: print a signed integer octal number
		* %x: print a signed integer hexadecimal number
		* %X: print a signed integer hexadecimal number
		* %p: print a pointer in hexadecimal digits
		*/
		static svar PrintFormatted(tchar *dst, svar length, ctstring format, ...);

		static svar PrintArguments(char *dst, uvar length, const char *format, ...);
		static svar PrintArguments(char *dst, uvar length, const char *format, void *args);
		static svar PrintArguments(wchar_t *dst, uvar length, const wchar_t *format, ...);
		static svar PrintArguments(wchar_t *dst, uvar length, const wchar_t *format, void *args);
		static String PrintArguments(const char *format, ...);
		static String PrintArguments(const char *format, void *args);
		static String PrintArguments(const wchar_t *format, ...);
		static String PrintArguments(const wchar_t *format, void *args);

		template <typename T>
		static u64 GetHashFNV1a(const T *str, svar length)
		{
			static const u64 OffsetBasis = 14695981039346656037ULL;
			static const u64 Prime = 1099511628213ULL;

			u64 hash = OffsetBasis;
			for (svar i = 0; i < length; i++)
			{
				hash ^= str[i];
				hash *= Prime;
			}
			return hash;
		}
	};
}