#pragma once
#include <XeSDK/XeMathRectangle.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/IIOStream.h>

namespace Xe { namespace Game { namespace Animations {
	struct Frame
	{
		Xe::Math::Rectangle<u16> Rect;
		Xe::Math::Vector2<s16> Pivot;
		u16 Duration;
		u16 Reference;
	};

	Frame& ParseJson(Frame& frame, Xe::IO::IStream* pStream);
	const Frame& SaveJson(const Frame& frame, Xe::IO::IStream* pStream);
} } }