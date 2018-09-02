#pragma once
#include <XeSDK/IObject.h>

namespace Xe { namespace Game { namespace Component {
	interface IComponent : public IObject {
	public:
		virtual void OnUpdate(float update) = 0;
		virtual void OnDraw() = 0;
	};
} } }