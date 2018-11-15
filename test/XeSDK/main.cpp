#include <stddef.h>
#include <stdio.h>
#include <XeSDK/XeCore.h>
#include <XeSDK/XeStringSpan.h>
#include <gtest/gtest.h>

using namespace Xe;
using namespace Xe::Core;

TEST(Sample, Sample1)
{
	EXPECT_EQ(StringSpan("XeEngine"), GetEngineInfo().Name);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace Xe::Core;