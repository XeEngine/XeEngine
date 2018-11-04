#include "pch.h"
#include <XeSDK/XeString.h>
#include "gtest/gtest.h"

#define STRASSERT(strExpected, str) \
	ASSERT_STREQ(strExpected, (str)); \
	EXPECT_EQ(strlen(str), (str).GetLength());

void XeStringTest(const char* strExpected)
{
	Xe::String str(strExpected);
	EXPECT_EQ(0, strcmp(strExpected, str.GetData()));
	STRASSERT(strExpected, str);
}

TEST(XeStringTest, BaseTest)
{
	XeStringTest("");
	XeStringTest("test");
	XeStringTest(Xe::String::Empty);
}

TEST(XeStringTest, EmptyTest)
{
	STRASSERT("", Xe::String());
	STRASSERT("", Xe::String::Empty);
}

TEST(XeStringTest, IsEmptyTest)
{
	EXPECT_TRUE(Xe::String::Empty.IsEmpty());
	EXPECT_TRUE(Xe::String("").IsEmpty());
	EXPECT_FALSE(Xe::String("!").IsEmpty());
}

TEST(XeStringTest, IsEmptyOrWhitespaceTest)
{
	EXPECT_TRUE(Xe::String().IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String::Empty.IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String("").IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String(" ").IsEmptyOrWhitespace());
	EXPECT_TRUE(Xe::String("   ").IsEmptyOrWhitespace());
	EXPECT_FALSE(Xe::String("!").IsEmptyOrWhitespace());
	EXPECT_FALSE(Xe::String(" !").IsEmptyOrWhitespace());
	EXPECT_FALSE(Xe::String("! ").IsEmptyOrWhitespace());
}

TEST(XeStringTest, StartsWithTest)
{
	EXPECT_TRUE(Xe::String("Hello world!").StartsWith(""));
	EXPECT_TRUE(Xe::String("Hello world!").StartsWith(Xe::String("")));
	EXPECT_TRUE(Xe::String("Hello world!").StartsWith("H"));
	EXPECT_TRUE(Xe::String("Hello world!").StartsWith(Xe::String("H")));
	EXPECT_TRUE(Xe::String("Hello world!").StartsWith("Hello"));
	EXPECT_TRUE(Xe::String("Hello world!").StartsWith(Xe::String("Hello")));
	EXPECT_FALSE(Xe::String("Hello world!").StartsWith("ello"));
	EXPECT_FALSE(Xe::String("Hello world!").StartsWith(Xe::String("ello")));
}

TEST(XeStringTest, EndsWithTest)
{
	EXPECT_TRUE(Xe::String("Hello world!").EndsWith(""));
	EXPECT_TRUE(Xe::String("Hello world!").EndsWith(Xe::String("")));
	EXPECT_TRUE(Xe::String("Hello world!").EndsWith("!"));
	EXPECT_TRUE(Xe::String("Hello world!").EndsWith(Xe::String("!")));
	EXPECT_TRUE(Xe::String("Hello world!").EndsWith("world!"));
	EXPECT_TRUE(Xe::String("Hello world!").EndsWith(Xe::String("world!")));
	EXPECT_FALSE(Xe::String("Hello world!").EndsWith("world"));
	EXPECT_FALSE(Xe::String("Hello world!").EndsWith(Xe::String("world")));
}

TEST(XeStringTest, IndexOfTest)
{
	EXPECT_EQ(-1, Xe::String("Hello world!").IndexOf('\0'));
	EXPECT_EQ(0, Xe::String("Hello world!").IndexOf('H'));
	EXPECT_EQ(2, Xe::String("Hello world!").IndexOf('l'));
	EXPECT_EQ(11, Xe::String("Hello world!").IndexOf('!'));
	EXPECT_EQ(-1, Xe::String("Hello world!").IndexOf('?'));
	EXPECT_EQ(0, Xe::String("Hello world!").IndexOf(""));
	EXPECT_EQ(0, Xe::String("Hello world!").IndexOf(Xe::String("")));
	EXPECT_EQ(0, Xe::String("Hello world!").IndexOf("Hello"));
	EXPECT_EQ(0, Xe::String("Hello world!").IndexOf(Xe::String("Hello")));
	EXPECT_EQ(6, Xe::String("Hello world!").IndexOf("world"));
	EXPECT_EQ(6, Xe::String("Hello world!").IndexOf(Xe::String("world")));
	EXPECT_EQ(11, Xe::String("Hello world!").IndexOf("!"));
	EXPECT_EQ(11, Xe::String("Hello world!").IndexOf(Xe::String("!")));
	EXPECT_EQ(-1, Xe::String("Hello world!").IndexOf("hello"));
	EXPECT_EQ(-1, Xe::String("Hello world!").IndexOf(Xe::String("hello")));
	EXPECT_EQ(2, Xe::String("! ciao ciao !").IndexOf(Xe::String("ciao")));
}

TEST(XeStringTest, IndexOfAnyTest)
{
	EXPECT_EQ(0, Xe::String("02468").IndexOfAny("0"));
	EXPECT_EQ(1, Xe::String("02468").IndexOfAny("2"));
	EXPECT_EQ(4, Xe::String("02468").IndexOfAny("8"));
	EXPECT_EQ(1, Xe::String("02468").IndexOfAny("24"));
	EXPECT_EQ(1, Xe::String("02468").IndexOfAny("42"));
	EXPECT_EQ(2, Xe::String("02468").IndexOfAny("846"));
	EXPECT_EQ(2, Xe::String("02468").IndexOfAny("13574"));
	EXPECT_EQ(-1, Xe::String("02468").IndexOfAny(""));
	EXPECT_EQ(-1, Xe::String("02468").IndexOfAny("1"));
	EXPECT_EQ(1, Xe::String("abba").IndexOfAny("b"));
}

TEST(XeStringTest, LastIndexOfTest)
{
	EXPECT_EQ(-1, Xe::String("Hello world!").LastIndexOf('\0'));
	EXPECT_EQ(0, Xe::String("Hello world!").LastIndexOf('H'));
	EXPECT_EQ(9, Xe::String("Hello world!").LastIndexOf('l'));
	EXPECT_EQ(11, Xe::String("Hello world!").LastIndexOf('!'));
	EXPECT_EQ(-1, Xe::String("Hello world!").LastIndexOf('?'));
	EXPECT_EQ(12, Xe::String("Hello world!").LastIndexOf(""));
	EXPECT_EQ(12, Xe::String("Hello world!").LastIndexOf(Xe::String("")));
	EXPECT_EQ(0, Xe::String("Hello world!").LastIndexOf("H"));
	EXPECT_EQ(0, Xe::String("Hello world!").LastIndexOf("Hello"));
	EXPECT_EQ(0, Xe::String("Hello world!").LastIndexOf(Xe::String("Hello")));
	EXPECT_EQ(6, Xe::String("Hello world!").LastIndexOf("world"));
	EXPECT_EQ(6, Xe::String("Hello world!").LastIndexOf(Xe::String("world")));
	EXPECT_EQ(11, Xe::String("Hello world!").LastIndexOf("!"));
	EXPECT_EQ(11, Xe::String("Hello world!").LastIndexOf(Xe::String("!")));
	EXPECT_EQ(-1, Xe::String("Hello world!").LastIndexOf("hello"));
	EXPECT_EQ(-1, Xe::String("Hello world!").LastIndexOf(Xe::String("hello")));
	EXPECT_EQ(2, Xe::String("Hello world!").LastIndexOf(Xe::String("ll")));
	EXPECT_EQ(7, Xe::String("! ciao ciao !").LastIndexOf(Xe::String("ciao")));
}

TEST(XeStringTest, LastIndexOfAnyTest)
{
	EXPECT_EQ(0, Xe::String("02468").LastIndexOfAny("0"));
	EXPECT_EQ(1, Xe::String("02468").LastIndexOfAny("2"));
	EXPECT_EQ(4, Xe::String("02468").LastIndexOfAny("8"));
	EXPECT_EQ(2, Xe::String("02468").LastIndexOfAny("24"));
	EXPECT_EQ(2, Xe::String("02468").LastIndexOfAny("42"));
	EXPECT_EQ(3, Xe::String("02468").LastIndexOfAny("264"));
	EXPECT_EQ(2, Xe::String("02468").LastIndexOfAny("13574"));
	EXPECT_EQ(-1, Xe::String("02468").LastIndexOfAny(""));
	EXPECT_EQ(-1, Xe::String("02468").LastIndexOfAny("1"));
	EXPECT_EQ(2, Xe::String("abba").LastIndexOfAny("b"));
}

TEST(XeStringTest, ToUpperTest)
{
	STRASSERT("TEST", Xe::String("TeSt").ToUpper());
	STRASSERT("TOAST", Xe::String("tOAsT").ToUpper());
}

TEST(XeStringTest, ToLowerTest)
{
	STRASSERT("test", Xe::String("TeSt").ToLower());
	STRASSERT("toast", Xe::String("tOAsT").ToLower());
}

TEST(XeStringTest, SubstringTest)
{
	Xe::String str("Hello world!");
	STRASSERT("Hello", str.Substring(0, 5));
	STRASSERT("world!", str.Substring(6));
	EXPECT_THROW(Xe::String("error").Substring(-1, 1), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Substring(9, 1), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Substring(2, 9), std::invalid_argument);
}

TEST(XeStringTest, AppendTest)
{
	STRASSERT("test-toast", Xe::String("test").Append("-toast"));
	STRASSERT("test-toast", Xe::String("test").Append(Xe::String("-toast")));
	//STRASSERT("test-toast", Xe::String("test") + "-toast");
	//STRASSERT("test-toast", Xe::String("test") + Xe::StringSpan("-toast"));
	STRASSERT("test-toast", Xe::String("test") + Xe::String("-toast"));
}

TEST(XeStringTest, InsertTest)
{
	STRASSERT("1234", Xe::String("1234").Insert(0, ""));
	STRASSERT("1234", Xe::String("123").Insert(3, "4"));
	STRASSERT("1234", Xe::String("123").Insert(3, Xe::String("4")));
	STRASSERT("1234", Xe::String("124").Insert(2, "3"));
	STRASSERT("1234", Xe::String("124").Insert(2, Xe::String("3")));
	STRASSERT("1234", Xe::String("234").Insert(0, "1"));
	STRASSERT("1234", Xe::String("234").Insert(0, Xe::String("1")));
	EXPECT_THROW(Xe::String("error").Insert(-1, ""), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Insert(9, ""), std::invalid_argument);
}

TEST(XeStringTest, RemoveTest)
{
	STRASSERT("Hello world!", Xe::String("Hello world!").Remove(0, 0));
	STRASSERT("Hello world!", Xe::String("Hello world!").Remove(5, 0));
	STRASSERT("Hello", Xe::String("Hello world!").Remove(5));
	STRASSERT("Hello", Xe::String("Hello world!").Remove(5, 7));
	STRASSERT("Hello!", Xe::String("Hello world!").Remove(5, 6));
	STRASSERT("world!", Xe::String("Hello world!").Remove(0, 6));
	STRASSERT("", Xe::String("Hello world!").Remove(0));
	EXPECT_THROW(Xe::String("error").Remove(-1), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Remove(0, -1), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Remove(-1, 0), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Remove(0, 6), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Remove(1, 5), std::invalid_argument);
}

TEST(XeStringTest, ReplaceTest)
{
	STRASSERT("cbbc", Xe::String("abba").Replace('a', 'c'));
	STRASSERT("abba", Xe::String("abba").Replace('c', 'a'));
	STRASSERT("Hello XeEngine!", Xe::String("Hello world!").Replace("world", "XeEngine"));
	STRASSERT("Hello XeEngine!", Xe::String("Hello world!").Replace(Xe::String("world"), Xe::String("XeEngine")));
	STRASSERT("Hello world!", Xe::String("Hello world!").Replace(Xe::String("world?"), Xe::String("XeEngine")));
	STRASSERT("Hello!", Xe::String("**!").Replace(Xe::String("**"), Xe::String("Hello")));
	STRASSERT("Hello world!", Xe::String("Hello?").Replace(Xe::String("?"), Xe::String(" world!")));
	STRASSERT("aaxaaxaax", Xe::String("axaxax").Replace("x", "ax"));
	STRASSERT("aaa", Xe::String("axaxax").Replace("x", ""));
	STRASSERT("xxx", Xe::String("axaxax").Replace("a", ""));
	EXPECT_THROW(Xe::String("error").Replace("", ""), std::invalid_argument);
	EXPECT_THROW(Xe::String("error").Replace("", "a"), std::invalid_argument);
}

TEST(XeStringTest, PadLeftTest)
{
	STRASSERT("abba", Xe::String("abba").PadLeft(0));
	STRASSERT(" abba", Xe::String("abba").PadLeft(1));
	STRASSERT("  abba", Xe::String("abba").PadLeft(2));
	STRASSERT("abba", Xe::String("abba").PadLeft(0, 'X'));
	STRASSERT("Xabba", Xe::String("abba").PadLeft(1, 'X'));
	STRASSERT("XXabba", Xe::String("abba").PadLeft(2, 'X'));
}

TEST(XeStringTest, PadRightTest)
{
	STRASSERT("abba", Xe::String("abba").PadRight(0));
	STRASSERT("abba ", Xe::String("abba").PadRight(1));
	STRASSERT("abba  ", Xe::String("abba").PadRight(2));
	STRASSERT("abba", Xe::String("abba").PadRight(0, 'X'));
	STRASSERT("abbaX", Xe::String("abba").PadRight(1, 'X'));
	STRASSERT("abbaXX", Xe::String("abba").PadRight(2, 'X'));
}

TEST(XeStringTest, TrimTest)
{
	STRASSERT("abba", Xe::String("abba").Trim());
	STRASSERT("abba", Xe::String(" abba").Trim());
	STRASSERT("abba", Xe::String("  abba").Trim());
	STRASSERT("abba", Xe::String("abba ").Trim());
	STRASSERT("abba", Xe::String("abba  ").Trim());
	STRASSERT("abba", Xe::String(" abba ").Trim());
	STRASSERT("abba", Xe::String("  abba  ").Trim());
}

TEST(XeStringTest, TrimStartTest)
{
	STRASSERT("abba", Xe::String("abba").TrimStart());
	STRASSERT("abba", Xe::String(" abba").TrimStart());
	STRASSERT("abba", Xe::String("  abba").TrimStart());
	STRASSERT("abba ", Xe::String("abba ").TrimStart());
	STRASSERT("abba  ", Xe::String("abba  ").TrimStart());
	STRASSERT("abba ", Xe::String(" abba ").TrimStart());
	STRASSERT("abba  ", Xe::String("  abba  ").TrimStart());
}

TEST(XeStringTest, TrimEndTest)
{
	STRASSERT("abba", Xe::String("abba").TrimEnd());
	STRASSERT(" abba", Xe::String(" abba").TrimEnd());
	STRASSERT("  abba", Xe::String("  abba").TrimEnd());
	STRASSERT("abba", Xe::String("abba ").TrimEnd());
	STRASSERT("abba", Xe::String("abba  ").TrimEnd());
	STRASSERT(" abba", Xe::String(" abba ").TrimEnd());
	STRASSERT("  abba", Xe::String("  abba  ").TrimEnd());
}

TEST(XeStringTest, CompareTest)
{
	EXPECT_EQ(0, Xe::String::Compare("", ""));
	EXPECT_EQ(0, Xe::String::Compare("a", "a"));
	EXPECT_EQ(-1, Xe::String::Compare("A", "a"));
	EXPECT_EQ(1, Xe::String::Compare("a", "A"));
	EXPECT_EQ(1, Xe::String::Compare("ebba", "abba"));
	EXPECT_EQ(-1, Xe::String::Compare("abba", "ebba"));
	EXPECT_EQ(-1, Xe::String::Compare("1", "22"));
	EXPECT_EQ(1, Xe::String::Compare("22", "1"));
}

TEST(XeStringTest, CompareInsensitiveTest)
{
	EXPECT_EQ(0, Xe::String::CompareInsensitive("", ""));
	EXPECT_EQ(0, Xe::String::CompareInsensitive("a", "a"));
	EXPECT_EQ(0, Xe::String::CompareInsensitive("A", "a"));
	EXPECT_EQ(0, Xe::String::CompareInsensitive("a", "A"));
	EXPECT_EQ(1, Xe::String::CompareInsensitive("ebba", "abba"));
	EXPECT_EQ(-1, Xe::String::CompareInsensitive("abba", "ebba"));
	EXPECT_EQ(-1, Xe::String::CompareInsensitive("1", "22"));
	EXPECT_EQ(1, Xe::String::CompareInsensitive("22", "1"));
}

TEST(XeStringTest, JoinTest)
{
	const Xe::StringSpan str[]{ "1", "2", "3" };
	STRASSERT("1, 2, 3", Xe::String::Join(", ", str));
	STRASSERT("1, 2, 3", Xe::String::Join(", ", str, 3));
	STRASSERT("1,2,3", Xe::String::Join(",", str, 3));
	STRASSERT("123", Xe::String::Join("", str, 3));
	STRASSERT("1,2,3", Xe::String::Join(',', str, 3));

	const Xe::String str2[]{ "1", "2", "3" };
	STRASSERT("1, 2, 3", Xe::String::Join(", ", str));
	STRASSERT("1, 2, 3", Xe::String::Join(", ", str, 3));
	STRASSERT("1,2,3", Xe::String::Join(",", str, 3));
	STRASSERT("123", Xe::String::Join("", str, 3));
	STRASSERT("1,2,3", Xe::String::Join(',', str, 3));

	STRASSERT("1, 2, 3", Xe::String::Join(Xe::String(", "), str));
	STRASSERT("1, 2, 3", Xe::String::Join(Xe::String(", "), str, 3));
	STRASSERT("1,2,3", Xe::String::Join(Xe::String(","), str, 3));
	STRASSERT("123", Xe::String::Join(Xe::String(""), str, 3));
}

TEST(XeStringTest, TryParseIntTest)
{
	int value;
	EXPECT_TRUE(Xe::StringSpan("").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("0").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("1").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("10").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("99999999").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("11").TryParse(value, 2));
	EXPECT_TRUE(Xe::StringSpan("01234567").TryParse(value, 8));
	EXPECT_TRUE(Xe::StringSpan("9ABCDEF").TryParse(value, 16));
	EXPECT_TRUE(Xe::StringSpan("deadbeef").TryParse(value, 16));
	EXPECT_TRUE(Xe::StringSpan("+123").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("-123").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("\t\t \n\r+123").TryParse(value));
	EXPECT_TRUE(Xe::StringSpan("123 ").TryParse(value));

	EXPECT_FALSE(Xe::StringSpan(" a123 ").TryParse(value));
	EXPECT_FALSE(Xe::StringSpan(" 123 a").TryParse(value));
	EXPECT_FALSE(Xe::StringSpan("2").TryParse(value, 2));
	EXPECT_FALSE(Xe::StringSpan("9").TryParse(value, 8));
	EXPECT_FALSE(Xe::StringSpan("A").TryParse(value, 10));
	EXPECT_FALSE(Xe::StringSpan("x").TryParse(value, 16));
	EXPECT_FALSE(Xe::StringSpan("12").TryParse(value, 2));
	EXPECT_FALSE(Xe::StringSpan("19").TryParse(value, 8));
	EXPECT_FALSE(Xe::StringSpan("1A").TryParse(value, 10));
	EXPECT_FALSE(Xe::StringSpan("1x").TryParse(value, 16));
}

TEST(XeStringTest, ParseIntTest)
{
	int value;
	EXPECT_EQ(0, Xe::StringSpan("").Parse(-1));
	EXPECT_EQ(0, Xe::StringSpan("0").Parse(-1));
	EXPECT_EQ(1, Xe::StringSpan("1").Parse(-1));
	EXPECT_EQ(10, Xe::StringSpan("10").Parse(-1));
	EXPECT_EQ(99999999, Xe::StringSpan("99999999").Parse(-1));
	EXPECT_EQ(3, Xe::StringSpan("11").Parse(-1, 2));
	EXPECT_EQ(342391, Xe::StringSpan("01234567").Parse(-1, 8));
	EXPECT_EQ(162254319, Xe::StringSpan("9ABCDEF").Parse(-1, 16));
	EXPECT_EQ(3735928559U, Xe::StringSpan("deadbeef").Parse(-1, 16));
	EXPECT_EQ(+123, Xe::StringSpan("+123").Parse(-1));
	EXPECT_EQ(-123, Xe::StringSpan("-123").Parse(-1));
	EXPECT_EQ(+123, Xe::StringSpan("\t\t \n\r+123").Parse(-1));
	EXPECT_EQ(+123, Xe::StringSpan("123 ").Parse(-1));
	
	EXPECT_EQ(-1, Xe::StringSpan(" a123 ").Parse(-1));
	EXPECT_EQ(-1, Xe::StringSpan(" 123 a").Parse(-1));
	EXPECT_EQ(-1, Xe::StringSpan("2").Parse(-1, 2));
	EXPECT_EQ(-1, Xe::StringSpan("9").Parse(-1, 8));
	EXPECT_EQ(-1, Xe::StringSpan("A").Parse(-1, 10));
	EXPECT_EQ(-1, Xe::StringSpan("x").Parse(-1, 16));
	EXPECT_EQ(-1, Xe::StringSpan("12").Parse(-1, 2));
	EXPECT_EQ(-1, Xe::StringSpan("19").Parse(-1, 8));
	EXPECT_EQ(-1, Xe::StringSpan("1A").Parse(-1, 10));
	EXPECT_EQ(-1, Xe::StringSpan("1x").Parse(-1, 16));
}