#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Game {
		namespace Component {
			class IComponent : public IObject {
			public:
				static const UID ID = 0x3f067fb3a7df47d8ULL;

				virtual void OnUpdate(float update) = 0;
				virtual void OnDraw() = 0;
			};
		}
	}
}