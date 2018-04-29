#include "pch.h"

#include <XeSDK/XeIOGamepad.h>
#include <XeSDK/XeCore.h>
#include <XeSDK/XeLogger.h>
#include <XeSDK/XeIODevices.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeMemory.h>
#include <cassert>
#include <Windows.h>

#define VENDOR(a) (a[0])
#define PRODUCTS(a) (a + 1)
#define PRODUCTSCOUNT(a) (lengthof(a) - 1)
#define GETHIDS(a) IO::GetHidDevices(VENDOR(a), PRODUCTSCOUNT(a), PRODUCTS(a))

#define VID_SONY 0x054C
#define VID_SONY_DS3 0x0268
#define VID_SONY_DS4 0x05C4
#define VID_SONY_DS4V2 0x09CC
#define VID_SONY_DS4W 0x0BA0

namespace Xe {
	namespace IO {
		struct InternalGamepad {
			IO::HidDevice Hid;
			u64 Hash;
			bool IsOpen;
		};

		class GamepadGeneric : public Gamepad {
		private:
			HANDLE m_Handle;
			HANDLE m_Thread;
			DWORD m_ThreadId;
			BOOL m_Closed;

			bool TryOpen(const char* path, bool exclusive) {
				m_Handle = CreateFileA(path,
					GENERIC_READ | GENERIC_WRITE,
					exclusive ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE),
					NULL, OPEN_EXISTING, 0, NULL);
				return m_Handle != INVALID_HANDLE_VALUE;
			}

			void Close() {
				const DWORD TIMEOUT = 250;

				m_Closed = TRUE;
				WaitForSingleObject(m_Thread, TIMEOUT);

				CloseHandle(m_Handle);
				m_Handle = INVALID_HANDLE_VALUE;

				HANDLE thread = m_Thread;
				m_Thread = NULL;
				m_ThreadId = 0;
				TerminateThread(thread, 0);
			}

		protected:
			const HANDLE& Handle;
			bool m_IsInvalidated;

			Buttons m_BufButtons;
			Xe::Math::Vector3f m_BufLAxis;
			Xe::Math::Vector3f m_BufRAxis;
			Xe::Math::Vector3f m_BufGyroscope;
			Xe::Math::Vector3f m_BufAccelerometer;
			Xe::Math::Vector2f m_BufTouchpad;

			void CatchError() {
				DWORD error = GetLastError();
				switch (error) {
				case ERROR_INVALID_PARAMETER:
					Logger::Info("Gamepad invalid parameter.\n");
					break;
				case ERROR_OPERATION_ABORTED:
				case ERROR_DEVICE_NOT_CONNECTED:
					Close();
					Logger::Info("Gamepad was disconnected.\n");
					break;
				case ERROR_INVALID_USER_BUFFER:
					Logger::Info("Gamepad invalid user buffer.\n");
					break;
				default:
					Logger::Warning("Uncaught Gamepad error: %08X\n", error);
					Close();
					break;
				}

			}

			static u8 ToU8(float data) {
				return (BYTE)(Math::Min(Math::Max(data, 0.0f), 1.0f) * 255.0f);
			}

			static float NormalizeS8(BYTE data) {
				if (data > 0x80) return (float)(data - 0x80) / 127.0f;
				if (data < 0x80) return (float)(data - 0x7F) / 127.0f;
				return 0.0f;
			}

			static float NormalizeU8(BYTE data) {
				return (float)data / 255.0f;
			}

		public:
			bool Query(IObject **obj, UID id) {
				switch (id) {
				case IObject::ID:
					*obj = this;
					return true;
				}
				*obj = nullptr;
				return false;
			}

			GamepadGeneric(const InternalGamepad& gamepad) :
				m_Handle(INVALID_HANDLE_VALUE),
				Handle(m_Handle),
				m_Closed(FALSE),
				m_IsInvalidated(true)
			{
				Logger::Info("Opening gamepad at %s\n", gamepad.Hid.Path);
				if (TryOpen(gamepad.Hid.Path, false)) {
					Logger::Debug("Gamepad connected with success!");
				}
				else {
					Logger::Warning("Unable to connect to the gamepad %s.", gamepad.Hid.Path);
				}

				if (IsAvailable()) {
					Capabilities.Data = 0x53ffffff;
					Capabilities.Gyroscope = 0;			// 0x04000000
					Capabilities.Accelerometer = 0;		// 0x08000000
					Capabilities.Audio = 0;				// 0x20000000
					Capabilities.Touchpad = 0;			// 0x80000000
				}

				m_Thread = CreateThread(
					NULL,
					sizeof(size_t) * 4,
					UpdatePad,
					this,
					0,
					&m_ThreadId
				);

				if (m_Thread == NULL) {
					Logger::Warning("Unable to create a thread for updating.", gamepad.Hid.Path);
				}
			}

			virtual ~GamepadGeneric() {
				Close();
			}

			bool IsAvailable() const {
				return m_Handle != INVALID_HANDLE_VALUE;
			}

			void Update() {
				if (m_Thread != NULL) {
					PressedButtons = m_BufButtons;
					m_BufButtons.Data = 0;

					LAxis = m_BufLAxis;
					RAxis = m_BufRAxis;
					Gyroscope = m_BufGyroscope;
					Accelerometer = m_BufAccelerometer;
					Touchpad = m_BufTouchpad;
				}
				else {
					RealUpdate();
				}
			}

			void RealUpdate() {
				while (!m_Closed && IsAvailable()) {
					Query();
					if (IsAvailable() && m_IsInvalidated) {
						Flush();
						m_IsInvalidated = false;
					}
				}
			}

			virtual void Query() = 0;

			static DWORD __stdcall UpdatePad(LPVOID lpThreadParameter) {
				auto pad = (GamepadGeneric*)lpThreadParameter;
				if (pad) {
					pad->RealUpdate();
				}

				return 0U;
			}
		};

		class GamepadDS3 : public GamepadGeneric {
		public:
			typedef GamepadGeneric Base;

			static const int InBufferSize = 48;
			static const int OutBufferSize = 32;

			BYTE m_InBuffer[InBufferSize];
			BYTE m_OutBuffer[OutBufferSize];

			GamepadDS3(InternalGamepad& gamePad) :
				Base(gamePad) {
				Memory::Fill(m_OutBuffer, 0x00, sizeof(m_OutBuffer));
			}

			~GamepadDS3() {
				Memory::Fill(m_OutBuffer, 0x00, sizeof(m_OutBuffer));
				Flush();
			}

			void SetRumbling(const Math::Vector2f& rumbling) {
			}

			void SetLightbar(const Graphics::Color& color) {

			}

			void Query() {
				DWORD read;
				if (ReadFile(Handle, m_InBuffer, InBufferSize, &read, NULL)) {
					Logger::Debug("%02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X\n",
						m_InBuffer[0x00], m_InBuffer[0x01], m_InBuffer[0x02], m_InBuffer[0x03],
						m_InBuffer[0x04], m_InBuffer[0x05], m_InBuffer[0x06], m_InBuffer[0x07],
						m_InBuffer[0x08], m_InBuffer[0x09], m_InBuffer[0x0A], m_InBuffer[0x0B],
						m_InBuffer[0x0C], m_InBuffer[0x0D], m_InBuffer[0x0E], m_InBuffer[0x0F]);
				}
				else
					CatchError();
			}

			void Flush() {
			}
		};

		class GamepadDS4 : public GamepadGeneric {
		public:
			typedef GamepadGeneric Base;

			static const int InBufferSize = 64;
			static const int OutBufferSize = 32;

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

			BYTE m_InBuffer[InBufferSize];
			BYTE m_OutBuffer[OutBufferSize];

			GamepadDS4(InternalGamepad& gamePad) :
				Base(gamePad) {
				Memory::Fill(m_OutBuffer, 0x00, sizeof(m_OutBuffer));
			}
			~GamepadDS4() {
				Memory::Fill(m_OutBuffer, 0x00, sizeof(m_OutBuffer));
				Flush();
			}

			void SetRumbling(const Math::Vector2f& rumbling) {
				m_OutBuffer[OffsetRumbleLeft] = ToU8(rumbling.y);
				m_OutBuffer[OffsetRumbleRight] = ToU8(rumbling.x);
				m_IsInvalidated = true;
			}

			void SetLightbar(const Graphics::Color& color) {
				m_OutBuffer[OffsetLightbarR] = ToU8(color.r);
				m_OutBuffer[OffsetLightbarG] = ToU8(color.g);
				m_OutBuffer[OffsetLightbarB] = ToU8(color.b);
				m_IsInvalidated = true;
			}

			virtual void Query() {
				DWORD read;
				if (ReadFile(Handle, m_InBuffer, InBufferSize, &read, NULL)) {
					m_BufLAxis.x = NormalizeS8(m_InBuffer[OffsetLX]);
					m_BufLAxis.y = NormalizeS8(m_InBuffer[OffsetLY]);
					m_BufRAxis.x = NormalizeS8(m_InBuffer[OffsetRX]);
					m_BufRAxis.y = NormalizeS8(m_InBuffer[OffsetRY]);
					m_BufLAxis.z = NormalizeU8(m_InBuffer[OffsetLZ]);
					m_BufRAxis.z = NormalizeU8(m_InBuffer[OffsetRZ]);

					m_BufButtons.RLeft |= !!(m_InBuffer[OffsetButtons1] & 0x10);
					m_BufButtons.RDown |= !!(m_InBuffer[OffsetButtons1] & 0x20);
					m_BufButtons.RRight |= !!(m_InBuffer[OffsetButtons1] & 0x40);
					m_BufButtons.RUp |= !!(m_InBuffer[OffsetButtons1] & 0x80);
					m_BufButtons.TriggerL1 |= !!(m_InBuffer[OffsetButtons2] & 0x01);
					m_BufButtons.TriggerR1 |= !!(m_InBuffer[OffsetButtons2] & 0x02);
					m_BufButtons.TriggerL2 |= !!(m_InBuffer[OffsetButtons2] & 0x04);
					m_BufButtons.TriggerR2 |= !!(m_InBuffer[OffsetButtons2] & 0x08);
					m_BufButtons.Select |= !!(m_InBuffer[OffsetButtons2] & 0x10);
					m_BufButtons.Start |= !!(m_InBuffer[OffsetButtons2] & 0x20);
					m_BufButtons.TriggerL3 |= !!(m_InBuffer[OffsetButtons2] & 0x40);
					m_BufButtons.TriggerR3 |= !!(m_InBuffer[OffsetButtons2] & 0x80);
					m_BufButtons.Home |= !!(m_InBuffer[OffsetButtonExtra] & 0x01);
					m_BufButtons.Tap |= !!(m_InBuffer[OffsetButtonExtra] & 0x02);

					switch (m_InBuffer[OffsetButtons1] & 0x0F) {
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

					int frame = m_InBuffer[OffsetButtonExtra] >> 2;
					Battery = m_InBuffer[OffsetBattery] & 0x0F;
					IsBatteryCharging = !!(m_InBuffer[OffsetBattery] & 0x10);
				}
				else
					CatchError();
			}

			virtual void Flush() {
				DWORD written;
				m_OutBuffer[0x00] = 0x05;
				m_OutBuffer[0x01] = 0xFF;
				if (WriteFile(Handle, m_OutBuffer, OutBufferSize, &written, NULL)) {
				}
				else
					CatchError();
			}
		};

		class GamepadDS4V2 : public GamepadDS4 {
		public:
			typedef GamepadDS4 Base;

			GamepadDS4V2(InternalGamepad& gamePad) :
				Base(gamePad) {
				Logger::Info("Found DualShock 4 gen.2");
			}
		};

		class GamepadDS4Wireless : public GamepadGeneric {
		public:
			typedef GamepadGeneric Base;

			static const int InBufferSize = 0x223;
			static const int OutBufferSize = 0x4E;

			// Input buffer offsets
			static const int OffsetLX = 0x03;
			static const int OffsetLY = 0x04;
			static const int OffsetRX = 0x05;
			static const int OffsetRY = 0x06;
			static const int OffsetButtons1 = 0x07;
			static const int OffsetButtons2 = 0x08;
			static const int OffsetButtonExtra = 0x09;
			static const int OffsetLZ = 0x0A;
			static const int OffsetRZ = 0x0B;
			static const int OffsetBattery = 0x20;

			// Output buffer offsets
			static const int OffsetRumbleLeft = 0x06;
			static const int OffsetRumbleRight = 0x07;
			static const int OffsetLightbarR = 0x08;
			static const int OffsetLightbarG = 0x09;
			static const int OffsetLightbarB = 0x0A;

			BYTE m_InBuffer[InBufferSize];
			BYTE m_OutBuffer[OutBufferSize];

			GamepadDS4Wireless(InternalGamepad& gamePad) :
				Base(gamePad) {
				Memory::Fill(m_OutBuffer, 0x00, sizeof(m_OutBuffer));
			}
			~GamepadDS4Wireless() {
				Memory::Fill(m_OutBuffer, 0x00, sizeof(m_OutBuffer));
				Flush();
			}



			void SetRumbling(const Math::Vector2f& rumbling) {
				m_OutBuffer[OffsetRumbleLeft] = ToU8(rumbling.y);
				m_OutBuffer[OffsetRumbleRight] = ToU8(rumbling.x);
				m_IsInvalidated = true;
			}

			void SetLightbar(const Graphics::Color& color) {
				m_OutBuffer[OffsetLightbarR] = ToU8(color.r);
				m_OutBuffer[OffsetLightbarG] = ToU8(color.g);
				m_OutBuffer[OffsetLightbarB] = ToU8(color.b);
				m_IsInvalidated = true;
			}

			virtual void Query() {
				DWORD read;
				if (ReadFile(Handle, m_InBuffer, InBufferSize, &read, NULL)) {
					LAxis.x = NormalizeS8(m_InBuffer[OffsetLX]);
					LAxis.y = NormalizeS8(m_InBuffer[OffsetLY]);
					RAxis.x = NormalizeS8(m_InBuffer[OffsetRX]);
					RAxis.y = NormalizeS8(m_InBuffer[OffsetRY]);
					LAxis.z = NormalizeU8(m_InBuffer[OffsetLZ]);
					RAxis.z = NormalizeU8(m_InBuffer[OffsetRZ]);

					PressedButtons.RLeft = !!(m_InBuffer[OffsetButtons1] & 0x10);
					PressedButtons.RDown = !!(m_InBuffer[OffsetButtons1] & 0x20);
					PressedButtons.RRight = !!(m_InBuffer[OffsetButtons1] & 0x40);
					PressedButtons.RUp = !!(m_InBuffer[OffsetButtons1] & 0x80);
					PressedButtons.TriggerL1 = !!(m_InBuffer[OffsetButtons2] & 0x01);
					PressedButtons.TriggerR1 = !!(m_InBuffer[OffsetButtons2] & 0x02);
					PressedButtons.TriggerL2 = !!(m_InBuffer[OffsetButtons2] & 0x04);
					PressedButtons.TriggerR2 = !!(m_InBuffer[OffsetButtons2] & 0x08);
					PressedButtons.Select = !!(m_InBuffer[OffsetButtons2] & 0x10);
					PressedButtons.Start = !!(m_InBuffer[OffsetButtons2] & 0x20);
					PressedButtons.TriggerL3 = !!(m_InBuffer[OffsetButtons2] & 0x40);
					PressedButtons.TriggerR3 = !!(m_InBuffer[OffsetButtons2] & 0x80);
					PressedButtons.Home = !!(m_InBuffer[OffsetButtonExtra] & 0x01);
					PressedButtons.Tap = !!(m_InBuffer[OffsetButtonExtra] & 0x02);

					switch (m_InBuffer[OffsetButtons1] & 0x0F) {
					case 0:
						PressedButtons.LUp = 1;
						break;
					case 1:
						PressedButtons.LUp = 1;
						PressedButtons.LRight = 1;
						break;
					case 2:
						PressedButtons.LRight = 1;
						break;
					case 3:
						PressedButtons.LRight = 1;
						PressedButtons.LDown = 1;
						break;
					case 4:
						PressedButtons.LDown = 1;
						break;
					case 5:
						PressedButtons.LDown = 1;
						PressedButtons.LLeft = 1;
						break;
					case 6:
						PressedButtons.LLeft = 1;
						break;
					case 7:
						PressedButtons.LUp = 1;
						PressedButtons.LLeft = 1;
						break;
					}

					int frame = m_InBuffer[OffsetButtonExtra] >> 2;
					Battery = m_InBuffer[OffsetBattery] & 0x0F;
					IsBatteryCharging = !!(m_InBuffer[OffsetBattery] & 0x10);
				}
				else
					CatchError();
			}

			void Flush() {
				DWORD written;
				m_OutBuffer[0x00] = 0x11;
				m_OutBuffer[0x01] = 0x80;
				m_OutBuffer[0x02] = 0x00;
				m_OutBuffer[0x03] = 0xFF;
				if (WriteFile(Handle, m_OutBuffer, OutBufferSize, &written, NULL)) {
				}
				else
					CatchError();
			}
		};

		int g_GamepadsCount = 0;
		InternalGamepad g_Gamepads[16];

		GamepadResult InternalOpenGamepad(InternalGamepad& gamePad, Gamepad **ppGamepad) {
			if (gamePad.Hid.VendorId == VID_SONY) {
				switch (gamePad.Hid.ProductId) {
				case VID_SONY_DS3:
					*ppGamepad = new GamepadDS3(gamePad);
					return GamepadResult::Ok;
				case VID_SONY_DS4:
					*ppGamepad = new GamepadDS4(gamePad);
					return GamepadResult::Ok;
				case VID_SONY_DS4V2:
					*ppGamepad = new GamepadDS4V2(gamePad);
					return GamepadResult::Ok;
				case VID_SONY_DS4W:
					*ppGamepad = new GamepadDS4Wireless(gamePad);
					return GamepadResult::Ok;
				}
			}
			return GamepadResult::NotSupported;
		}

		int QueryGamepads(GamepadEntry* pEntries, int count) {
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
				internalGamepad.Hid = *it;
				internalGamepad.Hash = String::GetHashFNV1a(internalGamepad.Hid.Path, String::GetLength(internalGamepad.Hid.Path));
			}
			g_GamepadsCount = padIndex;

			int found = Math::Min(count, g_GamepadsCount);
			for (int i = 0; i < found; i++) {
				pEntries[i] = g_Gamepads[i].Hash;
			}
			return found;
		}

		GamepadResult OpenGamepad(const GamepadEntry &entry, Gamepad **ppGamepad) {
			for (int i = 0; i < g_GamepadsCount; i++) {
				if (g_Gamepads[i].Hash == entry) {
					return InternalOpenGamepad(g_Gamepads[i], ppGamepad);
				}
			}
			return GamepadResult::NotFound;
		}
	}
}
