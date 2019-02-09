#pragma once

namespace Xe { namespace Game { namespace Animations {
	interface IAnimationGroup;
	struct AnimationDocument;

	bool Factory(IAnimationGroup** ppAnimationGroup, const AnimationDocument& document);
} } }