
#include "pch.h"
#include "XeDriverInputXInput.h"
#include <XeSDK/XeIOGamepad.h>

namespace Xe { namespace Driver { namespace Input {
	XInputLib g_XInput;

	struct GamepadXInput : public Xe::IO::Gamepad {
		bool m_IsAvailable;
		DWORD m_UserIndex;

		GamepadXInput(DWORD userIndex) :
			m_UserIndex(userIndex)
		{
			m_IsAvailable = true;
			m_UserIndex = 0;

			Capabilities.Data = 0;

			XINPUT_CAPABILITIES capabilities;
			ZeroMemory(&capabilities, sizeof(XINPUT_CAPABILITIES));

			DWORD result = g_XInput.GetCapabilities(m_UserIndex, XINPUT_FLAG_GAMEPAD, &capabilities);
			
			if (result == ERROR_SUCCESS) {
				Capabilities.Data = TranslateButtons(capabilities.Gamepad.wButtons);

				if (capabilities.Gamepad.bLeftTrigger > 0)
					Capabilities.TriggerL2 = Capabilities.LZ = true;
				if (capabilities.Gamepad.bRightTrigger > 0)
					Capabilities.TriggerR2 = Capabilities.RZ = true;
				if (capabilities.Gamepad.sThumbLX > 0)
					Capabilities.LX = true;
				if (capabilities.Gamepad.sThumbLY > 0)
					Capabilities.LY = true;
				if (capabilities.Gamepad.sThumbRX > 0)
					Capabilities.RX = true;
				if (capabilities.Gamepad.sThumbRY > 0)
					Capabilities.RY = true;
				if (capabilities.Vibration.wLeftMotorSpeed > 0)
					Capabilities.RumbleX = true;
				if (capabilities.Vibration.wRightMotorSpeed > 0)
					Capabilities.RumbleY = true;

				Capabilities.Battery = true;
				Capabilities.Audio = !!(capabilities.Flags & XINPUT_CAPS_VOICE_SUPPORTED);
				
				if (capabilities.Flags & XINPUT_CAPS_WIRELESS) {
					Capabilities.Battery = true;
					GetBatteryStatus();
				}
				else {
					Capabilities.Battery = false;
				}
			}
			else {
				Capabilities.Data = 0;
				Battery = 0.0f;
				IsBatteryCharging = false;
			}
		}

		virtual ~GamepadXInput() {

		}

		bool IsAvailable() const {
			return m_IsAvailable;
		}

		void Update() {
			if (!IsAvailable()) return;

			XINPUT_STATE state;
			switch (g_XInput.GetState(m_UserIndex, &state)) {
			case ERROR_SUCCESS:
				ProcessGamepad(state.Gamepad);
				if (Capabilities.Battery) {
					GetBatteryStatus();
				}
				break;
			case ERROR_DEVICE_NOT_CONNECTED:
				m_IsAvailable = false;
				break;
			}
		}

		void Flush() {
		}

		void SetRumbling(const Math::Vector2f& rumbling) {
			if (!IsAvailable()) return;

			XINPUT_VIBRATION vibration;
			vibration.wLeftMotorSpeed = (WORD)(rumbling.x * 65535.0f);
			vibration.wRightMotorSpeed = (WORD)(rumbling.y * 65535.0f);
			if (g_XInput.SetState(m_UserIndex, &vibration) ==
				ERROR_DEVICE_NOT_CONNECTED) {
				m_IsAvailable = false;
			}
		}

		void SetLightbar(const Graphics::Color& color) {
		}

		void ProcessGamepad(XINPUT_GAMEPAD& gamepad) {
			const int TriggerTreshold = 128;
			const float AnalogDivisor = 32767.0f;
			const float TriggerDivisor = 255.0f;

			PressedButtons.Data = TranslateButtons(gamepad.wButtons);

			if (gamepad.bLeftTrigger >= TriggerTreshold)
				PressedButtons.Data |= 1 << Xe::IO::Button_TriggerL2;
			if (gamepad.bRightTrigger >= TriggerTreshold)
				PressedButtons.Data |= 1 << Xe::IO::Button_TriggerR2;

			LAxis.x = (float)gamepad.sThumbLX / AnalogDivisor;
			LAxis.y = (float)gamepad.sThumbLY / -AnalogDivisor;
			LAxis.z = (float)gamepad.bLeftTrigger / TriggerDivisor;
			RAxis.x = (float)gamepad.sThumbRX / AnalogDivisor;
			RAxis.y = (float)gamepad.sThumbRY / -AnalogDivisor;
			RAxis.z = (float)gamepad.bRightTrigger / TriggerDivisor;
		}

		void GetBatteryStatus() {
			XINPUT_BATTERY_INFORMATION batteryInfo;

			g_XInput.GetBatteryInformation(m_UserIndex, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);

			switch (batteryInfo.BatteryLevel) {
			case BATTERY_LEVEL_EMPTY:
				Battery = 0.25f;
				break;
			case BATTERY_LEVEL_LOW:
				Battery = 0.5f;
				break;
			case BATTERY_LEVEL_MEDIUM:
				Battery = 0.75f;
				break;
			case BATTERY_LEVEL_FULL:
				Battery = 1.0f;
				break;
			}

			switch (batteryInfo.BatteryType) {
			case BATTERY_TYPE_WIRED:
				IsBatteryCharging = true;
				break;
			case BATTERY_TYPE_ALKALINE:
			case BATTERY_TYPE_NIMH:
				IsBatteryCharging = false;
				break;
			}
		}

		u32 TranslateButton(WORD id) {
			switch (id) {
			case XINPUT_GAMEPAD_DPAD_UP: return 0;
			case XINPUT_GAMEPAD_DPAD_DOWN: return 1;
			case XINPUT_GAMEPAD_DPAD_LEFT: return 2;
			case XINPUT_GAMEPAD_DPAD_RIGHT: return 3;
			case XINPUT_GAMEPAD_START: return 15;
			case XINPUT_GAMEPAD_BACK: return 14;
			case XINPUT_GAMEPAD_LEFT_THUMB: return 12;
			case XINPUT_GAMEPAD_RIGHT_THUMB: return 13;
			case XINPUT_GAMEPAD_LEFT_SHOULDER: return 8;
			case XINPUT_GAMEPAD_RIGHT_SHOULDER: return 9;
				//case 0x400: return 10;
				//case 0x800: return 11;
			case XINPUT_GAMEPAD_A: return 5;
			case XINPUT_GAMEPAD_B: return 7;
			case XINPUT_GAMEPAD_X: return 6;
			case XINPUT_GAMEPAD_Y: return 4;
			}
		}
		
		u32 TranslateButtons(WORD id) {
			u32 data = 0;

			if (id & XINPUT_GAMEPAD_DPAD_UP) data &= 1 << 0;
			if (id & XINPUT_GAMEPAD_DPAD_DOWN) data &= 1 << 1;
			if (id & XINPUT_GAMEPAD_DPAD_LEFT) data &= 1 << 2;
			if (id & XINPUT_GAMEPAD_DPAD_RIGHT) data &= 1 << 3;

			if (id & XINPUT_GAMEPAD_START) data &= 1 << 15;
			if (id & XINPUT_GAMEPAD_BACK) data &= 1 << 14;

			if (id & XINPUT_GAMEPAD_LEFT_THUMB) data &= 1 << 12;
			if (id & XINPUT_GAMEPAD_RIGHT_THUMB) data &= 1 << 13;
			if (id & XINPUT_GAMEPAD_LEFT_SHOULDER) data &= 1 << 8;
			if (id & XINPUT_GAMEPAD_RIGHT_SHOULDER) data &= 1 << 9;

			if (id & XINPUT_GAMEPAD_A) data &= 1 << 5;
			if (id & XINPUT_GAMEPAD_B) data &= 1 << 7;
			if (id & XINPUT_GAMEPAD_X) data &= 1 << 6;
			if (id & XINPUT_GAMEPAD_Y) data &= 1 << 4;

			return data;
		}
	};

	const Xe::IO::GamepadEntry g_XInputPads[XUSER_MAX_COUNT]{
		0x629f93ec2a5a,
		0x49d611ec218f,
		0x0a0638223f37,
		0x2d91e20ad909
	};

	bool XInput::OpenDevice(
		Xe::IO::Gamepad ** ppGamepad,
		const Xe::IO::GamepadEntry & entry) {

		DWORD index = -1;

		for (size_t i = 0; i < lengthof(g_XInputPads); i++) {
			if (g_XInputPads[i] == entry) {
				index = i;
				break;
			}
		}

		if (index < 0) {
			*ppGamepad = nullptr;
			return false;
		}

		*ppGamepad = new GamepadXInput(index);
		return true;
	}

	size_t XInput::EnumerateDevices(
		Xe::IO::GamepadEntry* pEntries,
		size_t index,
		size_t count) {
		size_t found = 0;

		for (DWORD i = 0;
			index + found < count && i < lengthof(g_XInputPads);
			i++) {
			XINPUT_CAPABILITIES capabilities;
			ZeroMemory(&capabilities, sizeof(XINPUT_CAPABILITIES));

			if (g_XInput.GetCapabilities(i, XINPUT_FLAG_GAMEPAD, &capabilities) == ERROR_SUCCESS) {
				pEntries[index + found++] = g_XInputPads[i];
			}
		}

		return found;
	}
} } }