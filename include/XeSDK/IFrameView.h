#pragma once
#include <XeSDK/XeGraphics.h>

namespace Xe { namespace Core {

	interface IApplicationHandler;
	interface IFrameHandler;
	interface IKeyboardHandler;
	interface IPointerHandler;

	//! \brief Specify how to control the event dispatcher of the OS.
	/** \details When OnUpdate event finish the control is passed to the
	* operating system, that has the task to process all the events in
	* queue like window management, pointer and keyboard. After the OS
	* event dispatcher the control returns to the app, calling OnDraw
	* and OnUpdate when needed. This enum helps to decide how to manage
	* the operating system control.
	* To manually dispatch an event, see Graphics::IContext::Process.
	*/
	enum DispatchType
	{
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

	//! \brief Type of orientation.
	enum Orientation
	{
		//! \brief Not specified or not supported by current device.
		Orientation_Unknown,
		Orientation_Landscape,
		Orientation_Portrait,
		Orientation_LandscapeFlipped,
		Orientation_PortraitFlipped,
	};

	//! \internal
	interface IFrameView : public IObject
	{
		virtual void SetApplicationHandler(Xe::Core::IApplicationHandler* pApplicationHandler) = 0;
		virtual void SetKeyboardHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler) = 0;
		virtual void SetPointerHandler(Xe::Core::IPointerHandler* pPointerHandler) = 0;

		//! \brief Process and remove from the event queue the events.
		virtual bool DispatchEvents(DispatchType type) = 0;

		//! \brief Set a new title
		virtual void SetTitle(const String& title) = 0;

		//! \brief Get the current size
		virtual Xe::Graphics::Size GetSize() const = 0;

		//! \brief Specify a new size, when possible.
		//! \return false if param is invalid or device does not support it.
		virtual bool SetSize(const Xe::Graphics::Size& size) = 0;

		//! \brief Enable the fullscreen mode, if supported.
		//! \param[in] fullScreen set or reset the fullscreen.
		virtual void SetFullScreen(bool fullScreen) = 0;

		//! \brief Get current orientation of device.
		virtual Xe::Core::Orientation GetOrientation() const = 0;

		//! \brief Set a new orientation, when possible.
		virtual void SetPreferredOrientation(Xe::Core::Orientation orientation) = 0;

		//! \brief Get a scale value to adapt output to physical device size.
		virtual float GetScale() const = 0;

		//! \brief Get a pointer of window system's object, used for internal stuff.
		virtual void* GetSystemWindow() const = 0;
	};
} }