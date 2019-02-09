#include "pch.h"
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeGame/XeGameAnimationDef.h>
#include "JsonTestHelper.h"

using namespace Xe;
using namespace Xe::IO;
using namespace Xe::Game::Animations;

TEST(XeGameAnimationsTest, SaveAndParseFrameTest) {

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	Frame expected, actual;

	expected.Rect.left = 12;
	expected.Rect.top = 34;
	expected.Rect.right = 56;
	expected.Rect.bottom = 78;
	expected.Pivot.x = 13;
	expected.Pivot.y = 24;
	expected.Duration = 99;
	expected.Reference = 123;

	SaveJson(expected, memStream);
	memStream->SetPosition(0);
	ParseJson(actual, memStream);

	ASSERT_EQ(expected.Rect.left, actual.Rect.left);
	ASSERT_EQ(expected.Rect.top, actual.Rect.top);
	ASSERT_EQ(expected.Rect.right, actual.Rect.right);
	ASSERT_EQ(expected.Rect.bottom, actual.Rect.bottom);
	ASSERT_EQ(expected.Pivot.x, actual.Pivot.x);
	ASSERT_EQ(expected.Pivot.y, actual.Pivot.y);
	ASSERT_EQ(expected.Duration, actual.Duration);
	ASSERT_EQ(expected.Reference, actual.Reference);
}