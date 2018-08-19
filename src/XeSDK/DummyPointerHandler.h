#pragma once
#include <XeSDK/ICoreView.h>

namespace Xe { namespace Core {
	class DummyPointerHandler : public IPointerHandler
	{
		void OnPointerMoved(const IO::PointerEvent& e) { }
		void OnPointerPressed(const IO::PointerEvent& e) { }
		void OnPointerReleased(const IO::PointerEvent& e) { }
		void OnPointerEnter(const IO::PointerEvent& e) { }
		void OnPointerLeave(const IO::PointerEvent& e) { }
		void OnPointerWheel(const Math::Vector2<float>& wheel) { }
	};
} }