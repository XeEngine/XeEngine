#include "pch.h"
#include <XeSDK/XeCryptoCrc32.h>
#include <XeSDK/XeCryptoCrc64.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Crypto {
	template <typename T>
	void CrcGenerateTable(T(&table)[0x100], T polynomial)
	{
		for (int n = 0; n < 256; n++) {
			T c = (T)n;
			for (int k = 0; k < 8; k++)
				c = c & 1 ? polynomial ^ (c >> 1) : c >> 1;
			table[n] = c;
		}
	}

	template <typename T>
	T CrcCalculate(const T* table, const char* str, int length)
	{
		T value = -1;

		for (int i = 0; i < length; i++)
		{
			unsigned char index = ((unsigned char)value) ^ str[i];
			value = table[index] ^ (value >> 8);
		}
		return value;
	}

	template <typename T>
	T CrcCalculate(const T* table, const char* str, int offset, int length)
	{
		return CrcCalculate(table, str + offset, length);
	}

	Crc32::Crc32(u32 polynomial)
	{
		CrcGenerateTable(Table, polynomial);
	}

	u32 Crc32::Calculate(ctstring str) const
	{
		int strLen = Xe::StringSpan(str).GetLength();
		return Calculate(str, 0, strLen);
	}

	u32 Crc32::Calculate(const char* data, int offset, int length) const
	{
		return CrcCalculate<u32>(Table, data, offset, length);
	}

	Crc64::Crc64(u64 polynomial)
	{
		CrcGenerateTable(Table, polynomial);
	}

	u64 Crc64::Calculate(ctstring str) const
	{
		int strLen = Xe::StringSpan(str).GetLength();
		return Calculate(str, 0, strLen);
	}

	u64 Crc64::Calculate(const char* data, int offset, int length) const
	{
		return CrcCalculate<u64>(Table, data, offset, length);
	}
} }
