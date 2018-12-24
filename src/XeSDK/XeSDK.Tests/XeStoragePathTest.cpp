#include "pch.h"
#include <XeSDK/XeStoragePath.h>
#include "gtest/gtest.h"

#define STRSPANASSERT(strExpected, str) \
	ASSERT_STREQ(strExpected, std::string(String(str)).c_str())

using namespace Xe;
using namespace Xe::Storage;

struct XeStoragePathTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* expected() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* param() { return std::tr1::get<1>(GetParam()).c_str(); }
};

struct XeStoragePathGetDirectoryNameTest : XeStoragePathTest
{
	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::GetDirectoryName(param())));
	}
};

struct XeStoragePathGetExtensionTest : XeStoragePathTest
{
	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::GetExtension(param())));
	}
};

struct XeStoragePathGetFileNameTest : XeStoragePathTest
{
	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::GetFileName(param())));
	}
};

struct XeStoragePathGetFileNameWithoutExtensionTest : XeStoragePathTest
{
	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::GetFileNameWithoutExtension(param())));
	}
};

struct XeStoragePathGetPathRootTest : XeStoragePathTest
{
	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::GetPathRoot(param())));
	}
};

struct XeStoragePathCombineTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string, std::string>>
{
	const char* expected() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* param() { return std::tr1::get<1>(GetParam()).c_str(); }
	const char* param2() { return std::tr1::get<2>(GetParam()).c_str(); }

	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::Combine(param(), param2())));
	}
};

struct XeStoragePathChangeExtensionTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string, std::string>>
{
	const char* expected() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* param() { return std::tr1::get<1>(GetParam()).c_str(); }
	const char* param2() { return std::tr1::get<2>(GetParam()).c_str(); }

	void Assert()
	{
		ASSERT_STREQ(expected(), String(Path::ChangeExtension(param(), param2())));
	}
};

INSTANTIATE_TEST_CASE_P(
	GetDirectoryNameTestParams, XeStoragePathGetDirectoryNameTest, testing::Values(
		std::make_tuple("C:\\MyDir\\MySubDir", "C:\\MyDir\\MySubDir\\myfile.ext"),
		std::make_tuple("C:\\MyDir\\MySubDir", "C:\\MyDir\\MySubDir\\"),
		std::make_tuple("C:\\MyDir", "C:\\MyDir\\MySubDir"),
		std::make_tuple("C:", "C:\\MyDir"),
		std::make_tuple("C:", "C:\\"),
		std::make_tuple("", "C:"),
		std::make_tuple("/c/test", "/c/test/"),
		std::make_tuple("/c", "/c/test"),
		std::make_tuple("/c", "/c/"),
		std::make_tuple("/", "/c"),
		std::make_tuple("", "/"),
		std::make_tuple("", "")
	));

INSTANTIATE_TEST_CASE_P(
	GetExtensionTestParams, XeStoragePathGetExtensionTest, testing::Values(
		std::make_tuple(".ext", "C:\\MyDir\\MySubDir\\myfile.ext"),
		std::make_tuple("", "C:\\MyDir.old\\"),
		std::make_tuple(".old", ".old"),
		std::make_tuple("", ".old/"),
		std::make_tuple("", "")
	));

INSTANTIATE_TEST_CASE_P(
	GetFileNameTestParams, XeStoragePathGetFileNameTest, testing::Values(
		std::make_tuple("file.txt", "/c/file.txt"),
		std::make_tuple("file", "file"),
		std::make_tuple("", "/c/path.txt/"),
		std::make_tuple(".file", ".file"),
		std::make_tuple("", ".path/"),
		std::make_tuple("", "")
	));

INSTANTIATE_TEST_CASE_P(
	GetFileNameWithoutExtensionTestParams, XeStoragePathGetFileNameWithoutExtensionTest, testing::Values(
		std::make_tuple("file", "/c/file.txt"),
		std::make_tuple("", "/c/path.txt/"),
		std::make_tuple("", ".file"),
		std::make_tuple("", ".path/"),
		std::make_tuple("", "")
	));

INSTANTIATE_TEST_CASE_P(
	GetPathRootTestParams, XeStoragePathGetPathRootTest, testing::Values(
		std::make_tuple("C:\\", "C:\\MyDir\\MyFile.txt"),
		std::make_tuple("", "MyDir\\MyFile.txt"),
		std::make_tuple("/", "/mydir/myfile.txt"),
		std::make_tuple("", "mydir/myfile.txt"),
		std::make_tuple("\\", "\\mydir"),
		std::make_tuple("\\\\ServerPath\\MyDir", "\\\\ServerPath\\MyDir\\MyDir2\\MyFile.txt"),
		std::make_tuple("\\\\?\\UNC\\server\\sharename", "\\\\?\\UNC\\server\\sharename\\folder\\file.txt"),
		std::make_tuple("\\\\.\\device\\", "\\\\.\\device\\folder\\file.txt"),
		std::make_tuple("\\\\.\\", "\\\\.\\"),
		std::make_tuple("\\\\.", "\\\\."),
		std::make_tuple("\\\\", "\\\\"),
		std::make_tuple("C:", "C:...\\asd"),
		std::make_tuple("C:", "C: \\asd"),
		std::make_tuple("C:", "C:"),
		std::make_tuple("", "XY:\\"),
		std::make_tuple("/", "/"),
		std::make_tuple("\\", "\\"),
		std::make_tuple("C:", "C:dasdas"),
		std::make_tuple("", "protocol://host"),
		std::make_tuple("", "file://host/file.txt"),
		std::make_tuple("", "")
	));

INSTANTIATE_TEST_CASE_P(
	CombineTestParams, XeStoragePathCombineTest, testing::Values(
		std::make_tuple("a", "a", ""),
		std::make_tuple("b", "", "b"),
		std::make_tuple("a/b", "a", "b"),
		std::make_tuple("/a/b", "/a", "b"),
		std::make_tuple("/a/b", "/a/", "b"),
		std::make_tuple("/a/b", "/a/", "b"),
		std::make_tuple("/a/b/", "/a/", "b/"),
		std::make_tuple("/a/b\\", "/a/", "b\\"),
		std::make_tuple("/a\\b\\", "/a\\", "b\\"),
		std::make_tuple("/a\\b/", "/a\\", "b/"),
		std::make_tuple("/a/b", "/a", "/b")
	));

INSTANTIATE_TEST_CASE_P(
	ChangeExtensionTestParams, XeStoragePathChangeExtensionTest, testing::Values(
		std::make_tuple("MyFile.bin", "MyFile.txt", ".bin"),
		std::make_tuple("MyFile.bin", "MyFile.txt", "bin"),
		std::make_tuple("MyFile.txt.bin.gz", "MyFile.txt.gz", ".bin.gz"),
		std::make_tuple("MyFile.bin", "MyFile", ".bin"),
		std::make_tuple("MyFile.txt/.bin", "MyFile.txt/", ".bin"),
		std::make_tuple("MyFile.", "MyFile.txt", ""),
		std::make_tuple("MyFile.", "MyFile", ""),
		std::make_tuple("MyFile.", "MyFile.txt", "."),
		std::make_tuple("MyFile.", "MyFile", ".")
	));

TEST_P(XeStoragePathGetDirectoryNameTest, GetDirectoryNameTest) { Assert(); }

TEST_P(XeStoragePathGetExtensionTest, GetExtensionTest) { Assert(); }

TEST_P(XeStoragePathGetFileNameTest, GetFileNameTest) { Assert(); }

TEST_P(XeStoragePathGetFileNameWithoutExtensionTest, GetFileNameWithoutExtensionTest) { Assert(); }

TEST_P(XeStoragePathGetPathRootTest, GetPathRootTest) { Assert(); }

TEST_P(XeStoragePathCombineTest, CombineTest) { Assert(); }

TEST_P(XeStoragePathChangeExtensionTest, ChangeExtensionTest) { Assert(); }
