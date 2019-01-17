/*
   base64.cpp and base64.h

   base64 encoding and decoding with C++.

   Version: 1.01.00

   Copyright (C) 2004-2017 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
	  claim that you wrote the original source code. If you use this source code
	  in a product, an acknowledgment in the product documentation would be
	  appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
	  misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

#include "pch.h"
#include <XeSDK/XeConvert.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeObjPtr.h>
#include <string>

using namespace Xe;

static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

size_t Xe_Convert_FromBase64GetLength(const StringSpan& string)
{
	int in_len = string.GetLength();
	int i = 0;
	int in_ = 0;
	size_t length = 0;

	const char* encoded_string = string.GetData();
	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		i++;
		in_++;
		if (i == 4)
		{
			length += 3;
			i = 0;
		}
	}

	if (i)
	{
		length += i - 1;
	}

	return length;
}

size_t Convert::FromBase64String(IO::IStream* pStream, const StringSpan& string)
{
	int dstLength = Xe_Convert_FromBase64GetLength(string);
	if (pStream != nullptr)
	{
		// If possible, pre-allocate the destination stream
		pStream->SetLength(pStream->GetLength() + Xe_Convert_FromBase64GetLength(string));

		int in_len = string.GetLength();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];

		ObjPtr<IO::IMemoryStream> inStream = IO::IMemoryStream::From(string.GetData(), string.GetLength(), true);
		dstLength = FromBase64String(pStream, inStream, inStream->GetLength());
	}

	return dstLength;
}

size_t Convert::FromBase64String(IO::IStream* pOutStream, IO::IStream* pInStream, size_t in_len)
{
	size_t outLength = 0;
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	unsigned char ch;
	
	while (in_len-- && pInStream->Read(&ch, 0, 1) == 1 && (ch != '=') && is_base64(ch))
	{
		char_array_4[i++] = ch; in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
			{
				pOutStream->Write(char_array_3, i, 1);
			}

			i = 0;
			outLength += 3;
		}
	}

	if (i)
	{
		for (j = 0; j < i; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

		pOutStream->Write(char_array_3, 0, i - 1);
		outLength += i - 1;
	}

	return outLength;
}

size_t Convert::ToBase64String(IO::IStream* pOutStream, IO::IStream* pInStream, size_t in_len)
{
	ASSERT(pInStream != nullptr);
	ASSERT(pOutStream != nullptr);

	const unsigned char ChEof = '=';
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	size_t prev = pOutStream->GetLength();
	while (in_len-- && pInStream->Read(char_array_3, i++, 1))
	{
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				pOutStream->Write(&base64_chars[char_array_4[i]], 0, 1);
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
			pOutStream->Write(&base64_chars[char_array_4[j]], 0, 1);

		while ((i++ < 3))
			pOutStream->Write(&ChEof, 0, 1);

	}

	return pOutStream->GetLength() - prev;
}