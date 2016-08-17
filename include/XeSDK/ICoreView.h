#pragma once
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeIOPointer.h>

namespace Xe {
	namespace IO {
		//! \brief Unique identifier for a keyboard button.
		typedef u32 ScanCode;
		//! \brief Translation from a scancode through globalization keyboard.
		typedef u32 VirtualKey;

		//! \brief Describe the event for a keyboard button.
		struct KeyboardEvent {
			//! \brief The physical ID of the key.
			ScanCode ScanCode;
			//! \brief Translated physical key as keyboard language describes.
			VirtualKey VirtualCode;
		};

		//! \brief Describe the event for a character.
		struct CharacterEvent {
			//! \brief The physical ID of the key.
			ScanCode ScanCode;
			//! \brief Character processed
			u32 Character;
		};
	}
	namespace Graphics {
		//! \brief Type of orientation.
		enum Orientation {
			//! \brief Not specified or not supported by current device.
			Orientation_Unknown,
			Orientation_Landscape,
			Orientation_Portrait,
			Orientation_LandscapeFlipped,
			Orientation_PortraitFlipped,
		};
	}
	namespace Core {
		//! \brief Specify how to control the event dispatcher of the OS.
		/** \details When OnUpdate event finish the control is passed to the
		* operating system, that has the task to process all the events in
		* queue like window management, pointer and keyboard. After the OS
		* event dispatcher the control returns to the app, calling OnDraw
		* and OnUpdate when needed. This enum helps to decide how to manage
		* the operating system control.
		* To manually dispatch an event, see Graphics::IContext::Process.
		*/
		enum DispatchType {
			//! \brief Exit from the loop.
			/** \details Do not process any other event and exit immediately.
			* No other events than OnDestroy will be called.
			*/
			DispatchType_Exit,

			//! \brief Do not dispatch any event.
			/** \details This is the faster option, but can cause the OS or its
			* window to be froze. Because of this we reccomend to not dispatch
			* events in queue
			*/
			DispatchType_None,

			//! \brief Dispatch one event then return immediately.
			/** \details This is a good compromise between None and All.
			* If the events arrives faster than the deltaTime then the
			* latency to process them will be very high. It's reccomended to
			* not use this on heavy application or when VBlank is evaluated.
			*/
			DispatchType_One,

			//! \brief Dispatch all events then return immediately.
			/** \details This is the most common use of the event dispatcher.
			* There are no risk and the events processing latency is reduces
			* to the minimum.
			*/
			DispatchType_All,

			//! \brief Dispatch all events on queue then wait until a new one.
			/** \details This can be used to those application that do use of
			* rendering on demand. The update and drawing are called only when
			* needed, safing CPU clocks and improve power compsum.
			*/
			DispatchType_WaitOnce,

			//! \brief Dispatch every event until closure.
			/** \details This is useful when an application needs to
			* respond only to input events.
			*/
			DispatchType_WaitUntilQuit,
		};

		//! \internal
		class IFrameView {
		public:
			virtual bool DispatchEvents(DispatchType type) = 0;
			virtual void SetTitle(const String& title) = 0;
			virtual Graphics::Size GetSize() const = 0;
			virtual bool SetSize(const Graphics::Size& size) = 0;
			virtual Xe::Graphics::Orientation GetOrientation() const = 0;
			virtual void SetPreferredOrientation(Xe::Graphics::Orientation orientation) = 0;
			virtual float GetScale() const = 0;
		};

		//! \brief Offers a space for drawing and event handling.
		/** \details The IView is that kind of abstraction layer between an app
		 * and the window management and event handler of the operating system.
		 */
		class IView : public IObject {
		public:
			IFrameView *m_FrameView;
		public:
			//! \brief Used to define some properties of IView.
			struct InitProperties {
				//! \brief The title of the application, if available.
				String Title;
				//! \brief Initial size of window, if supported.
				Graphics::Size Size;
				//! \brief Set if window's context can change its size.
				//! \details In Windows Store builds this value is ignored.
				bool IsResizable;
				//! \brief Set if the application run in full-screen.
				bool IsFullscreen;

				InitProperties() :
					Title("XeEngine application"),
					Size(960, 540),
					IsResizable(false),
					IsFullscreen(false)
				{}
			};

			static const UID ID = 0xd3a5204caaad42eaULL;

			//! \brief Manage the life cycle of a view.
			/** \details When Run is called, OnInitialize is called. If it
			 * returns true, then OnRun is called too. Before to exit,
			 * OnDestroy is called to revert initialization state.
			 */
			static void Run(IView *pView, const InitProperties& properties = InitProperties());

			//! \brief Process and remove from the event queue the events.
			bool DispatchEvents(DispatchType type);

			//! \brief Set a new title
			void SetTitle(const String& title);

			//! \brief Get the current size
			Graphics::Size GetSize() const;

			//! \brief Specify a new size, when possible.
			//! \return false if param is invalid or device does not support it.
			bool SetSize(const Graphics::Size& size);

			//! \brief Get current orientation of device.
			Xe::Graphics::Orientation GetOrientation() const;

			//! \brief Set a new orientation, when possible.
			void SetPreferredOrientation(Xe::Graphics::Orientation orientation);

			//! \brief Get a scale value to adapt output to physical device size.
			float GetScale() const;

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


			virtual bool OnClosing(bool forced) = 0;
			virtual void OnFocusGot() = 0;
			virtual void OnFocusLost() = 0;
			virtual void OnVisibilityChanged(bool visibility) = 0;
			virtual void OnSizeChanged(const Graphics::Size& size) = 0;
			virtual void OnOrientationChanged(Graphics::Orientation orientation) = 0;
			virtual void OnDpiChanged(float dpi) = 0;

			virtual void OnCharacter(const IO::CharacterEvent& e) = 0;
			virtual void OnKeyPressed(const IO::KeyboardEvent& e) = 0;
			virtual void OnKeyReleased(const IO::KeyboardEvent& e) = 0;

			virtual void OnPointerMoved(const IO::PointerEvent& e) = 0;
			virtual void OnPointerPressed(const IO::PointerEvent& e) = 0;
			virtual void OnPointerReleased(const IO::PointerEvent& e) = 0;
			virtual void OnPointerEnter(const IO::PointerEvent& e) = 0;
			virtual void OnPointerLeave(const IO::PointerEvent& e) = 0;
			virtual void OnPointerWheel(const Math::Vector2<float>& wheel) = 0;
		};
	}
}