#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeIOPointer.h>
#include <XeSDK/IFrameView.h>

namespace Xe {
	namespace IO {
		//! \brief Unique identifier for a keyboard button.
		typedef u32 ScanKey;
		//! \brief Translation from a scancode through globalization keyboard.
		typedef u32 VirtualKey;

		//! \brief Describe the event for a keyboard button.
		struct KeyboardEvent {
			//! \brief The physical ID of the key.
			ScanKey ScanCode;
			//! \brief Translated physical key as keyboard language describes.
			VirtualKey VirtualCode;
		};

		//! \brief Describe the event for a character.
		struct CharacterEvent {
			//! \brief The physical ID of the key.
			ScanKey ScanCode;
			//! \brief Character processed
			u32 Character;
		};
	}

	namespace Core
	{
		struct DeviceArgs
		{

		};

		interface IApplicationHandler : public virtual IObject
		{
			//! \brief This is called before any other event.
			//! \return false if initialization was unsuccessful.
			virtual bool OnInitialize() = 0;

			//! \brief Revert the initialization, freeing allocated stuff.
			virtual void OnDestroy() = 0;

			//! \brief When the OS needs to save the state of the app before to close it.
			virtual void OnSuspend() = 0;

			//! \brief When the OS can open and resume the state of the app.
			virtual void OnResume() = 0;

			//! \brief Here lives the main application.
			virtual void OnRun() = 0;

			//! \brief When the application needs a re-draw of screen content.
			virtual void OnDraw() = 0;

			//! \brief Fired when a device has been attached or de-attached.
			virtual void OnDevice(const DeviceArgs& args) = 0;
		};

		interface IFrameHandler : public virtual IObject
		{
			virtual bool OnAttach(IFrameView* pFrameView) = 0;
			virtual bool OnClosing(bool forced) = 0;
			virtual void OnFocusGot() = 0;
			virtual void OnFocusLost() = 0;
			virtual void OnVisibilityChanged(bool visibility) = 0;
			virtual void OnSizeChanged(const Graphics::Size& size) = 0;
			virtual void OnOrientationChanged(Xe::Core::Orientation orientation) = 0;
			virtual void OnDpiChanged(float dpi) = 0;
		};

		interface IKeyboardHandler : public virtual IObject
		{
			virtual void OnCharacter(const IO::CharacterEvent& e) = 0;
			virtual void OnKeyPressed(const IO::KeyboardEvent& e) = 0;
			virtual void OnKeyReleased(const IO::KeyboardEvent& e) = 0;
		};

		interface IPointerHandler : public virtual IObject
		{
			virtual void OnPointerMoved(const IO::PointerEvent& e) = 0;
			virtual void OnPointerPressed(const IO::PointerEvent& e) = 0;
			virtual void OnPointerReleased(const IO::PointerEvent& e) = 0;
			virtual void OnPointerEnter(const IO::PointerEvent& e) = 0;
			virtual void OnPointerLeave(const IO::PointerEvent& e) = 0;
			virtual void OnPointerWheel(const Math::Vector2<float>& wheel) = 0;
		};
	}
}