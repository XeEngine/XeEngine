#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Game { namespace Animations {
	struct Frame;
	interface ISequenceAnimator : IObject
	{
		virtual ~ISequenceAnimator() {}

		virtual const String& GetName() const = 0;

		virtual size_t GetLoopIndex() const = 0;

		virtual size_t GetFrameCount() const = 0;

		virtual const Frame& GetFrame(size_t index) const = 0;

		virtual size_t GetCurrentFrameIndex() const = 0;

		virtual const Frame& GetCurrentFrame() const = 0;

		virtual bool IsAnimationFinish() const = 0;

		virtual size_t GetLoopHitCount() const = 0;

		virtual double GetTime() const = 0;

		virtual void SetTime(double time) = 0;

		virtual void AddTime(double time) = 0;
	};
} } }