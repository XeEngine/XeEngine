#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeIOPointer.h>

namespace Xe {
	interface IApplication : public IObject {
	public:
		struct Key {
			uint key;
		};

		virtual ~IApplication() {}

		virtual bool OnInitialize() = 0;
		virtual void OnDestroy() = 0;
		virtual bool OnUpdate(double deltaTime) = 0;
		virtual void OnDraw() = 0;

		virtual void OnKeybDown(const Key &key) = 0;
		virtual void OnKeybUp(const Key &key) = 0;
		virtual void OnKeybChar(const Key &key, uint ch) = 0;

		virtual void OnPointerMove(const IO::PointerEvent& e) = 0;
		virtual void OnPointerPressed(const IO::PointerEvent& e) = 0;
		virtual void OnPointerReleased(const IO::PointerEvent& e) = 0;
		virtual void OnPointerEnter() = 0;
		virtual void OnPointerLeave() = 0;
		virtual void OnPointerWheel(const Math::Vector2<float>& wheel) = 0;

		virtual void OnSizeChanged(const Math::Vector2<svar> &size) = 0;
		virtual void OnLostFocus() = 0;
		virtual void OnGotFocus() = 0;
		virtual void OnSuspend() = 0;
		virtual void OnResume() = 0;
		virtual bool OnClosing(bool forced) = 0;
	};
}