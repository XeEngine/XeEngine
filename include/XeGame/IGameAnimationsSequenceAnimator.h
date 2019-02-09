#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Game { namespace Animations {
	struct Frame;
	interface ISequenceAnimator
	{
		virtual ~ISequenceAnimator() {}

		virtual const String& GetName() const = 0;

		virtual size_t GetLoopIndex() const = 0;

		virtual size_t GetFrameCount() const = 0;

		virtual const Frame& GetFrame(size_t index) const = 0;


	};
} } }