#include "pch.h"
#include "XboxGamepad.h"

namespace Xe { namespace IO {
	XboxGamepad::XboxGamepad(
		const GamepadEntry& entry,
		const ComPtr<IController>& controller) :
		m_IsAvailable(true)
	{
		m_Controller = controller;
		HR(m_Controller.As(&m_Controller2));
		HR(m_Controller.As(&m_Gamepad));
	}

	XboxGamepad::~XboxGamepad()
	{

	}

	bool XboxGamepad::IsAvailable() const
	{
		return m_IsAvailable;
	}

	void XboxGamepad::Update()
	{
		RawGamepadReading reading;
		HRESULT hr = m_Gamepad->GetRawCurrentReading(&reading);
		if (hr == S_OK)
		{
			LAxis.x = reading.LeftThumbstickX;
			LAxis.y = -reading.LeftThumbstickY;
			LAxis.z = reading.LeftTrigger;
			RAxis.x = reading.RightThumbstickX;
			RAxis.y = -reading.RightThumbstickY;
			RAxis.z = reading.RightTrigger;

			PressedButtons.LUp = !!(reading.Buttons & GamepadButtons::GamepadButtons_DPadUp);
			PressedButtons.LDown = !!(reading.Buttons & GamepadButtons::GamepadButtons_DPadDown);
			PressedButtons.LLeft = !!(reading.Buttons & GamepadButtons::GamepadButtons_DPadLeft);
			PressedButtons.LRight = !!(reading.Buttons & GamepadButtons::GamepadButtons_DPadRight);
			PressedButtons.RUp = !!(reading.Buttons & GamepadButtons::GamepadButtons_Y);
			PressedButtons.RDown = !!(reading.Buttons & GamepadButtons::GamepadButtons_A);
			PressedButtons.RLeft = !!(reading.Buttons & GamepadButtons::GamepadButtons_X);
			PressedButtons.RRight = !!(reading.Buttons & GamepadButtons::GamepadButtons_B);
			PressedButtons.TriggerL1 = !!(reading.Buttons & GamepadButtons::GamepadButtons_LeftShoulder);
			PressedButtons.TriggerR1 = !!(reading.Buttons & GamepadButtons::GamepadButtons_RightShoulder);
			PressedButtons.TriggerL2 = reading.LeftTrigger > 0.75f;
			PressedButtons.TriggerR2 = reading.RightTrigger > 0.75f;
			PressedButtons.TriggerL3 = !!(reading.Buttons & GamepadButtons::GamepadButtons_LeftThumbstick);
			PressedButtons.TriggerR3 = !!(reading.Buttons & GamepadButtons::GamepadButtons_RightThumbstick);
			PressedButtons.Select = !!(reading.Buttons & GamepadButtons::GamepadButtons_View);
			PressedButtons.Start = !!(reading.Buttons & GamepadButtons::GamepadButtons_Menu);
			PressedButtons.Home = false;
			PressedButtons.Tap = false;
		}
		
		m_IsAvailable = false;
	}

	void XboxGamepad::SetRumbling(const Math::Vector2f& rumbling)
	{
		GamepadVibration vibration;
		vibration.LeftMotorLevel = rumbling.x;
		vibration.LeftTriggerLevel = rumbling.x;
		vibration.RightMotorLevel = rumbling.y;
		vibration.RightTriggerLevel = rumbling.y;
		m_Gamepad->SetVibration(vibration);
	}

	void XboxGamepad::SetLightbar(const Graphics::Color& color)
	{

	}
} }