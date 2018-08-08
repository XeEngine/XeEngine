#include "pch.h"
#include "XeDriverInputXInput.h"

namespace Xe { namespace Drivers { namespace Input {
	void XInputLib::Initialize() {
		static const Versions XINPUT_VER[] =
		{
			{ L"XINPUT1_4.DLL", 0x0104 },	// Windows 8
			{ L"XINPUT1_3.DLL", 0x0103 },	// DXSDK June 2010
			{ L"XINPUT9_1_0.DLL", 0x0102 },	// Windows Vista
			{ L"XINPUT1_2.DLL", 0x0102 },
			{ L"XINPUT1_1.DLL", 0x0101 },	// DXSDK April 2006
		};

#ifdef _WIN32
		// If using Win32 API, the engine can be run from Vista or later.
		// In this case we should check what library is installed.
		for (size_t i = 0; i < lengthof(XINPUT_VER); i++)
		{
			hXInput = LoadLibraryW(XINPUT_VER[i].szDll);
			if (hXInput != nullptr)
			{
				nXInputVersion = XINPUT_VER[i].version;
				break;
			}
		}

		if (nXInputVersion == 0) {
			Logger::Error("No XInput DLL found.");
			return;
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
	}

	void XInputLib::Destroy() {
		if (hXInput) FreeLibrary(hXInput);
	}

	XInputLib::XInputLib() {
		Initialize();
	}
	XInputLib::~XInputLib() {
		Destroy();
	}

	DWORD XInputLib::GetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities) {
		if (pXInputGetCapabilities)
			return pXInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
		return ERROR_NOT_SUPPORTED;
	}
	DWORD XInputLib::GetState(DWORD dwUserIndex, XINPUT_STATE *pState) {
		if (pXInputGetState)
			return pXInputGetState(dwUserIndex, pState);
		return ERROR_NOT_SUPPORTED;
	}
	DWORD XInputLib::SetState(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration) {
		if (pXInputSetState)
			return pXInputSetState(dwUserIndex, pVibration);
		return ERROR_NOT_SUPPORTED;
	}
	DWORD XInputLib::GetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION *pBatteryInformation) {
		if (pXInputGetBatteryInformation)
			return pXInputGetBatteryInformation(dwUserIndex, devType, pBatteryInformation);
		return ERROR_NOT_SUPPORTED;
	}

} } }
