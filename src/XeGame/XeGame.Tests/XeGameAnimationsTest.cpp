#include "pch.h"
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeGame/XeGameAnimationDef.h>
#include "JsonTestHelper.h"
#include <XeGame/IGameAnimationsAnimationGroup.h>
#include <XeGame/IGameAnimationsSequenceAnimator.h>
#include <XeGame/XeGameAnimations.h>

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

TEST(XeGameAnimationsTest, FactoryAnimationGroupTest) {
	AnimationDocument doc;
	doc.Name = "AnimationDocumentName";
	auto& sequence1 = Add(doc.Sequences);
	auto& sequence2 = Add(doc.Sequences);
	auto& sequence3 = Add(doc.Sequences);
	auto& sequence4 = Add(doc.Sequences);
	auto& sequence5 = Add(doc.Sequences);
	auto& sequence6 = Add(doc.Sequences);

	sequence1.Name = "Anim1";
	sequence2.Name = "Anim2";
	sequence3.Name = "Anim3";
	sequence4.Name = "Anim4";
	sequence5.Name = "Anim5";
	sequence6.Name = "Anim6";

	sequence3.Duration = 123;

	IAnimationGroup* pAnimGroup;
	ASSERT_EQ(true, Factory(&pAnimGroup, doc));
	doc.Name = "DifferentName";
	doc.Sequences.clear();
	doc.SpriteSheets.clear();

	ASSERT_STREQ("AnimationDocumentName", pAnimGroup->GetName());
	ASSERT_EQ(6, pAnimGroup->GetSequenceNames().size());

	ASSERT_STREQ("Anim1", (*pAnimGroup->GetSequenceNames().begin()));
	ASSERT_STREQ("Anim6", (*std::prev(pAnimGroup->GetSequenceNames().end())));
	ASSERT_EQ(true, pAnimGroup->IsSequenceExists("Anim2"));
	ASSERT_EQ(false, pAnimGroup->IsSequenceExists("NotExistingAnim"));
	ASSERT_EQ(false, pAnimGroup->IsSequenceExists(""));

	const FrameSequence* pFrameSequence = (const FrameSequence*)0xDEADCCCC;
	ASSERT_EQ(false, pAnimGroup->TryGetSequence(&pFrameSequence, "none"));
	ASSERT_EQ(nullptr, pFrameSequence);

	ASSERT_EQ(true, pAnimGroup->TryGetSequence(&pFrameSequence, "Anim3"));
	ASSERT_NE(nullptr, pFrameSequence);
}


TEST(XeGameAnimationsTest, CreateSequenceAnimatorTest) {
	AnimationDocument doc;
	doc.Name = "AnimationDocumentName";

	auto& sequence = Add(doc.Sequences);
	sequence.Name = "Sequence";
	sequence.Loop = 1;

	auto& frame1 = Add(sequence.Frames);
	auto& frame2 = Add(sequence.Frames);
	frame1.Duration = 666;
	frame2.Duration = 777;

	IAnimationGroup* pAnimGroup;
	ASSERT_EQ(true, Factory(&pAnimGroup, doc));

	ISequenceAnimator* pSequenceAnimator = (ISequenceAnimator*)0xDEADCCCC;
	ASSERT_EQ(false, pAnimGroup->TryCreateSequenceAnimator(&pSequenceAnimator, "none"));
	ASSERT_EQ(nullptr, pSequenceAnimator);

	ASSERT_EQ(true, pAnimGroup->TryCreateSequenceAnimator(&pSequenceAnimator, "Sequence"));
	ASSERT_NE(nullptr, pSequenceAnimator);

	ASSERT_STREQ(sequence.Name, pSequenceAnimator->GetName());
	ASSERT_EQ(sequence.Loop, pSequenceAnimator->GetLoopIndex());
	ASSERT_EQ(sequence.Frames.size(), pSequenceAnimator->GetFrameCount());
	ASSERT_EQ((*sequence.Frames.begin()).Duration, pSequenceAnimator->GetFrame(0).Duration);
	ASSERT_EQ((*std::next(sequence.Frames.begin())).Duration, pSequenceAnimator->GetFrame(1).Duration);

	ASSERT_THROW(pSequenceAnimator->GetFrame(sequence.Frames.size()), std::invalid_argument);
}


TEST(XeGameAnimationsTest, ExecuteSequenceAnimatorWithLoopTest) {
	AnimationDocument doc;
	doc.Name = "AnimationDocumentName";

	auto& sequence = Add(doc.Sequences);
	sequence.Name = "Sequence";
	sequence.Loop = 1;
	sequence.Duration = 4;

	auto& frame1 = Add(sequence.Frames);
	auto& frame2 = Add(sequence.Frames);
	auto& frame3 = Add(sequence.Frames);
	auto& frame4 = Add(sequence.Frames);

	frame1.Duration = 0;
	frame2.Duration = 128;
	frame3.Duration = 256;
	frame4.Duration = 256;

	IAnimationGroup* pAnimGroup;
	ASSERT_EQ(true, Factory(&pAnimGroup, doc));

	ISequenceAnimator* pSequenceAnimator;
	ASSERT_EQ(true, pAnimGroup->TryCreateSequenceAnimator(&pSequenceAnimator, "Sequence"));

	ASSERT_EQ(0, pSequenceAnimator->GetCurrentFrameIndex());
	ASSERT_EQ(0, pSequenceAnimator->GetLoopHitCount());
	ASSERT_EQ(false, pSequenceAnimator->IsAnimationFinish());
	ASSERT_EQ(0.0, pSequenceAnimator->GetTime());

	pSequenceAnimator->AddTime(0.8);
	ASSERT_EQ(0, pSequenceAnimator->GetCurrentFrameIndex());

	pSequenceAnimator->SetTime(1.0);
	ASSERT_EQ(1.0, pSequenceAnimator->GetTime());
	ASSERT_EQ(1, pSequenceAnimator->GetCurrentFrameIndex());

	pSequenceAnimator->AddTime(1.5);
	ASSERT_EQ(3, pSequenceAnimator->GetCurrentFrameIndex());
	ASSERT_EQ(0, pSequenceAnimator->GetLoopHitCount());

	pSequenceAnimator->AddTime(1.0);
	ASSERT_EQ(1, pSequenceAnimator->GetCurrentFrameIndex());
	ASSERT_EQ(1, pSequenceAnimator->GetLoopHitCount());

	pSequenceAnimator->AddTime(-1.0);
	ASSERT_EQ(3, pSequenceAnimator->GetCurrentFrameIndex());
	ASSERT_EQ(0, pSequenceAnimator->GetLoopHitCount());

	pSequenceAnimator->AddTime(1.0);
	ASSERT_EQ(1, pSequenceAnimator->GetCurrentFrameIndex());
	ASSERT_EQ(1, pSequenceAnimator->GetLoopHitCount());

	pSequenceAnimator->SetTime(0.0);
	ASSERT_EQ(0, pSequenceAnimator->GetCurrentFrameIndex());
	ASSERT_EQ(0, pSequenceAnimator->GetLoopHitCount());
	ASSERT_EQ(false, pSequenceAnimator->IsAnimationFinish());
	ASSERT_EQ(0.0, pSequenceAnimator->GetTime());

	ASSERT_THROW(pSequenceAnimator->AddTime(-0.1), std::invalid_argument);
}

