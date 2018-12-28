#include "pch.h"
#include <XeSDK/XeConvert.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/XeMemory.h>
#include "gtest/gtest.h"

struct XeConvertFromBase64StringTest :
	::testing::TestWithParam<std::tr1::tuple<std::vector<u8>, std::string>>
{
	const std::vector<u8>& expected() { return std::tr1::get<0>(GetParam()); }
	const char* param() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		Xe::ObjPtr<Xe::IO::IMemoryStream> mem = Xe::IO::IMemoryStream::New(0, true);
		int length = Xe::Convert::FromBase64String(mem, param());

		const auto& exp = expected();
		ASSERT_EQ(exp.size(), length);
		ASSERT_EQ(exp.size(), mem->GetLength());

		for (int i = 0; i < exp.size(); i++)
		{
			ASSERT_EQ(exp[i], ((u8*)mem->GetMemory())[i]);
		}
	}
};

struct XeConvertFromToBase64StringTest :
	::testing::TestWithParam<std::tr1::tuple<std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }

	void Assert()
	{
		Xe::ObjPtr<Xe::IO::IMemoryStream> memDec = Xe::IO::IMemoryStream::New(0, true);
		Xe::ObjPtr<Xe::IO::IMemoryStream> memEnc = Xe::IO::IMemoryStream::New(0, true);
		int length = Xe::Convert::FromBase64String(memDec, param());
		int outLength = Xe::Convert::ToBase64String(memEnc, memDec, memDec->GetLength());

		ASSERT_EQ(outLength, memEnc->GetLength());
		Xe::Memory::Compare(param(), 0, memEnc->GetMemory(), 0, outLength);
	}
};

INSTANTIATE_TEST_CASE_P(
	FromBase64String, XeConvertFromBase64StringTest, testing::Values(
		std::make_tuple(std::vector<u8>{ 0x00 }, "AA=="),
		std::make_tuple(std::vector<u8>{ 0x01 }, "AQ=="),
		std::make_tuple(std::vector<u8>{ 0xFF }, "/w=="),
		std::make_tuple(std::vector<u8>{ 0x10, 0x20 }, "ECA="),
		std::make_tuple(std::vector<u8>{ 'h', 'e', 'l', 'l', 'o' }, "aGVsbG8=")
	));

INSTANTIATE_TEST_CASE_P(
	FromToBase64String, XeConvertFromToBase64StringTest, testing::Values(
		std::make_tuple("AA=="),
		std::make_tuple("VGhpcyBpcyBhIHRlc3Qgd2l0aCBhIGxvbmcgYmFzZTY0IHN0cmluZyB0byBkZWNvZGUgZmlyc3QgYW5kIHRoZW4gZW5jb2RlIGl0IGJhY2sh")
	));

TEST_P(XeConvertFromBase64StringTest, FromBase64String) { Assert(); }

TEST_P(XeConvertFromToBase64StringTest, FromToBase64String) { Assert(); }
