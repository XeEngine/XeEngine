#include "pch.h"
#include "gtest/gtest.h"
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeIO.h>
#include <XeSDK/XeStorage.h>

using namespace Xe;
using namespace Xe::IO;

static char *XeIOStreamTest_rand_string(char *str, size_t size)
{
	// https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
	const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz_";
	if (size) {
		--size;
		for (size_t n = 0; n < size; n++) {
			int key = rand() % (int)(sizeof charset - 1);
			str[n] = charset[key];
		}
		str[size] = '\0';
	}
	return str;
}

void XeIOStreamTest_AssertValidPosition(IO::IStream* stream, bool canOverflow)
{
	auto streamPosition = stream->GetPosition();
	ASSERT_EQ(Error::IO_POSITION_INVALID, stream->SetPosition(-1, IO::Seek_Set));
	ASSERT_EQ(Error::IO_POSITION_INVALID, stream->SetPosition(-stream->GetLength() - 1, IO::Seek_End));
	ASSERT_EQ(Error::IO_POSITION_INVALID, stream->SetPosition(-stream->GetLength(), IO::Seek_Cur));
	ASSERT_EQ(Error::IO_SEEK_MODE_INVALID, stream->SetPosition(0, (IO::Seek)0xDEAD));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetPosition(stream->GetLength() + 1, IO::Seek_Set));
	ASSERT_EQ(Error::OK, stream->SetPosition(0, IO::Seek_End));
	ASSERT_EQ(Error::OK, stream->SetPosition(stream->GetLength(), IO::Seek_Cur));
	stream->SetPosition(streamPosition);
}

void XeIOStreamTest_Assert1100(IO::IStream* stream)
{
	auto streamLength = stream->GetLength();
	auto streamPosition = stream->GetPosition();

	ASSERT_TRUE(stream->CanRead());
	ASSERT_TRUE(stream->CanSeek());
	ASSERT_FALSE(stream->CanWrite());
	ASSERT_FALSE(stream->CanOverflow());

	ASSERT_NE(Error::OK, stream->SetLength(streamLength + 1));
	ASSERT_EQ(streamLength, stream->GetLength());

	int ch = 0xDEADB33F;
	ASSERT_EQ(0, stream->Write(&ch, 0, 4));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(1, stream->Read(&ch, 0, 1));
	ASSERT_EQ(streamPosition + 1, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetPosition(-1, IO::Seek_Cur));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(1, stream->Read(&ch, 1, 1));
	ASSERT_EQ((ch & 0xFF), ((ch >> 8) & 0xFF));

	ASSERT_EQ(Error::OK, stream->SetPosition(-stream->GetLength() + streamPosition, IO::Seek_End));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetPosition(-2, IO::Seek_End));
	ASSERT_EQ(2, stream->Read(&ch, 0, 4));
	ASSERT_EQ(0, stream->Read(&ch, 0, 4));

	ASSERT_EQ(Error::OK, stream->SetPosition(streamPosition, IO::Seek_Set));
	ASSERT_EQ(streamPosition, stream->GetPosition());
}

void XeIOStreamTest_Assert1110(IO::IStream* stream)
{
	const int ExpectedValue = 0xDEADB33F;

	int ch;
	auto streamLength = stream->GetLength();
	auto streamPosition = stream->GetPosition();

	ASSERT_TRUE(stream->CanRead());
	ASSERT_TRUE(stream->CanSeek());
	ASSERT_TRUE(stream->CanWrite());
	ASSERT_FALSE(stream->CanOverflow());

	ASSERT_NE(Error::OK, stream->SetLength(streamLength + 1));
	ASSERT_EQ(streamLength, stream->GetLength());

	ASSERT_EQ(4, stream->Write(&ExpectedValue, 0, 4));
	ASSERT_EQ(streamPosition + 4, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetPosition(streamPosition));
	ASSERT_EQ(4, stream->Read(&ch, 0, 4));
	ASSERT_EQ(streamPosition + 4, stream->GetPosition());
	ASSERT_EQ(ExpectedValue, ch);

	ASSERT_EQ(Error::OK, stream->SetPosition(-4, IO::Seek_Cur));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(1, stream->Read(&ch, 1, 1));
	ASSERT_EQ((ch & 0xFF), ((ch >> 8) & 0xFF));

	ASSERT_EQ(Error::OK, stream->SetPosition(-stream->GetLength() + streamPosition, IO::Seek_End));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetPosition(-2, IO::Seek_End));
	ASSERT_EQ(2, stream->Read(&ch, 0, 4));
	ASSERT_EQ(0, stream->Read(&ch, 0, 4));

	ASSERT_EQ(Error::OK, stream->SetPosition(streamPosition, IO::Seek_Set));
	ASSERT_EQ(streamPosition, stream->GetPosition());
}

void XeIOStreamTest_Assert1111(IO::IStream* stream)
{
	const int ExpectedValue = 0xDEADB33F;

	int ch;
	auto streamLength = stream->GetLength();
	auto streamPosition = stream->GetPosition();

	ASSERT_TRUE(stream->CanRead());
	ASSERT_TRUE(stream->CanSeek());
	ASSERT_TRUE(stream->CanWrite());
	ASSERT_TRUE(stream->CanOverflow());

	ASSERT_EQ(Error::OK, stream->SetPosition(streamPosition));
	ASSERT_EQ(4, stream->Write(&ExpectedValue, 0, 4));
	ASSERT_EQ(streamPosition + 4, stream->GetPosition());
	ASSERT_EQ(streamLength, stream->GetLength());
	
	ASSERT_EQ(Error::OK, stream->SetPosition(streamPosition));
	ASSERT_EQ(4, stream->Read(&ch, 0, 4));
	ASSERT_EQ(streamPosition + 4, stream->GetPosition());
	ASSERT_EQ(ExpectedValue, ch);

	ASSERT_EQ(Error::OK, stream->SetPosition(-4, IO::Seek_Cur));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetLength(streamLength + 1));
	ASSERT_EQ(streamLength + 1, stream->GetLength());

	ASSERT_EQ(Error::OK, stream->SetLength(streamLength));
	ASSERT_EQ(streamLength, stream->GetLength());

	ASSERT_EQ(Error::OK, stream->SetPosition(-2, IO::Seek_End));
	ASSERT_EQ(4, stream->Write(&ExpectedValue, 0, 4));
	ASSERT_EQ(streamLength + 2, stream->GetPosition());
	ASSERT_EQ(streamLength + 2, stream->GetLength());
	ASSERT_EQ(Error::OK, stream->SetLength(streamLength));
	ASSERT_EQ(streamLength + 2, stream->GetPosition());
	ASSERT_EQ(streamLength, stream->GetLength());

	ASSERT_EQ(Error::OK, stream->SetPosition(-stream->GetLength() + streamPosition, IO::Seek_End));
	ASSERT_EQ(streamPosition, stream->GetPosition());

	ASSERT_EQ(Error::OK, stream->SetPosition(-2, IO::Seek_End));
	ASSERT_EQ(2, stream->Read(&ch, 0, 4));
	ASSERT_EQ(0, stream->Read(&ch, 0, 4));

	ASSERT_EQ(Error::OK, stream->SetPosition(streamPosition, IO::Seek_Set));
	ASSERT_EQ(streamPosition, stream->GetPosition());
}

TEST(XeIOStreamTest, MemoryStreamReadOnlyTest)
{
	char expected[] = "Test";
	int actual = 0;

	ObjPtr<IMemoryStream> stream = IMemoryStream::From((const char*)expected, lengthof(expected));
	XeIOStreamTest_Assert1100(stream);
	XeIOStreamTest_AssertValidPosition(stream, false);

	ASSERT_EQ(lengthof(expected), stream->GetLength());
	ASSERT_EQ(4, stream->Read(&actual, 0, 4));
	ASSERT_EQ(*(int*)"Test", actual);
}

TEST(XeIOStreamTest, MemoryStreamReadWriteTest)
{
	char unexpected[] = "Test";
	int actual = 0;

	ObjPtr<IMemoryStream> stream = IMemoryStream::From(unexpected, lengthof(unexpected));
	XeIOStreamTest_Assert1110(stream);
	XeIOStreamTest_AssertValidPosition(stream, false);

	ASSERT_EQ(lengthof(unexpected), stream->GetLength());
	ASSERT_EQ(4, stream->Read(&actual, 0, 4));
	ASSERT_NE(*(int*)"Test", actual);
}

TEST(XeIOStreamTest, MemoryStreamReadWriteNotResizableTest)
{
	char unexpected[] = "Test";
	int actual = 0;

	ObjPtr<IMemoryStream> stream = IMemoryStream::New(lengthof(unexpected), false);
	Xe::Memory::Copy(stream->GetData(), unexpected, lengthof(unexpected));
	XeIOStreamTest_Assert1110(stream);
	XeIOStreamTest_AssertValidPosition(stream, false);

	ASSERT_EQ(lengthof(unexpected), stream->GetLength());
	ASSERT_EQ(4, stream->Read(&actual, 0, 4));
	ASSERT_NE(*(int*)"Test", actual);
}

TEST(XeIOStreamTest, MemoryStreamReadWriteOverflowTest)
{
	char unexpected[] = "Test";
	int actual = 0;

	ObjPtr<IMemoryStream> stream = IMemoryStream::New(lengthof(unexpected), true);
	Xe::Memory::Copy(stream->GetData<void>(), unexpected, lengthof(unexpected));
	XeIOStreamTest_Assert1111(stream);
	XeIOStreamTest_AssertValidPosition(stream, true);

	ASSERT_EQ(lengthof(unexpected), stream->GetLength());
	ASSERT_EQ(4, stream->Read(&actual, 0, 4));
	ASSERT_NE(*(int*)"Test", actual);
}

TEST(XeIOStreamTest, MemoryStreamOverflowTest)
{
	char expected[] = "Test";
	int actual = 0;

	ObjPtr<IMemoryStream> stream = IMemoryStream::New(4, true);
	stream->Write(expected, 0, 4);
	
	for (int i = 0; i < 100; i++)
	{
		ASSERT_EQ(Error::OK, stream->SetLength(4 + i * 4096 - (i / 2) * 2048));
	}

	stream->SetPosition(0);
	stream->Read(&actual, 0, 4);
	ASSERT_EQ(*(int*)"Test", actual);
}

TEST(XeIOStreamTest, FileStreamReadOnlyTest)
{
	char expected[] = "Test";
	int actual = 0;
	char strFileNameBuffer[16];
	XeIOStreamTest_rand_string(strFileNameBuffer, lengthof(strFileNameBuffer) - 1);

	Storage::IStorage* pStorage;
	ASSERT_TRUE(Storage::GetStorage(&pStorage));

	IO::IFileStream *pStream;
	ASSERT_EQ(Error::OK, pStorage->FileOpen(&pStream, strFileNameBuffer, Storage::Access_ReadWrite, Storage::Mode_Create));
	ASSERT_EQ(4, pStream->Write(expected, 0, 4));
	pStream->Release();

	ASSERT_EQ(Error::OK, pStorage->FileOpen(&pStream, strFileNameBuffer, Storage::Access_Read, Storage::Mode_OpenIfExists));
	XeIOStreamTest_Assert1100(pStream);
	XeIOStreamTest_AssertValidPosition(pStream, false);

	pStream->SetPosition(0);
	pStream->Read(&actual, 0, 4);
	ASSERT_EQ(*(int*)"Test", actual);

	pStream->Release();
	ASSERT_EQ(Error::OK, pStorage->FileDelete(strFileNameBuffer));
}

TEST(XeIOStreamTest, FileStreamReadWriteOverflowTest)
{
	char unexpected[] = "Test";
	int actual = 0;
	char strFileNameBuffer[16];
	XeIOStreamTest_rand_string(strFileNameBuffer, lengthof(strFileNameBuffer) - 1);

	Storage::IStorage* pStorage;
	ASSERT_TRUE(Storage::GetStorage(&pStorage));

	IO::IFileStream *pStream;
	ASSERT_EQ(Error::OK, pStorage->FileOpen(&pStream, strFileNameBuffer, Storage::Access_ReadWrite, Storage::Mode_Create));
	ASSERT_EQ(4, pStream->Write(unexpected, 0, 4));
	pStream->SetPosition(0);
	
	XeIOStreamTest_Assert1111(pStream);
	XeIOStreamTest_AssertValidPosition(pStream, true);

	pStream->SetPosition(0);
	pStream->Read(&actual, 0, 4);
	ASSERT_NE(*(int*)"Test", actual);

	pStream->Release();
	ASSERT_EQ(Error::OK, pStorage->FileDelete(strFileNameBuffer));
}
