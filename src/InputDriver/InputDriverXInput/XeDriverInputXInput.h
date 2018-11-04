#pragma once
#include <XeSDK/IDriversGamepadDriver.h>

#include <Windows.h>
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

namespace Xe { namespace Drivers {

	class XInput : public IGamepadDriver {

		// Inherited from IDriver
		const char* GetDriverName() const { return "XInput"; }
		DriverType GetDriverType() const { return DriverType_Input; }

		// Inherited from IInputDriver
		InputDeviceType GetInputDeviceType() const { return InputDeviceType_Gamepad; }

		// Inherited from IGamepadDriver
		bool OpenDevice(
			Xe::IO::Gamepad** ppGamepad,
			const Xe::IO::GamepadEntry& entry);

		size_t EnumerateDevices(
			Xe::IO::GamepadEntry* pEntries,
			size_t index,
			size_t count);
	};

	class XInputLib {
	public:
		XInputLib();
		~XInputLib();

		DWORD GetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities);
		DWORD GetState(DWORD dwUserIndex, XINPUT_STATE *pState);
		DWORD SetState(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);
		DWORD GetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION *pBatteryInformation);

	private:
		struct Versions
		{
			const wchar_t* szDll;
			int version;
		};

		HMODULE hXInput = nullptr;
		int nXInputVersion = 0;

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

		void Initialize();
		void Destroy();
	};

} }
