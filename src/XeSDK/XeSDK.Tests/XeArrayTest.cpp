#include "pch.h"
#include <XeSDK/XeCollectionsArray.h>
#include <XeSDK/XeCollectionsStaticArray.h>
#include "gtest/gtest.h"

using namespace Xe::Collections;

TEST(XeCollectionsArrayTest, StaticArrayTest)
{
	int cArray[] = { 0, 2, 4, 8, 8, 16 };
	auto array = StaticArray<int, lengthof(cArray)>(cArray);

	EXPECT_FALSE(array.IsEmpty());
	EXPECT_EQ(6, array.GetLength());
	EXPECT_EQ(0, array.IndexOf(0));
	EXPECT_EQ(2, array.IndexOf(4));
	EXPECT_EQ(4, array[2]);
	EXPECT_EQ(3, array.IndexOf(8));
	EXPECT_EQ(-1, array.IndexOf(7));
	EXPECT_EQ(4, array.LastIndexOf(8));
	EXPECT_EQ(-1, array.LastIndexOf(7));
	EXPECT_TRUE(array.Contains(8));
	EXPECT_FALSE(array.Contains(7));
}

TEST(XeCollectionsArrayTest, ArrayTest)
{
	int cArray[] = { 0, 2, 4, 8, 8, 16 };
	auto array = Array<int>(cArray);

	EXPECT_FALSE(array.IsEmpty());
	EXPECT_EQ(6, array.GetLength());
	EXPECT_EQ(0, array.IndexOf(0));
	EXPECT_EQ(2, array.IndexOf(4));
	EXPECT_EQ(4, array[2]);
	EXPECT_EQ(3, array.IndexOf(8));
	EXPECT_EQ(-1, array.IndexOf(7));
	EXPECT_EQ(4, array.LastIndexOf(8));
	EXPECT_EQ(-1, array.LastIndexOf(7));
	EXPECT_TRUE(array.Contains(8));
	EXPECT_FALSE(array.Contains(7));
}

TEST(XeCollectionsArrayTest, SubArrayTest)
{
	int cArray[] = { 0, 2, 4, 8, 8, 16 };
	auto array = Array<int>(cArray);

	EXPECT_FALSE(array.IsEmpty());
	EXPECT_EQ(6, array.GetLength());
	EXPECT_EQ(5, array.SubArray(1).GetLength());
	EXPECT_EQ(4, array.SubArray(1)[1]);
	EXPECT_EQ(3, array.SubArray(0, 3).GetLength());
	EXPECT_EQ(4, array.SubArray(0, 3)[2]);
	EXPECT_TRUE(array.SubArray(0, 3).SubArray(2).Contains(4));
}
