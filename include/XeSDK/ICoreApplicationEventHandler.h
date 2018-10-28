#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/IObject.h>
#include <XeSDK/IDelegate.h>

namespace Xe { namespace Core {

	//! \brief This is called before any other event, useful for initialization.
	struct InitializeEventArgs
	{
		//! \brief OUT Check if the initialization has been completed with success.
		bool Succeeded;
	};

	//! \brief Revert the initialization, freeing allocated stuff.
	struct DestroyEventArgs
	{

	};

	//! \brief When the OS needs to save the state of the app before to close it.
	struct SuspendEventArgs
	{

	};

	//! \brief When the OS can open and resume the state of the app.
	struct ResumeEventArgs
	{

	};

	//! \brief Here lives the main application.
	struct RunEventArgs
	{

	};

	//! \brief When the application needs a re-draw of screen content.
	struct DrawEventArgs
	{

	};

	//! \brief Fired when a device has been attached or de-attached.
	struct DeviceChangedEventArgs
	{

	};

	interface IApplicationEventHandler :
		public IObject,
		public IDelegate<InitializeEventArgs&>,
		public IDelegate<const DestroyEventArgs&>,
		public IDelegate<const SuspendEventArgs&>,
		public IDelegate<const ResumeEventArgs&>,
		public IDelegate<const RunEventArgs&>,
		public IDelegate<const DrawEventArgs&>,
		public IDelegate<const DeviceChangedEventArgs&>
	{ };


	//! \warning DEPRECATED
	struct DeviceArgs
	{

	};

	//! \warning DEPRECATED
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
} }