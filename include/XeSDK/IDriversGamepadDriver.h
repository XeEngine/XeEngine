#pragma once
#include <XeSDK/IDriversDriver.h>
#include <XeSDK/XeIOGamepad.h>

namespace Xe { namespace Drivers {
	enum InputDeviceType
	{
		InputDeviceType_Gamepad
	};

	interface IGamepadEventHandler : public IObject
	{
		virtual void OnGamepadAttached(const Xe::IO::GamepadEntry& entry) = 0;

		virtual void OnGamepadDeattached(const Xe::IO::GamepadEntry& entry) = 0;
	};

	//! \brief Contract for Input Drivers
	interface IInputDriver : public IDriver
	{
		//! \brief Get the device type which the driver is referring on.
		virtual InputDeviceType GetInputDeviceType() const = 0;
	};

	//! \brief Contract for a Gamepad Input Driver.
	interface IGamepadDriver : public IInputDriver
	{
		//! \brief Open a specific device.
		//! \param[out] ppGamepad The gamepad object
		//! \param[in] entry Gamepad entry returned from the specific driver's enumerator.
		//! \return True if the device was opened with success, else false.
		virtual bool OpenDevice(
			Xe::IO::Gamepad** ppGamepad,
			const Xe::IO::GamepadEntry& entry) = 0;

		//! \brief Returns a list of connected devices.
		//! \param[out] pEntries Where the list of entries should be stored.
		//! \param[in] index Starting index of pEntries.
		//! \param[in] count Maximum numbers of entries to retrieve.
		//! \return Number of devices written to pEntries.
		virtual size_t EnumerateDevices(
			Xe::IO::GamepadEntry* pEntries,
			size_t index,
			size_t count) = 0;

		//virtual void SetGamepadEventHandler(
		//	IGamepadEventHandler* pGamepadEventHandler) = 0;
	};
} }