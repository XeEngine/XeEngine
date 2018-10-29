#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/IObject.h>
#include <XeSDK/IDelegate.h>
#include <XeSDK/IFrameView.h>

namespace Xe { namespace Core {

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
} }

namespace Xe { namespace Graphics {
	//! \warning DEPRECATED
	typedef Xe::Core::Orientation Orientation;
} }

namespace Xe { namespace Core {
	struct AttachEventArgs
	{
		//! \brief Frame to attach
		IFrameView* FrameView;

		//! \brief Returns if the attach has been completed with success.
		bool Succeeded;
	};

	struct ClosingEventArgs
	{
		//! \brief If the close event is forced, the Frame will close even if CloseApproved is false.
		bool IsCloseForced;

		//! \brief Returns true if the frame can close, else if not. Ignored if IsCloseForced is true.
		bool CloseApproved;
	};

	struct FocusChangedEventArgs
	{
		//! \brief Check if the frame has currently the focus or not.
		bool IsFocused;
	};

	struct VisibilityChangedEventArgs
	{
		//! \brief Check if the frame is visible or not.
		bool IsVisible;
	};

	struct SizeChangedEventArgs
	{
		//! \brief New size of the frame content.
		Xe::Graphics::Size Size;

		//! \brief Scale based on DPI settings.
		float Scale;
	};

	struct OrientationChangedEventArgs
	{
		//! \brief New orientation of the frame.
		Xe::Core::Orientation Orientation;
	};

	interface IFrameEventHandler :
		public IObject,
		public IDelegate<AttachEventArgs&>,
		public IDelegate<ClosingEventArgs&>,
		public IDelegate<const FocusChangedEventArgs&>,
		public IDelegate<const VisibilityChangedEventArgs&>,
		public IDelegate<const SizeChangedEventArgs&>,
		public IDelegate<const OrientationChangedEventArgs&>
	{ };

	//! \warning DEPRECATED
	interface IFrameHandler : public virtual IObject
	{
		virtual bool OnAttach(IFrameView* pFrameView) = 0;
		virtual bool OnClosing(bool forced) = 0;
		virtual void OnFocusGot() = 0;
		virtual void OnFocusLost() = 0;
		virtual void OnVisibilityChanged(bool visibility) = 0;
		virtual void OnSizeChanged(const Graphics::Size& size) = 0;
		virtual void OnOrientationChanged(Graphics::Orientation orientation) = 0;
		virtual void OnDpiChanged(float dpi) = 0;
	};
} }