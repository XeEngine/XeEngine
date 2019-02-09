#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeString.h>
#include <list>

namespace Xe { namespace Game { namespace Animations {
	struct ISequenceAnimator;
	struct FrameSequence;
	interface IAnimationGroup
	{
		virtual ~IAnimationGroup() {}

		virtual const String& GetName() const = 0;

		virtual std::vector<String> GetSequenceNames() const = 0;

		virtual bool IsSequenceExists(const StringSpan& sequenceName) const = 0;

		virtual bool TryGetSequence(const FrameSequence** ppFrameSequence, const StringSpan& sequenceName) const = 0;

		virtual bool TryCreateSequenceAnimator(ISequenceAnimator** ppSequenceAnimator, const StringSpan& sequenceName) const = 0;
	};
} } }