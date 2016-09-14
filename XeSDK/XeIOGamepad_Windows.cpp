#include "pch.h"

#include <XeSDK/XeIOGamepad.h>
#if defined(PLATFORM_WINDOWS) && !defined(PLATFORM_WINPHONE)
#include <XeSDK/XeCore.h>
#include <XInput.h>

// From XInput 1.1
typedef void (WINAPI *PFNXINPUTENABLEPROC) (
	_In_ BOOL enable);
typedef DWORD(WINAPI *PFNXINPUTGETCAPABILITIESPROC) (
	_In_   DWORD dwUserIndex,
	_In_   DWORD dwFlags,
	_Out_  XINPUT_CAPABILITIES *pCapabilities);
typedef DWORD(WINAPI *PFNXINPUTGETSTATEPROC) (
	_In_   DWORD dwUserIndex,
	_Out_  XINPUT_STATE *pState
	);
typedef DWORD(WINAPI *PFNXINPUTSETSTATEPROC) (
	_In_     DWORD dwUserIndex,
	_Inout_  XINPUT_VIBRATION *pVibration
	);

// From XInput 1.3
typedef DWORD(WINAPI *PFNXINPUTGETBATTERYINFORMATIONPROC) (
	_In_   DWORD dwUserIndex,
	_In_   BYTE devType,
	_Out_  XINPUT_BATTERY_INFORMATION *pBatteryInformation
	);
typedef DWORD(WINAPI *PFNXINPUTGETKEYSTROKEPROC) (
	DWORD dwUserIndex,
	DWORD dwReserved,
	PXINPUT_KEYSTROKE pKeystroke
	);

// From XInput 1.4
typedef DWORD(WINAPI *PFNXINPUTGETAUDIODEVICESIDSPROC) (
	_In_         DWORD dwUserIndex,
	_Out_opt_    LPWSTR pRenderDeviceId,
	_Inout_opt_  UINT *pRenderCount,
	_Out_opt_    LPWSTR pCaptureDeviceId,
	_Inout_opt_  UINT *pCaptureCount
	);

namespace Xe {
	namespace IO {
		struct Gamepad_Windows : Gamepad {
			bool IsInitialized;
			bool InitializationError;
			bool AsyncConnected[MaxGamepads];
			State AsyncState[MaxGamepads];

			HANDLE hThread = nullptr;
			DWORD idThread = 0;

			HMODULE hXInput = nullptr;
			svar nXInputVersion = 0;
			// XInput 1.1
			PFNXINPUTENABLEPROC pXInputEnable = nullptr;
			PFNXINPUTGETCAPABILITIESPROC pXInputGetCapabilities = nullptr;
			PFNXINPUTGETSTATEPROC pXInputGetState = nullptr;
			PFNXINPUTSETSTATEPROC pXInputSetState = nullptr;
			// XInput 1.3
			PFNXINPUTGETBATTERYINFORMATIONPROC pXInputGetBatteryInformation = nullptr;
			PFNXINPUTGETKEYSTROKEPROC pXInputGetKeystroke = nullptr;
			// XInput 1.4
			PFNXINPUTGETAUDIODEVICESIDSPROC pXInputGetAudioDeviceIds = nullptr;

			Gamepad_Windows() :
				IsInitialized(false),
				InitializationError(false)
			{
				for (svar i = 0; i < MaxGamepads; i++)
					AsyncConnected[i] = false;
			}
			~Gamepad_Windows() {
				Deinitialize();
			}
			bool Initialize() {
				if (IsInitialized) return true;
				if (InitializationError) return false;
				IsInitialized = true;

				struct XInputVersionCheck
				{
					ctstring szDll;
					svar version;
				};
				static const XInputVersionCheck XINPUT_VER[] =
				{
					{ _S("XINPUT1_4.DLL"), 0x0104 },	// Windows 8
					{ _S("XINPUT1_3.DLL"), 0x0103 },	// DXSDK June 2010
					{ _S("XINPUT9_1_0.DLL"), 0x0102 },	// Windows Vista
					{ _S("XINPUT1_2.DLL"), 0x0102 },
					{ _S("XINPUT1_1.DLL"), 0x0101 },	// DXSDK April 2006
				};
#ifdef PLATFORM_WIN32
				// If using Win32 API, the engine can be run from Vista or later.
				// In this case we should check what library is installed.
				for (svar i = 0; i < sizeof(XINPUT_VER) / sizeof(XInputVersionCheck); i++)
				{
					hXInput = LoadLibrary(XINPUT_VER[i].szDll);
					if (hXInput != nullptr)
					{
						nXInputVersion = XINPUT_VER[i].version;
						break;
					}
				}
				if (nXInputVersion == 0) {
					InitializationError = true;
					return false;
				}
				if (nXInputVersion >= 0x104)
					pXInputGetAudioDeviceIds = (PFNXINPUTGETAUDIODEVICESIDSPROC)GetProcAddress(hXInput, "XInputGetAudioDeviceIds");
				if (nXInputVersion >= 0x103)
				{
					pXInputGetBatteryInformation = (PFNXINPUTGETBATTERYINFORMATIONPROC)GetProcAddress(hXInput, "XInputGetBatteryInformation");
					pXInputGetKeystroke = (PFNXINPUTGETKEYSTROKEPROC)GetProcAddress(hXInput, "XInputGetKeystroke");
				}
				if (nXInputVersion >= 0x101)
				{
					pXInputEnable = (PFNXINPUTENABLEPROC)GetProcAddress(hXInput, "XInputEnable");
					pXInputGetCapabilities = (PFNXINPUTGETCAPABILITIESPROC)GetProcAddress(hXInput, "XInputGetCapabilities");
					pXInputGetState = (PFNXINPUTGETSTATEPROC)GetProcAddress(hXInput, "XInputGetState");
					pXInputSetState = (PFNXINPUTSETSTATEPROC)GetProcAddress(hXInput, "XInputSetState");

					pXInputEnable(TRUE);
				}
#else
				// We know for sure that Windows Runtime API has XInput 1.4

#if defined(PLATFORM_WINUNIVERSAL)
#else
				XInputEnable(TRUE);
#endif
				pXInputGetCapabilities = (PFNXINPUTGETCAPABILITIESPROC)XInputGetCapabilities;
				pXInputGetState = (PFNXINPUTGETSTATEPROC)XInputGetState;
				pXInputSetState = (PFNXINPUTSETSTATEPROC)XInputSetState;
				pXInputGetBatteryInformation = (PFNXINPUTGETBATTERYINFORMATIONPROC)XInputGetBatteryInformation;
				pXInputGetKeystroke = (PFNXINPUTGETKEYSTROKEPROC)XInputGetKeystroke;
				pXInputGetBatteryInformation = (PFNXINPUTGETBATTERYINFORMATIONPROC)XInputGetBatteryInformation;
				pXInputGetKeystroke = (PFNXINPUTGETKEYSTROKEPROC)XInputGetKeystroke;
				pXInputGetAudioDeviceIds = (PFNXINPUTGETAUDIODEVICESIDSPROC)XInputGetAudioDeviceIds;
#endif
				return true;
			}
			void Deinitialize() {
				if (IsInitialized) {
					if (hXInput) FreeLibrary(hXInput);
					if (hThread) CloseHandle(hThread);
				}
			}

			void InitializeAsync() {
				if (hThread) return;
				hThread = CreateThread(
					NULL,       // default security attributes
					0,          // default stack size
					(LPTHREAD_START_ROUTINE)AsyncProcessState,
					this,       // thread function arguments
					0,          // default creation flags
					&idThread); // receive thread identifier
			}
			static void AsyncProcessState(Gamepad_Windows *pGamePad) {
				State state;
				while (true) {
					for (svar i = 0; i < 3; i++) {
						bool isConnected = pGamePad->ProcessState(state, i);
						if (isConnected) pGamePad->AsyncState[i] = state;
						pGamePad->AsyncConnected[i] = isConnected;
					}
				}
			}

			bool ProcessState(State& state, svar index) {
				if (Initialize()) {
					XINPUT_STATE xstate;
					switch (pXInputGetState(index, &xstate)) {
					case ERROR_SUCCESS:
						break;
					case ERROR_DEVICE_NOT_CONNECTED:
						return false;
					default:
						return false;
					}
					state.LPad_Up = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
					state.LPad_Down = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
					state.LPad_Left = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
					state.LPad_Right = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
					state.RPad_Up = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
					state.RPad_Down = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_A);
					state.RPad_Left = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_X);
					state.RPad_Right = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_B);
					state.LTrigger_1 = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
					state.LTrigger_2 = xstate.Gamepad.bLeftTrigger >= 128;
					state.RTrigger_1 = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
					state.RTrigger_2 = xstate.Gamepad.bRightTrigger >= 128;
					state.LTrigger_3 = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
					state.RTrigger_3 = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
					state.BPause = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_START);
					state.BMenu = !!(xstate.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
					state.Left.x = (float)xstate.Gamepad.sThumbLX / 32767.0f;
					state.Left.y = (float)xstate.Gamepad.sThumbLY / 32767.0f;
					state.Right.x = (float)xstate.Gamepad.sThumbRX / 32767.0f;
					state.Right.y = (float)xstate.Gamepad.sThumbRY / 32767.0f;
					state.LTrigger_Pressure = (float)xstate.Gamepad.bLeftTrigger / 255.0f;
					state.RTrigger_Pressure = (float)xstate.Gamepad.bRightTrigger / 255.0f;
					return true;
				}
				return false;
			}
			bool GetState(State& state, svar index) {
				InitializeAsync();
				if (AsyncConnected[index]) {
					state = AsyncState[index];
					return true;
				}
				return false;
			}
			void SetProperties(const Properties& properties, svar index) {
				XINPUT_VIBRATION vibration;
				vibration.wLeftMotorSpeed = (WORD)(properties.Vibration.x * 65535.0f);
				vibration.wRightMotorSpeed = (WORD)(properties.Vibration.y * 65535.0f);
				pXInputSetState((DWORD)index, &vibration);
			}
		};

		Gamepad_Windows _Gamepad;
		bool Gamepad::ProcessState(State& state, svar index) {
			return _Gamepad.ProcessState(state, index);
		}
		bool Gamepad::GetState(State& state, svar index) {
			return _Gamepad.GetState(state, index);
		}
		void Gamepad::SetProperties(const Properties& properties, svar index) {
			_Gamepad.SetProperties(properties, index);
		}
	}
}
#else
namespace Xe {
	namespace IO {
		bool Gamepad::ProcessState(State& state, svar index) {
			return false;
		}
		bool Gamepad::GetState(State& state, svar index) {
			return false;
		}
		void Gamepad::SetProperties(const Properties& properties, svar index) {
		}
	}
}
#endif