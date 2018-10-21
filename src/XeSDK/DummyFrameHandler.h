#pragma once
#include <XeSDK/ICoreView.h>

namespace Xe { namespace Core {
	class DummyFrameHandler : public IFrameHandler
	{
		bool OnClosing(bool forced) { return true; }
		void OnFocusGot() { }
		void OnFocusLost() { }
		void OnVisibilityChanged(bool visibility) { }
		void OnSizeChanged(const Xe::Graphics::Size& size) { }
		void OnOrientationChanged(Xe::Core::Orientation orientation) { }
		void OnDpiChanged(float dpi) { }
	};
} }