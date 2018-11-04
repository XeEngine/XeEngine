#include "pch.h"
#include <XeSDK/XeIODevices.h>
#include <setupapi.h>
#include <Hidsdi.h>
#include <XeSDK/XeLogger.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeMath.h>

#pragma comment(lib, "Setupapi.lib")

namespace Xe { namespace IO {
    struct HidLib {
        typedef void (WINAPI *ptrHidD_GetHidGuid)(LPGUID HidGuid);

        HMODULE m_ModuleHid;
        GUID m_GuidHid = { 0 };

        HidLib() {
            m_ModuleHid = LoadLibraryW(L"HID.DLL");
            if (m_ModuleHid != nullptr)
			{
                auto HidD_GetHidGuid = (ptrHidD_GetHidGuid)GetProcAddress(m_ModuleHid, "HidD_GetHidGuid");
                if (HidD_GetHidGuid != nullptr) {
                    HidD_GetHidGuid(&m_GuidHid);
					LOGI("HID GUID: {0x%08X-0x%04X-0x%04X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X}\n",
                                    m_GuidHid.Data1, m_GuidHid.Data2, m_GuidHid.Data3,
                                    m_GuidHid.Data4[0], m_GuidHid.Data4[1], m_GuidHid.Data4[2], m_GuidHid.Data4[3],
                                    m_GuidHid.Data4[4], m_GuidHid.Data4[5], m_GuidHid.Data4[6], m_GuidHid.Data4[7]);
                }
            }
        }
        ~HidLib() {
            if (m_ModuleHid != nullptr)
                FreeLibrary(m_ModuleHid);
        }
    };

    HidLib g_Hid;

    std::vector<HidDevice> GetHidDevices(u16 vendorId, size_t productIdCount, const u16 *productIds) {
		struct MY_DEVICE_INTERFACE_DETAIL_DATA {
			DWORD cbSize;
			CHAR DevicePath[0xFC];
		};

		std::vector<HidDevice> devices;
		HDEVINFO hDevInfo = SetupDiGetClassDevs(
			&g_Hid.m_GuidHid,	// Device setup class
			nullptr,
			nullptr,			// HWND parent
			DIGCF_PRESENT |
			DIGCF_DEVICEINTERFACE
		);
        if (hDevInfo != INVALID_HANDLE_VALUE) {
            SP_DEVINFO_DATA infoData = { 0 };
            infoData.cbSize = sizeof(SP_DEVINFO_DATA);

            for (DWORD deviceIndex = 0;
                    SetupDiEnumDeviceInfo(hDevInfo, deviceIndex, &infoData);
                    deviceIndex++) {
                SP_DEVICE_INTERFACE_DATA interfaceData = { 0 };
                interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

                for (DWORD interfaceIndex = 0;
                        SetupDiEnumDeviceInterfaces(hDevInfo, &infoData, &g_Hid.m_GuidHid, interfaceIndex, &interfaceData);
                        interfaceIndex++) {

					MY_DEVICE_INTERFACE_DETAIL_DATA interfaceDetailData;
					PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData =
						(PSP_DEVICE_INTERFACE_DETAIL_DATA)&interfaceDetailData;

					SP_DEVINFO_DATA devInfoData{ 0 };
					devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

					DWORD requiredSize;
					SetupDiGetDeviceInterfaceDetailA(hDevInfo, &interfaceData, nullptr, 0, &requiredSize, nullptr);
					pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
					requiredSize = Math::Min<DWORD>(requiredSize, sizeof(MY_DEVICE_INTERFACE_DETAIL_DATA));
					if (SetupDiGetDeviceInterfaceDetailA(hDevInfo, &interfaceData, pInterfaceDetailData, requiredSize, &requiredSize, nullptr)) {
						IO::HidDevice device;
						char* strIndex;
						strncpy(device.Path, (const char*)interfaceDetailData.DevicePath, sizeof(device.Path));
						strIndex = strstr((char*)interfaceDetailData.DevicePath, "vid_");
						if (strIndex != nullptr) {
							device.VendorId = (u16)StringSpan(strIndex, 4).Parse(0, 16);
							if (vendorId == 0 || (vendorId != 0 && vendorId == device.VendorId)) {
								strIndex = strstr(strIndex, "pid_");
								device.ProductId = (u16)StringSpan(strIndex, 4).Parse(0, 16);

								// Optimized way to filter from productId
								bool addDevice = false;
								if (productIdCount > 1) {
									for (size_t i = 0; i < productIdCount; i++) {
										if (device.ProductId == productIds[i]) {
											addDevice = true;
											break;
										}
									}
								}
								if (productIdCount == 1) {
									if (device.ProductId == productIds[0])
										addDevice = true;
								}
								else
									addDevice = true;
								if (addDevice) {

									LOGD("Device found: %04X:%04X %s\n", device.VendorId, device.ProductId, device.Path);
									devices.push_back(device);
								}
							}
						}
					}
					else {
						LOGE("SetupDiGetDeviceInterfaceDetailA failed: %08X\n", GetLastError());
					}
                }
            }
            SetupDiDestroyDeviceInfoList(hDevInfo);
        }
        else {
            LOGE("Unable to enumerate Hid Devices.");
        }
		return devices;
    }
} }