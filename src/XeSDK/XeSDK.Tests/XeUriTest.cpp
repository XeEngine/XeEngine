#include "pch.h"
#include <XeSDK/XeUri.h>
#include "gtest/gtest.h"

struct XeUriAuthorityTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* expect() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		EXPECT_STREQ(expect(), Xe::String(Xe::Uri(param()).GetAuthority()));
	}
};

struct XeUriUserInfoTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* expect() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		EXPECT_STREQ(expect(), Xe::String(Xe::Uri(param()).GetUserInfo()));
	}
};

struct XeUriHostTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* expect() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		EXPECT_STREQ(expect(), Xe::String(Xe::Uri(param()).GetHost()));
	}
};

struct XeUriPortTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, int>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	int expect() { return std::tr1::get<1>(GetParam()); }

	void Assert()
	{
		EXPECT_EQ(expect(), Xe::Uri(param()).GetPort());
	}
};

struct XeUriPathTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* expect() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		EXPECT_STREQ(expect(), Xe::String(Xe::Uri(param()).GetPath()));
	}
};

struct XeUriQueryTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* expect() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		EXPECT_STREQ(expect(), Xe::String(Xe::Uri(param()).GetQuery()));
	}
};

struct XeUriFragmentTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, std::string>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	const char* expect() { return std::tr1::get<1>(GetParam()).c_str(); }

	void Assert()
	{
		EXPECT_STREQ(expect(), Xe::String(Xe::Uri(param()).GetFragment()));
	}
};

struct XeUriIsSchemaValidTest :
	::testing::TestWithParam<std::tr1::tuple<std::string, bool>>
{
	const char* param() { return std::tr1::get<0>(GetParam()).c_str(); }
	bool expect() { return std::tr1::get<1>(GetParam()); }

	void Assert()
	{
		EXPECT_EQ(expect(), Xe::Uri::IsSchemaValid(param()));
	}
};

struct XeUriInvalidUriTest :
	::testing::TestWithParam<std::tr1::tuple<std::string>>
{
	const char* param() { return Xe::String(std::tr1::get<0>(GetParam()).c_str()); }

	void Assert()
	{
		EXPECT_THROW(Xe::Uri(param()).GetHost(), std::invalid_argument);
	}
};

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriAuthorityTest, testing::Values(
		std::make_tuple("scheme://www.example.com", "www.example.com"),
		std::make_tuple("scheme://127.0.0.1/somepath", "127.0.0.1"),
		std::make_tuple("scheme://0:0:0:0:0:0:0:1:12345", "0:0:0:0:0:0:0:1:12345"),
		std::make_tuple("scheme://host:12345/somepath", "host:12345"),
		std::make_tuple("scheme://userinfo@host:12345/somepath", "userinfo@host:12345"),
		std::make_tuple("scheme:somepath", "")
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriUserInfoTest, testing::Values(
		std::make_tuple("scheme://userinfo@host:12345/somepath", "userinfo"),
		std::make_tuple("scheme://www.example.com", ""),
		std::make_tuple("scheme:somepath", ""),
		std::make_tuple("scheme://@host", ""),
		std::make_tuple("scheme://user:password@host", "user"),
		std::make_tuple("scheme:path:notaport", "")
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriHostTest, testing::Values(
		std::make_tuple("scheme://userinfo@host:12345", "host"),
		std::make_tuple("scheme://userinfo@host:12345/somepath", "host"),
		std::make_tuple("scheme://www.example.com", "www.example.com"),
		std::make_tuple("scheme://host:", "host"),
		std::make_tuple("scheme:somepath", ""),
		std::make_tuple("scheme:path:notaport", "")
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriPortTest, testing::Values(
		std::make_tuple("scheme://userinfo@host:12345", 12345),
		std::make_tuple("scheme://userinfo@host:12345/somepath", 12345),
		std::make_tuple("scheme://www.example.com", -1),
		std::make_tuple("scheme://host:", -1),
		std::make_tuple("scheme:somepath", -1),
		std::make_tuple("scheme:path:notaport", -1)
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriPathTest, testing::Values(
		std::make_tuple("scheme://host/path", "/path"),
		std::make_tuple("scheme://host/a/quite/deep/path", "/a/quite/deep/path"),
		std::make_tuple("scheme:///nohostdir/file", "/nohostdir/file"),
		std::make_tuple("scheme://www.example.com", "/"),
		std::make_tuple("scheme://host/path#fragment?query", "/path"),
		std::make_tuple("scheme:somepath", "somepath"),
		std::make_tuple("scheme:path:notaport", "path:notaport"),
		std::make_tuple("scheme:", "")
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriQueryTest, testing::Values(
		std::make_tuple("scheme://host/path", ""),
		std::make_tuple("scheme://host/path?query=1&success=true", "?query=1&success=true"),
		std::make_tuple("scheme:path?query=1&success=true", "?query=1&success=true"),
		std::make_tuple("scheme://host/path?query#ignoreme", "?query"),
		std::make_tuple("scheme://host/path#ignoreme?ignoremetoo", "")
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriFragmentTest, testing::Values(
		std::make_tuple("scheme://host/path", ""),
		std::make_tuple("scheme://host/path#fragment", "#fragment"),
		std::make_tuple("scheme:path#fragment", "#fragment"),
		std::make_tuple("scheme://host/path#fragment?partofthefragment", "#fragment?partofthefragment"),
		std::make_tuple("scheme:path?ignoreme#fragment", "#fragment")
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriIsSchemaValidTest, testing::Values(
		std::make_tuple("abc", true),
		std::make_tuple("_bc", false),
		std::make_tuple("sch*me", false)
	));

INSTANTIATE_TEST_CASE_P(
	UriTest, XeUriInvalidUriTest, testing::Values(
		std::make_tuple("5cheme:something"),
		std::make_tuple("sch*me:something"),
		std::make_tuple("noscheme"),
		std::make_tuple("scheme://host:invalidport"),
		std::make_tuple("scheme://host:65536"),
		std::make_tuple("scheme://host:-1")
	));

TEST_P(XeUriAuthorityTest, UriAuthorityTest) { Assert(); }

TEST_P(XeUriUserInfoTest, UriUserInfoTest) { Assert(); }

TEST_P(XeUriHostTest, UriHostTest) { Assert(); }

TEST_P(XeUriPortTest, UriPortTest) { Assert(); }

TEST_P(XeUriPathTest, UriPathTest) { Assert(); }

TEST_P(XeUriQueryTest, UriQueryTest) { Assert(); }

TEST_P(XeUriFragmentTest, UriFragmentTest) { Assert(); }

TEST_P(XeUriIsSchemaValidTest, UriIsSchemaValidTest) { Assert(); }

TEST_P(XeUriInvalidUriTest, UriInvalidUriTest) { Assert(); }