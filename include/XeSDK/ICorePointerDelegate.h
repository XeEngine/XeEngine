#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/IObject.h>
#include <XeSDK/IDelegate.h>
#include <XeSDK/XeMathRectangle.h>
#include <XeSDK/XeTimer.h>

namespace Xe { namespace Core {

	enum PointerType
	{
		PointerType_Mouse,
		PointerType_Touch,
		PointerType_Pen,
	};

	enum PointerAction
	{
		PointerAction_Moved,
		PointerAction_Pressed,
		PointerAction_Released,
	};

	enum PointerButton
	{
		PointerButton_None = 0,
		PointerButton_Left = 1,
		PointerButton_Right = 2,
		PointerButton_Center = 4,
		PointerButton_XButton1 = 8,
		PointerButton_XButton2 = 16,
	};

	struct PointerDevice
	{
		//! \brief What kind of device is this
		PointerType Type;

		//! \brief Rectangle where the pointer is operating
		Math::Rectangle<svar> Rectangle;

		//! \brief Check if device supports 3D-touch
		bool Is3dSupported;
	};

	struct Pointer
	{
		//! \brief Check if current pointer data is processed.
		/** \details Check if the pointer is calculated or not.
		* An example of this is on a multi-touch device, where the
		* Nth point is touched or not. If false, the rest of the
		* content can be un-initialized; in this case please do not
		* rely on other attributes.
		*/
		bool IsProcessed;

		//! \brief Details about the device that created this pointer.
		PointerDevice Device;

		//! \brief Specify the last action of current pointer.
		PointerAction Action;

		//! \brief Specify if pointer 
		//! \brief Position of the pointer through the screen.
		/** \details This is the raw position of the pointer,
		* specified in pixel and following the size of an IContext.
		*/
		Math::Point2<svar> Position;

		//! \brief Buttons pressed from current pointer.
		//! \details This is a combination between PointerButton enum values.
		uvar Buttons;

		//! \brief Check the level of pointer's pressure to the screen.
		/** \details This is useful for devices that supports 3D touch.
		* 0.0 indicate that device is not over the screen device; this
		* usually happens with a mouse. Between 0 and 1 (not included)
		* means how the device is near to the screen. This is used
		* for example from the experimental Microsoft 3D touch.
		* A value of >= 1.0 means a contact with the screen and, with
		* values greater than 1, means how pressure is applied to the
		* screen. This is useful for iPhone 6S 3D touch or pens.
		*/
		float Pressure;

		//! \brief When the event occured.
		Timer Time;

		//! \brief Get normalized position [0, 1] of the pointer.
		Math::Point2<float> GetPositionNormalized() const
		{
			return Device.Rectangle.Normalize(Position);
		}

		//! \brief Get elapsed time in seconds of how been the pointer is processed.
		double GetElapsedTime() const
		{
			return Timer(Timer::Current() - Time).AsDouble();
		}
	};

	//! \brief Describe an event pointer
	struct PointerEventArgs
	{
		//! \brief Specify the pointer of the event
		Pointer CurrentPointer;
	};

	struct PointerMovedEventArgs : public PointerEventArgs {};
	struct PointerPressedEventArgs : public PointerEventArgs {};
	struct PointerReleasedEventArgs : public PointerEventArgs {};
	struct PointerEnterEventArgs : public PointerEventArgs {};
	struct PointerLeaveEventArgs : public PointerEventArgs {};
	struct PointerWheelEventArgs : public Math::Vector2<float> {};

	interface IPointerEventHandler :
		public IObject,
		public IDelegate<const PointerMovedEventArgs&>,
		public IDelegate<const PointerPressedEventArgs&>,
		public IDelegate<const PointerReleasedEventArgs&>,
		public IDelegate<const PointerEnterEventArgs&>,
		public IDelegate<const PointerLeaveEventArgs&>,
		public IDelegate<const PointerWheelEventArgs&>
	{ };
} }


// DEPRECATED - Compatible layer

namespace Xe { namespace IO {
	enum PointerType
	{
		PointerType_Mouse,
		PointerType_Touch,
		PointerType_Pen,
	};

	enum PointerAction
	{
		PointerAction_Moved,
		PointerAction_Pressed,
		PointerAction_Released,
	};

	enum PointerButton
	{
		PointerButton_None = 0,
		PointerButton_Left = 1,
		PointerButton_Right = 2,
		PointerButton_Center = 4,
		PointerButton_XButton1 = 8,
		PointerButton_XButton2 = 16,
	};

	//! \warning DEPRECATED
	struct PointerEvent : public Xe::Core::PointerEventArgs {};
} }

namespace Xe { namespace Core {
	//! \warning DEPRECATED
	interface IPointerHandler : public virtual IObject
	{
		virtual void OnPointerMoved(const IO::PointerEvent& e) = 0;
		virtual void OnPointerPressed(const IO::PointerEvent& e) = 0;
		virtual void OnPointerReleased(const IO::PointerEvent& e) = 0;
		virtual void OnPointerEnter(const IO::PointerEvent& e) = 0;
		virtual void OnPointerLeave(const IO::PointerEvent& e) = 0;
		virtual void OnPointerWheel(const Math::Vector2<float>& wheel) = 0;
	};
} }