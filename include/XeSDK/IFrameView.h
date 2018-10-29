#pragma once
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeObjPtr.h>
#include <XeSDK/ICoreApplicationEventHandler.h>
#include <XeSDK/ICoreFrameEventHandler.h>
#include <XeSDK/ICoreKeyboardEventHandler.h>
#include <XeSDK/ICorePointerDelegate.h>

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

	//! \internal
	interface IFrameView : public IObject
	{
		virtual void SetApplicationEventHandler(Xe::Core::IApplicationEventHandler*) = 0;
		virtual void SetKeyboardEventHandler(Xe::Core::IKeyboardEventHandler*) = 0;
		virtual void SetPointerEventHandler(Xe::Core::IPointerEventHandler*) = 0;

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

		// Compatibility layer. To remove on 0.10.x
		Xe::Core::IApplicationEventHandler* m_TmpApplicationEventHandler = nullptr;
		Xe::Core::IKeyboardEventHandler* m_TmpKeyboardEventHandler = nullptr;
		Xe::Core::IPointerEventHandler* m_TmpPointerEventHandler = nullptr;

		//! \warning DEPRECATED
		void SetApplicationHandler(Xe::Core::IApplicationHandler* pApplicationHandler)
		{
			struct ApplicationEventHandler : public IApplicationEventHandler
			{
				ObjPtr<Xe::Core::IApplicationHandler> m_Handler;

				ApplicationEventHandler(Xe::Core::IApplicationHandler* pApplicationHandler) :
					m_Handler(pApplicationHandler)
				{}

				void operator()(InitializeEventArgs& args) { args.Succeeded = m_Handler->OnInitialize(); }
				void operator()(const DestroyEventArgs& args) { m_Handler->OnDestroy(); }
				void operator()(const SuspendEventArgs& args) { m_Handler->OnDestroy(); }
				void operator()(const ResumeEventArgs& args) { m_Handler->OnDestroy(); }
				void operator()(const RunEventArgs& args) { m_Handler->OnDestroy(); }
				void operator()(const DrawEventArgs& args) { m_Handler->OnDestroy(); }
				void operator()(const DeviceChangedEventArgs& args) { m_Handler->OnDestroy(); }
			};

			SetApplicationEventHandler(pApplicationHandler ? new ApplicationEventHandler(pApplicationHandler) : nullptr);
		}

		//! \warning DEPRECATED
		void SetKeyboardHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler)
		{
			struct KeyboardEventHandler : public IKeyboardEventHandler
			{
				ObjPtr<Xe::Core::IKeyboardHandler> m_Handler;

				KeyboardEventHandler(Xe::Core::IKeyboardHandler* pKeyboardHandler) :
					m_Handler(pKeyboardHandler)
				{}

				void operator()(const CharPressedEventArgs& args) { m_Handler->OnCharacter(args); }
				void operator()(const KeyPressedEventArgs& args) { m_Handler->OnKeyPressed(args); }
				void operator()(const KeyReleasedEventArgs& args) { m_Handler->OnKeyReleased(args); }
			};

			if (m_TmpKeyboardEventHandler)
			{
				*this -= m_TmpKeyboardEventHandler;
				delete m_TmpKeyboardEventHandler;
			}

			if (pKeyboardHandler)
			{
				m_TmpKeyboardEventHandler = new KeyboardEventHandler(pKeyboardHandler);
				*this += m_TmpKeyboardEventHandler;
			}
			else
			{
				m_TmpKeyboardEventHandler = nullptr;
			}
		}

		//! \warning DEPRECATED
		void SetPointerHandler(Xe::Core::IPointerHandler* pPointerHandler)
		{
			struct PointerEventHandler : public IPointerEventHandler
			{
				ObjPtr<Xe::Core::IPointerHandler> m_Handler;

				PointerEventHandler(Xe::Core::IPointerHandler* pPointerHandler) :
					m_Handler(pPointerHandler)
				{}

				void operator()(const PointerMovedEventArgs& args) { m_Handler->OnPointerMoved(args); }
				void operator()(const PointerPressedEventArgs& args) { m_Handler->OnPointerPressed(args); }
				void operator()(const PointerReleasedEventArgs& args) { m_Handler->OnPointerReleased(args); }
				void operator()(const PointerEnterEventArgs& args) { m_Handler->OnPointerEnter(args); }
				void operator()(const PointerLeaveEventArgs& args) { m_Handler->OnPointerLeave(args); }
				void operator()(const PointerWheelEventArgs& args) { m_Handler->OnPointerWheel(args); }
			};

			if (m_TmpPointerEventHandler)
			{
				*this -= m_TmpPointerEventHandler;
				delete m_TmpPointerEventHandler;
			}

			if (pPointerHandler)
			{
				m_TmpPointerEventHandler = new PointerEventHandler(pPointerHandler);
				*this += m_TmpPointerEventHandler;
			}
			else
			{
				m_TmpPointerEventHandler = nullptr;
			}
		}
	};
} }