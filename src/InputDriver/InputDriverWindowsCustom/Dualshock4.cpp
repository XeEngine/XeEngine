#include "pch.h"
#include "Dualshock4.h"
#include <XeSDK/XeMemory.h>

namespace Xe {
	GamepadDS4::GamepadDS4(const Xe::IO::HidDevice& hidDevice) :
		Base(hidDevice)
	{
		LOGI("Found DualShock 4.\n");

		ReCreateBuffer(0x40, 0x20);

		Xe::Memory::Fill(m_BufferWrite, 0, m_BufferWriteLength);
		m_BufferWrite[0x00] = 0x05;
		m_BufferWrite[0x01] = 0xFF;
	}
	
	GamepadDS4::~GamepadDS4()
	{
	}

	void GamepadDS4::SetRumbling(const Math::Vector2f& rumbling)
	{
		m_BufferWrite[OffsetRumbleLeft] = ToU8(rumbling.y);
		m_BufferWrite[OffsetRumbleRight] = ToU8(rumbling.x);
		InvalidateWriteBuffer();
	}

	void GamepadDS4::SetLightbar(const Graphics::Color& Color)
	{
		m_BufferWrite[OffsetLightbarR] = ToU8(Color.r);
		m_BufferWrite[OffsetLightbarG] = ToU8(Color.g);
		m_BufferWrite[OffsetLightbarB] = ToU8(Color.b);
		InvalidateWriteBuffer();
	}

	void GamepadDS4::OnUpdate()
	{
		m_BufLAxis.x = FromS8(m_BufferRead[OffsetLX]);
		m_BufLAxis.y = FromS8(m_BufferRead[OffsetLY]);
		m_BufRAxis.x = FromS8(m_BufferRead[OffsetRX]);
		m_BufRAxis.y = FromS8(m_BufferRead[OffsetRY]);
		m_BufLAxis.z = FromU8(m_BufferRead[OffsetLZ]);
		m_BufRAxis.z = FromU8(m_BufferRead[OffsetRZ]);

		m_BufButtons.RLeft |= !!(m_BufferRead[OffsetButtons1] & 0x10);
		m_BufButtons.RDown |= !!(m_BufferRead[OffsetButtons1] & 0x20);
		m_BufButtons.RRight |= !!(m_BufferRead[OffsetButtons1] & 0x40);
		m_BufButtons.RUp |= !!(m_BufferRead[OffsetButtons1] & 0x80);
		m_BufButtons.TriggerL1 |= !!(m_BufferRead[OffsetButtons2] & 0x01);
		m_BufButtons.TriggerR1 |= !!(m_BufferRead[OffsetButtons2] & 0x02);
		m_BufButtons.TriggerL2 |= !!(m_BufferRead[OffsetButtons2] & 0x04);
		m_BufButtons.TriggerR2 |= !!(m_BufferRead[OffsetButtons2] & 0x08);
		m_BufButtons.Select |= !!(m_BufferRead[OffsetButtons2] & 0x10);
		m_BufButtons.Start |= !!(m_BufferRead[OffsetButtons2] & 0x20);
		m_BufButtons.TriggerL3 |= !!(m_BufferRead[OffsetButtons2] & 0x40);
		m_BufButtons.TriggerR3 |= !!(m_BufferRead[OffsetButtons2] & 0x80);
		m_BufButtons.Home |= !!(m_BufferRead[OffsetButtonExtra] & 0x01);
		m_BufButtons.Tap |= !!(m_BufferRead[OffsetButtonExtra] & 0x02);

		switch (m_BufferRead[OffsetButtons1] & 0x0F) {
		case 0:
			m_BufButtons.LUp |= 1;
			break;
		case 1:
			m_BufButtons.LUp |= 1;
			m_BufButtons.LRight |= 1;
			break;
		case 2:
			m_BufButtons.LRight |= 1;
			break;
		case 3:
			m_BufButtons.LRight |= 1;
			m_BufButtons.LDown |= 1;
			break;
		case 4:
			m_BufButtons.LDown |= 1;
			break;
		case 5:
			m_BufButtons.LDown |= 1;
			m_BufButtons.LLeft |= 1;
			break;
		case 6:
			m_BufButtons.LLeft |= 1;
			break;
		case 7:
			m_BufButtons.LUp |= 1;
			m_BufButtons.LLeft |= 1;
			break;
		}

		int frame = m_BufferRead[OffsetButtonExtra] >> 2;
		Battery = (float)(m_BufferRead[OffsetBattery] & 0x0F);
		IsBatteryCharging = !!(m_BufferRead[OffsetBattery] & 0x10);
	}
}