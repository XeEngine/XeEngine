#pragma once
#include "pch.h"
#include <XeSDK/XeMath.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeIODevices.h>
#include <cassert>
#include <Windows.h>

#include "XeDriverInputWindowsCustom.h"
#include "Dualshock4.h"
#include "Dualshock4v2.h"
#include "Dualshock4bt.h"

#define VENDOR(a) (a[0])
#define PRODUCTS(a) (a + 1)
#define PRODUCTSCOUNT(a) (lengthof(a) - 1)
#define GETHIDS(a) IO::GetHidDevices(VENDOR(a), PRODUCTSCOUNT(a), PRODUCTS(a))

#define VID_SONY 0x054C
#define VID_SONY_DS3 0x0268
#define VID_SONY_DS4 0x05C4
#define VID_SONY_DS4V2 0x09CC
#define VID_SONY_DS4W 0x0BA0

namespace Xe { namespace Drivers { namespace Input
{
	struct InternalGamepad
	{
		Xe::IO::HidDevice HidDevice;
		u64 Hash;
	};

	size_t g_GamepadsCount = 0;
	InternalGamepad g_Gamepads[16];

	const char* WindowsCustom::GetDriverName() const { return "WindowsCustom"; }
	DriverType WindowsCustom::GetDriverType() const { return DriverType_Input; }

	// Inherited from IInputDriver
	InputDeviceType WindowsCustom::GetInputDeviceType() const { return InputDeviceType_Gamepad; }

	// Inherited from IGamepadDriver
	bool WindowsCustom::OpenDevice(
		Xe::IO::Gamepad** ppGamepad,
		const Xe::IO::GamepadEntry& entry)
	{
		for (size_t i = 0; i < g_GamepadsCount; i++) {
			if (g_Gamepads[i].Hash == entry) {
				auto& gamepad = g_Gamepads[i];

				if (gamepad.HidDevice.VendorId == VID_SONY) {
					switch (gamepad.HidDevice.ProductId) {
					//case VID_SONY_DS3:
					//	*ppGamepad = new GamepadDS3(gamePad);
					//	return true;
					case VID_SONY_DS4:
						*ppGamepad = new GamepadDS4(gamepad.HidDevice);
						return true;
					case VID_SONY_DS4V2:
						*ppGamepad = new GamepadDS4v2(gamepad.HidDevice);
						return true;
					//case VID_SONY_DS4W:
					//	*ppGamepad = new GamepadDS4Wireless(gamePad);
					//	return true;
					default:
						LOGE("Gamepad %s not supported.", gamepad.HidDevice.Path);
						break;
					}
				}
			}
		}

		return false;
	}

	size_t WindowsCustom::EnumerateDevices(
		Xe::IO::GamepadEntry* pEntries,
		size_t index,
		size_t count)
	{
		static const u16 SONY_GAMEPADS[] {
			VID_SONY,		// VID
			//VID_SONY_DS3,	// PID Dualshock 3
			VID_SONY_DS4,	// PID Dualshock 4
			VID_SONY_DS4V2,	// PID Dualshock 4 gen. 2
			//VID_SONY_DS4W,	// PID Dualshock 4 wireless
		};

		auto controllers = GETHIDS(SONY_GAMEPADS);
		int padIndex = 0;
		for (auto it = controllers.begin(); it != controllers.end(); it++) {
			InternalGamepad& internalGamepad = g_Gamepads[padIndex++];
			internalGamepad.HidDevice = *it;
			internalGamepad.Hash = String::GetHashFNV1a(internalGamepad.HidDevice.Path, String::GetLength(internalGamepad.HidDevice.Path));
		}
		g_GamepadsCount = padIndex;

		int found = Math::Min(count, g_GamepadsCount);
		for (int i = 0; i < found && index + i < count; i++) {
			pEntries[index + i] = g_Gamepads[index + i].Hash;
		}

		return found;
	}

} } }