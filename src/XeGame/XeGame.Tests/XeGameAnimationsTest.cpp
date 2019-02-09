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

TEST(XeGameAnimationsTest, SaveAndParseFrameSequenceTest) {

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	FrameSequence expected, actual;

	expected.Name = "AnimationNameTest";
	expected.Duration = 123;
	expected.Loop = 456;
	expected.Flags = 789;

	Frame& frame1 = Add(expected.Frames);
	Frame& frame2 = Add(expected.Frames);
	frame1.Duration = 1212;

	SaveJson(expected, memStream);
	memStream->SetPosition(0);
	ParseJson(actual, memStream);

	ASSERT_STREQ(expected.Name, actual.Name);
	ASSERT_EQ(expected.Duration, actual.Duration);
	ASSERT_EQ(expected.Loop, actual.Loop);
	ASSERT_EQ(expected.Flags, actual.Flags);
	ASSERT_EQ(expected.Frames.size(), actual.Frames.size());
	ASSERT_EQ((*expected.Frames.begin()).Duration, (*actual.Frames.begin()).Duration);
}

TEST(XeGameAnimationsTest, SaveAndParseSpriteSheetTest) {

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	SpriteSheet expected, actual;

	expected.Path = "SomeRandomPath";

	SaveJson(expected, memStream);
	memStream->SetPosition(0);
	ParseJson(actual, memStream);

	ASSERT_STREQ(expected.Path, actual.Path);
}

TEST(XeGameAnimationsTest, SaveAndParseAnimationDocumentTest) {

	Xe::ObjPtr<IMemoryStream> memStream = IO::IMemoryStream::New(0, true);
	AnimationDocument expected, actual;

	expected.Name = "DocumentNameTest";

	auto& expectedSpriteSheet1 = Add(expected.SpriteSheets);
	auto& expectedSpriteSheet2 = Add(expected.SpriteSheets);
	expectedSpriteSheet1.Path = "TestPath";

	auto& expectedSequence1 = Add(expected.Sequences);
	auto& expectedSequence2 = Add(expected.Sequences);
	auto& expectedSequence3 = Add(expected.Sequences);
	expectedSequence1.Duration = 123;

	SaveJson(expected, memStream);
	memStream->SetPosition(0);
	ParseJson(actual, memStream);

	ASSERT_STREQ(expected.Name, actual.Name);
	ASSERT_EQ(expected.SpriteSheets.size(), actual.SpriteSheets.size());
	ASSERT_STREQ(expectedSpriteSheet1.Path, (*actual.SpriteSheets.begin()).Path);
	ASSERT_EQ(expected.Sequences.size(), actual.Sequences.size());
	ASSERT_EQ(expectedSequence1.Duration, (*actual.Sequences.begin()).Duration);
}