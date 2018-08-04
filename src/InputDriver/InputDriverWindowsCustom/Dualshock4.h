#pragma once
#include "GamepadGeneric.h"

namespace Xe {
	class GamepadDS4 : public GamepadGeneric {
	public:
		typedef GamepadGeneric Base;

		// Input buffer offsets
		static const int OffsetLX = 0x01;
		static const int OffsetLY = 0x02;
		static const int OffsetRX = 0x03;
		static const int OffsetRY = 0x04;
		static const int OffsetButtons1 = 0x05;
		static const int OffsetButtons2 = 0x06;
		static const int OffsetButtonExtra = 0x07;
		static const int OffsetLZ = 0x08;
		static const int OffsetRZ = 0x09;
		static const int OffsetBattery = 0x1E;

		// Output buffer offsets
		static const int OffsetRumbleLeft = 0x04;
		static const int OffsetRumbleRight = 0x05;
		static const int OffsetLightbarR = 0x06;
		static const int OffsetLightbarG = 0x07;
		static const int OffsetLightbarB = 0x08;

		GamepadDS4(const Xe::IO::HidDevice& hidDevice);
		virtual ~GamepadDS4();

		void SetRumbling(const Xe::Math::Vector2f& rumbling);

		void SetLightbar(const Xe::Graphics::Color& color);

		virtual void OnUpdate();
	};
}