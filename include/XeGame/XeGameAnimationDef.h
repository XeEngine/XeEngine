#pragma once
#include <XeSDK/XeMathRectangle.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/IIOStream.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeJson.h>

namespace Xe { namespace Game { namespace Animations {
	struct Frame
	{
		Xe::Math::Rectangle<u16> Rect;
		Xe::Math::Vector2<s16> Pivot;
		u16 Duration;
		u16 Reference;
	};

	struct FrameSequence
	{
		Xe::String Name;
		std::list<Frame> Frames;
		u16 Speed;
		u16 Loop;
		u32 Flags;
	};

	struct SpriteSheet
	{
		String Path;
	};

	struct AnimationDocument
	{
		String Name;
		std::list<FrameSequence> Sequences;
		std::list<SpriteSheet> SpriteSheets;
	};

	Frame& ParseJson(Frame& frame, Xe::IO::IStream* pStream);
	const Frame& SaveJson(const Frame& frame, Xe::IO::IStream* pStream);

	FrameSequence& ParseJson(FrameSequence& sequence, Xe::IO::IStream* pStream);
	const FrameSequence& SaveJson(const FrameSequence& sequence, Xe::IO::IStream* pStream);

	SpriteSheet& ParseJson(SpriteSheet& sequence, Xe::IO::IStream* pStream);
	const SpriteSheet& SaveJson(const SpriteSheet& sequence, Xe::IO::IStream* pStream);

	void ParseJson(AnimationDocument& obj, const rapidjson::Value& value);
	void SaveJson(const AnimationDocument& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator);
	AnimationDocument& ParseJson(AnimationDocument& frame, Xe::IO::IStream* pStream);
	const AnimationDocument& SaveJson(const AnimationDocument& frame, Xe::IO::IStream* pStream);
} } }