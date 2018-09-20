#include "pch.h"
#include <XeSDK/XeStorage.h>
#include <XeSDK/IStorageDirectory.h>
#include <XeSDK/XeDrivers.h>
#include <XeSDK/XeCore.h>
#include "XeCoreCommon.h"

#include "XeDriversRenderingNullDriver.h"
#include "XeDriversRenderingD3D11Driver.h"
//#include "XeDriversRenderingOpenGlDriver.h"
#include "XeDriversSoundNullDriver.h"
//#include "XeDriversSoundWasapiDriver.h"
#include "XeDriversSoundXAudioDriver.h"
//#include "XeDriverInputWindowsCustom.h"

namespace Xe {

	Xe::Drivers::Rendering::NullDriver g_DriverRenderingNull;
	Xe::Drivers::Rendering::D3D11Driver g_DriverRenderingD3D11;
	//Xe::Drivers::Rendering::OpenGlDriver g_DriverRenderingOpenGl;
	Xe::Drivers::Sound::NullDriver g_DriverSoundNull;
	//Xe::Drivers::Sound::WasapiDriver g_DriverSoundWasapi;
	Xe::Drivers::Sound::XAudioDriver g_DriverSoundXAudio2;
	//Xe::Drivers::Input::WindowsCustom g_DriverInputCustom;

	void RegisterRenderingDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverRenderingD3D11);
		Xe::Drivers::RegisterDriver(&g_DriverRenderingNull);
		//Xe::Drivers::RegisterDriver(&g_DriverRenderingOpenGl);
	}

	void RegisterAudioDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverSoundXAudio2);
		Xe::Drivers::RegisterDriver(&g_DriverSoundNull);
		//Xe::Drivers::RegisterDriver(&g_DriverSoundWasapi);
	}

	void RegisterInputDrivers()
	{
		// Refer to UwpFrameView
	}

	namespace Core
	{
		static HostInfo m_HostInfo;

		struct HostInfoDesc
		{
			static HostInfoDesc Instance;
			HostInfo m_HostInfo;

			HostInfoDesc()
			{
				SYSTEM_INFO systemInfo = {};
				GetNativeSystemInfo(&systemInfo);

				MEMORYSTATUSEX memoryStatus = {};
				memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&memoryStatus);

				SYSTEMOSVERSIONINFO systemOsVersionInfo;
				GetSystemOSVersion(&systemOsVersionInfo);

				m_HostInfo.CpuArch = CPUARCH;

				m_HostInfo.OsVersion.Major = systemOsVersionInfo.MajorVersion;
				m_HostInfo.OsVersion.Minor = systemOsVersionInfo.MinorVersion;
				m_HostInfo.OsVersion.Build = systemOsVersionInfo.BuildNumber;
				m_HostInfo.OsVersion.Revision = systemOsVersionInfo.Revision;
				m_HostInfo.SdkVersion = _XDK_EDITION;
				m_HostInfo.ProcessorsCount = systemInfo.dwNumberOfProcessors;
				m_HostInfo.MemorySize = memoryStatus.ullTotalPhys;

#if _XDK_VER >= 0x3F6803F3 // XDK Edition 170600
				switch (GetConsoleType())
				{
				case CONSOLE_TYPE_XBOX_ONE:
					m_HostInfo.Platform = Platform_XboxOne;
					m_HostInfo.FriendlyName = "Xbox One";
					break;
				case CONSOLE_TYPE_XBOX_ONE_S:
					m_HostInfo.Platform = Platform_XboxOneS;
					m_HostInfo.FriendlyName = "Xbox One S";
					break;
				case CONSOLE_TYPE_XBOX_ONE_X:
					m_HostInfo.Platform = Platform_XboxOneX;
					m_HostInfo.FriendlyName = "Xbox One X";
					break;
				case CONSOLE_TYPE_XBOX_ONE_X_DEVKIT:
					m_HostInfo.Platform = Platform_XboxOneX;
					m_HostInfo.FriendlyName = "Xbox One X (devkit)";
					break;
				default:
					m_HostInfo.Platform = Platform_Unknown;
					m_HostInfo.FriendlyName = "Xbox One (unknown)";
					break;
				}
#else
				m_HostInfo.Platform = Platform_XboxOne;
				m_HostInfo.FriendlyName = "Xbox One";
#endif
			}
		};

		void Initialize()
		{
			HRESULT hr = RoInitialize(RO_INIT_MULTITHREADED);
			hr = hr;

			RegisterRenderingDrivers();
			RegisterAudioDrivers();
			RegisterInputDrivers();
		}

		HostInfoDesc HostInfoDesc::Instance;
		const HostInfo& GetHostInfo()
		{
			return HostInfoDesc::Instance.m_HostInfo;
		}
	}
}
