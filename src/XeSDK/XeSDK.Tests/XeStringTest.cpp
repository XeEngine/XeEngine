#include "pch.h"
#include <XeSDK/XeString2.h>
#include "gtest/gtest.h"

#define STRASSERT(strExpected, str) \
	ASSERT_STREQ(strExpected, (str)); \
	EXPECT_EQ(strlen(str), (str).GetLength());

void XeStringTest(const char* strExpected)
{
	Xe::String2 str(strExpected);
	EXPECT_EQ(str, str.GetData());
	STRASSERT(strExpected, str);
}

TEST(XeStringTest, BaseTest)
{
	XeStringTest("");
	XeStringTest("test");
}

TEST(XeStringTest, EmptyTest)
{
	STRASSERT("", Xe::String2());
	STRASSERT("", Xe::String2::Empty);
}

TEST(XeStringTest, IsEmptyTest)
{
	EXPECT_TRUE(Xe::String2::Empty.IsEmpty());
	EXPECT_TRUE(Xe::String2("").IsEmpty());
	EXPECT_FALSE(Xe::String2("!").IsEmpty());
}

TEST(XeStringTest, IsEmptyOrWhitespaceTest)
{
	EXPECT_TRUE(Xe::String2().IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String2::Empty.IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String2("").IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String2(" ").IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String2("   ").IsEmptyOrWhitespace());
	EXPECT_FALSE(Xe::String2("!").IsEmptyOrWhitespace());
	EXPECT_FALSE(Xe::String2(" !").IsEmptyOrWhitespace());
	EXPECT_FALSE(Xe::String2("! ").IsEmptyOrWhitespace());
}

TEST(XeStringTest, StartsWithTest)
{
	EXPECT_TRUE(Xe::String2("Hello world!").StartsWith(""));
	EXPECT_TRUE(Xe::String2("Hello world!").StartsWith(Xe::String2("")));
	EXPECT_TRUE(Xe::String2("Hello world!").StartsWith("H"));
	EXPECT_TRUE(Xe::String2("Hello world!").StartsWith(Xe::String2("H")));
	EXPECT_TRUE(Xe::String2("Hello world!").StartsWith("Hello"));
	EXPECT_TRUE(Xe::String2("Hello world!").StartsWith(Xe::String2("Hello")));
	EXPECT_FALSE(Xe::String2("Hello world!").StartsWith("ello"));
	EXPECT_FALSE(Xe::String2("Hello world!").StartsWith(Xe::String2("ello")));
}

TEST(XeStringTest, EndsWithTest)
{
	EXPECT_TRUE(Xe::String2("Hello world!").EndsWith(""));
	EXPECT_TRUE(Xe::String2("Hello world!").EndsWith(Xe::String2("")));
	EXPECT_TRUE(Xe::String2("Hello world!").EndsWith("!"));
	EXPECT_TRUE(Xe::String2("Hello world!").EndsWith(Xe::String2("!")));
	EXPECT_TRUE(Xe::String2("Hello world!").EndsWith("world!"));
	EXPECT_TRUE(Xe::String2("Hello world!").EndsWith(Xe::String2("world!")));
	EXPECT_FALSE(Xe::String2("Hello world!").EndsWith("world"));
	EXPECT_FALSE(Xe::String2("Hello world!").EndsWith(Xe::String2("world")));
}

TEST(XeStringTest, IndexOfTest)
{
	EXPECT_EQ(12, Xe::String2("Hello world!").IndexOf('\0'));
	EXPECT_EQ(0, Xe::String2("Hello world!").IndexOf('H'));
	EXPECT_EQ(2, Xe::String2("Hello world!").IndexOf('l'));
	EXPECT_EQ(11, Xe::String2("Hello world!").IndexOf('!'));
	EXPECT_EQ(-1, Xe::String2("Hello world!").IndexOf('?'));
	EXPECT_EQ(0, Xe::String2("Hello world!").IndexOf(""));
	EXPECT_EQ(0, Xe::String2("Hello world!").IndexOf(Xe::String2("")));
	EXPECT_EQ(0, Xe::String2("Hello world!").IndexOf("Hello"));
	EXPECT_EQ(0, Xe::String2("Hello world!").IndexOf(Xe::String2("Hello")));
	EXPECT_EQ(6, Xe::String2("Hello world!").IndexOf("world"));
	EXPECT_EQ(6, Xe::String2("Hello world!").IndexOf(Xe::String2("world")));
	EXPECT_EQ(11, Xe::String2("Hello world!").IndexOf("!"));
	EXPECT_EQ(11, Xe::String2("Hello world!").IndexOf(Xe::String2("!")));
	EXPECT_EQ(-1, Xe::String2("Hello world!").IndexOf("hello"));
	EXPECT_EQ(-1, Xe::String2("Hello world!").IndexOf(Xe::String2("hello")));
	EXPECT_EQ(2, Xe::String2("! ciao ciao !").IndexOf(Xe::String2("ciao")));
}

TEST(XeStringTest, IndexOfAnyTest)
{
	EXPECT_EQ(0, Xe::String2("02468").IndexOfAny("0"));
	EXPECT_EQ(1, Xe::String2("02468").IndexOfAny("2"));
	EXPECT_EQ(4, Xe::String2("02468").IndexOfAny("8"));
	EXPECT_EQ(1, Xe::String2("02468").IndexOfAny("24"));
	EXPECT_EQ(1, Xe::String2("02468").IndexOfAny("42"));
	EXPECT_EQ(2, Xe::String2("02468").IndexOfAny("846"));
	EXPECT_EQ(2, Xe::String2("02468").IndexOfAny("13574"));
	EXPECT_EQ(-1, Xe::String2("02468").IndexOfAny(""));
	EXPECT_EQ(-1, Xe::String2("02468").IndexOfAny("1"));

	EXPECT_EQ(-1, Xe::String2("02468").IndexOfAny("2", 0));
	EXPECT_EQ(4, Xe::String2("02468").IndexOfAny("8642", 1));
	EXPECT_EQ(1, Xe::String2("02468").IndexOfAny("8642", 4));
	EXPECT_EQ(-1, Xe::String2("02468").IndexOfAny("13574", 4));

	EXPECT_EQ(1, Xe::String2("abba").IndexOfAny("b"));
	EXPECT_EQ(0, Xe::String2("abba").IndexOfAny("a", 1));
	EXPECT_EQ(1, Xe::String2("abba").IndexOfAny("b", 1));
}

TEST(XeStringTest, LastIndexOfTest)
{
	EXPECT_EQ(12, Xe::String2("Hello world!").LastIndexOf('\0'));
	EXPECT_EQ(0, Xe::String2("Hello world!").LastIndexOf('H'));
	EXPECT_EQ(9, Xe::String2("Hello world!").LastIndexOf('l'));
	EXPECT_EQ(11, Xe::String2("Hello world!").LastIndexOf('!'));
	EXPECT_EQ(-1, Xe::String2("Hello world!").LastIndexOf('?'));
	EXPECT_EQ(12, Xe::String2("Hello world!").LastIndexOf(""));
	EXPECT_EQ(12, Xe::String2("Hello world!").LastIndexOf(Xe::String2("")));
	EXPECT_EQ(0, Xe::String2("Hello world!").LastIndexOf("Hello"));
	EXPECT_EQ(0, Xe::String2("Hello world!").LastIndexOf(Xe::String2("Hello")));
	EXPECT_EQ(6, Xe::String2("Hello world!").LastIndexOf("world"));
	EXPECT_EQ(6, Xe::String2("Hello world!").LastIndexOf(Xe::String2("world")));
	EXPECT_EQ(11, Xe::String2("Hello world!").LastIndexOf("!"));
	EXPECT_EQ(11, Xe::String2("Hello world!").LastIndexOf(Xe::String2("!")));
	EXPECT_EQ(-1, Xe::String2("Hello world!").LastIndexOf("hello"));
	EXPECT_EQ(-1, Xe::String2("Hello world!").LastIndexOf(Xe::String2("hello")));
	EXPECT_EQ(2, Xe::String2("Hello world!").LastIndexOf(Xe::String2("ll")));
	EXPECT_EQ(7, Xe::String2("! ciao ciao !").LastIndexOf(Xe::String2("ciao")));
}

TEST(XeStringTest, LastIndexOfAnyTest)
{
	EXPECT_EQ(0, Xe::String2("02468").LastIndexOfAny("0"));
	EXPECT_EQ(1, Xe::String2("02468").LastIndexOfAny("2"));
	EXPECT_EQ(4, Xe::String2("02468").LastIndexOfAny("8"));
	EXPECT_EQ(2, Xe::String2("02468").LastIndexOfAny("24"));
	EXPECT_EQ(2, Xe::String2("02468").LastIndexOfAny("42"));
	EXPECT_EQ(3, Xe::String2("02468").LastIndexOfAny("264"));
	EXPECT_EQ(2, Xe::String2("02468").LastIndexOfAny("13574"));
	EXPECT_EQ(-1, Xe::String2("02468").LastIndexOfAny(""));
	EXPECT_EQ(-1, Xe::String2("02468").LastIndexOfAny("1"));

	EXPECT_EQ(-1, Xe::String2("02468").LastIndexOfAny("2", 0));
	EXPECT_EQ(1, Xe::String2("02468").LastIndexOfAny("2468", 1));
	EXPECT_EQ(4, Xe::String2("02468").LastIndexOfAny("2468", 4));
	EXPECT_EQ(-1, Xe::String2("02468").LastIndexOfAny("13574", 4));

	EXPECT_EQ(2, Xe::String2("abba").LastIndexOfAny("b"));
	EXPECT_EQ(3, Xe::String2("abba").LastIndexOfAny("a", 1));
	EXPECT_EQ(2, Xe::String2("abba").LastIndexOfAny("b", 1));
}

TEST(XeStringTest, ToUpperTest)
{
	STRASSERT("TEST", Xe::String2("TeSt").ToUpper());
	STRASSERT("TOAST", Xe::String2("tOAsT").ToUpper());
}

TEST(XeStringTest, ToLowerTest)
{
	STRASSERT("test", Xe::String2("TeSt").ToLower());
	STRASSERT("toast", Xe::String2("tOAsT").ToLower());
}

TEST(XeStringTest, SubstringTest)
{
	Xe::String2 str("Hello world!");
	STRASSERT("Hello", str.Substring(0, 5));
	STRASSERT("world!", str.Substring(6));
	EXPECT_THROW(Xe::String2("error").Substring(-1, 1), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Substring(9, 1), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Substring(2, 9), std::invalid_argument);
}

TEST(XeStringTest, AppendTest)
{
	STRASSERT("test-toast", Xe::String2("test").Append("-toast"));
	STRASSERT("test-toast", Xe::String2("test").Append(Xe::String2("-toast")));
	STRASSERT("test-toast", Xe::String2("test") + "-toast");
	STRASSERT("test-toast", Xe::String2("test") + Xe::String2("-toast"));
}

TEST(XeStringTest, InsertTest)
{
	STRASSERT("1234", Xe::String2("1234").Insert(0, ""));
	STRASSERT("1234", Xe::String2("123").Insert(3, "4"));
	STRASSERT("1234", Xe::String2("123").Insert(3, Xe::String2("4")));
	STRASSERT("1234", Xe::String2("124").Insert(2, "3"));
	STRASSERT("1234", Xe::String2("124").Insert(2, Xe::String2("3")));
	STRASSERT("1234", Xe::String2("234").Insert(0, "1"));
	STRASSERT("1234", Xe::String2("234").Insert(0, Xe::String2("1")));
	EXPECT_THROW(Xe::String2("error").Insert(-1, ""), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Insert(9, ""), std::invalid_argument);
}

TEST(XeStringTest, RemoveTest)
{
	STRASSERT("Hello world!", Xe::String2("Hello world!").Remove(0, 0));
	STRASSERT("Hello world!", Xe::String2("Hello world!").Remove(5, 0));
	STRASSERT("Hello", Xe::String2("Hello world!").Remove(5));
	STRASSERT("Hello", Xe::String2("Hello world!").Remove(5, 7));
	STRASSERT("Hello!", Xe::String2("Hello world!").Remove(5, 6));
	STRASSERT("world!", Xe::String2("Hello world!").Remove(0, 6));
	STRASSERT("", Xe::String2("Hello world!").Remove(0));
	EXPECT_THROW(Xe::String2("error").Remove(-1), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Remove(0, -1), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Remove(-1, 0), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Remove(0, 6), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Remove(1, 5), std::invalid_argument);
}

TEST(XeStringTest, ReplaceTest)
{
	STRASSERT("cbbc", Xe::String2("abba").Replace('a', 'c'));
	STRASSERT("abba", Xe::String2("abba").Replace('c', 'a'));
	STRASSERT("Hello XeEngine!", Xe::String2("Hello world!").Replace("world", "XeEngine"));
	STRASSERT("Hello XeEngine!", Xe::String2("Hello world!").Replace(Xe::String2("world"), Xe::String2("XeEngine")));
	STRASSERT("Hello world!", Xe::String2("Hello world!").Replace(Xe::String2("world?"), Xe::String2("XeEngine")));
	STRASSERT("Hello!", Xe::String2("**!").Replace(Xe::String2("**"), Xe::String2("Hello")));
	STRASSERT("Hello world!", Xe::String2("Hello?").Replace(Xe::String2("?"), Xe::String2(" world!")));
	STRASSERT("aaxaaxaax", Xe::String2("axaxax").Replace("x", "ax"));
	STRASSERT("aaa", Xe::String2("axaxax").Replace("x", ""));
	STRASSERT("xxx", Xe::String2("axaxax").Replace("a", ""));
	EXPECT_THROW(Xe::String2("error").Replace("", ""), std::invalid_argument);
	EXPECT_THROW(Xe::String2("error").Replace("", "a"), std::invalid_argument);
}

TEST(XeStringTest, PadLeftTest)
{
	STRASSERT("abba", Xe::String2("abba").PadLeft(0));
	STRASSERT(" abba", Xe::String2("abba").PadLeft(1));
	STRASSERT("  abba", Xe::String2("abba").PadLeft(2));
	STRASSERT("abba", Xe::String2("abba").PadLeft(0, 'X'));
	STRASSERT("Xabba", Xe::String2("abba").PadLeft(1, 'X'));
	STRASSERT("XXabba", Xe::String2("abba").PadLeft(2, 'X'));
}

TEST(XeStringTest, PadRightTest)
{
	STRASSERT("abba", Xe::String2("abba").PadRight(0));
	STRASSERT("abba ", Xe::String2("abba").PadRight(1));
	STRASSERT("abba  ", Xe::String2("abba").PadRight(2));
	STRASSERT("abba", Xe::String2("abba").PadRight(0, 'X'));
	STRASSERT("abbaX", Xe::String2("abba").PadRight(1, 'X'));
	STRASSERT("abbaXX", Xe::String2("abba").PadRight(2, 'X'));
}

TEST(XeStringTest, TrimTest)
{
	STRASSERT("abba", Xe::String2("abba").Trim());
	STRASSERT("abba", Xe::String2(" abba").Trim());
	STRASSERT("abba", Xe::String2("  abba").Trim());
	STRASSERT("abba", Xe::String2("abba ").Trim());
	STRASSERT("abba", Xe::String2("abba  ").Trim());
	STRASSERT("abba", Xe::String2(" abba ").Trim());
	STRASSERT("abba", Xe::String2("  abba  ").Trim());
}

TEST(XeStringTest, TrimStartTest)
{
	STRASSERT("abba", Xe::String2("abba").TrimStart());
	STRASSERT("abba", Xe::String2(" abba").TrimStart());
	STRASSERT("abba", Xe::String2("  abba").TrimStart());
	STRASSERT("abba ", Xe::String2("abba ").TrimStart());
	STRASSERT("abba  ", Xe::String2("abba  ").TrimStart());
	STRASSERT("abba ", Xe::String2(" abba ").TrimStart());
	STRASSERT("abba  ", Xe::String2("  abba  ").TrimStart());
}

TEST(XeStringTest, TrimEndTest)
{
	STRASSERT("abba", Xe::String2("abba").TrimEnd());
	STRASSERT(" abba", Xe::String2(" abba").TrimEnd());
	STRASSERT("  abba", Xe::String2("  abba").TrimEnd());
	STRASSERT("abba", Xe::String2("abba ").TrimEnd());
	STRASSERT("abba", Xe::String2("abba  ").TrimEnd());
	STRASSERT(" abba", Xe::String2(" abba ").TrimEnd());
	STRASSERT("  abba", Xe::String2("  abba  ").TrimEnd());
}

TEST(XeStringTest, CompareTest)
{
	EXPECT_EQ(0, Xe::String2::Compare("", ""));
	EXPECT_EQ(0, Xe::String2::Compare("a", "a"));
	EXPECT_EQ(-1, Xe::String2::Compare("A", "a"));
	EXPECT_EQ(1, Xe::String2::Compare("a", "A"));
	EXPECT_EQ(1, Xe::String2::Compare("ebba", "abba"));
	EXPECT_EQ(-1, Xe::String2::Compare("abba", "ebba"));
	EXPECT_EQ(-1, Xe::String2::Compare("1", "22"));
	EXPECT_EQ(1, Xe::String2::Compare("22", "1"));
}

TEST(XeStringTest, CompareInsensitiveTest)
{
	EXPECT_EQ(0, Xe::String2::CompareInsensitive("", ""));
	EXPECT_EQ(0, Xe::String2::CompareInsensitive("a", "a"));
	EXPECT_EQ(0, Xe::String2::CompareInsensitive("A", "a"));
	EXPECT_EQ(0, Xe::String2::CompareInsensitive("a", "A"));
	EXPECT_EQ(1, Xe::String2::CompareInsensitive("ebba", "abba"));
	EXPECT_EQ(-1, Xe::String2::CompareInsensitive("abba", "ebba"));
	EXPECT_EQ(-1, Xe::String2::CompareInsensitive("1", "22"));
	EXPECT_EQ(1, Xe::String2::CompareInsensitive("22", "1"));
}

TEST(XeStringTest, JoinTest)
{
	const char* str[]{ "1", "2", "3" };
	STRASSERT("1, 2, 3", Xe::String2::Join(", ", str));
	STRASSERT("1, 2, 3", Xe::String2::Join(", ", str, 3));
	STRASSERT("1,2,3", Xe::String2::Join(",", str, 3));
	STRASSERT("123", Xe::String2::Join("", str, 3));
	STRASSERT("1,2,3", Xe::String2::Join(',', str, 3));

	Xe::String2 str2[]{ "1", "2", "3" };
	STRASSERT("1, 2, 3", Xe::String2::Join(", ", str));
	STRASSERT("1, 2, 3", Xe::String2::Join(", ", str, 3));
	STRASSERT("1,2,3", Xe::String2::Join(",", str, 3));
	STRASSERT("123", Xe::String2::Join("", str, 3));
	STRASSERT("1,2,3", Xe::String2::Join(',', str, 3));

	STRASSERT("1, 2, 3", Xe::String2::Join(Xe::String2(", "), str));
	STRASSERT("1, 2, 3", Xe::String2::Join(Xe::String2(", "), str, 3));
	STRASSERT("1,2,3", Xe::String2::Join(Xe::String2(","), str, 3));
	STRASSERT("123", Xe::String2::Join(Xe::String2(""), str, 3));
}