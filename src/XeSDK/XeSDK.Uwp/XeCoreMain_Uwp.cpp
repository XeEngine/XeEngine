#include "pch.h"
#include <XeSDK/XeCore.h>
#include <XeSDK/XeDrivers.h>
#include <XeSDK/XeString.h>
#include "XeCoreCommon.h"

#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>

#include "XeDriversRenderingNullDriver.h"
#include "XeDriversRenderingD3D11Driver.h"
#include "XeDriversSoundNullDriver.h"
//#include "XeDriversSoundWasapiDriver.h"
#include "XeDriversSoundXAudioDriver.h"
//#include "XeDriverInputWindowsCustom.h"
#include "XeDriverInputXInput.h"

#define STR(x) (#x)

namespace Xe {
	Xe::Drivers::Rendering::NullDriver g_DriverRenderingNull;
	Xe::Drivers::Rendering::D3D11Driver g_DriverRenderingD3D11;
	Xe::Drivers::Sound::NullDriver g_DriverSoundNull;
	//Xe::Drivers::Sound::WasapiDriver g_DriverSoundWasapi;
	Xe::Drivers::Sound::XAudioDriver g_DriverSoundXAudio2;
	//Xe::Drivers::Input::WindowsCustom g_DriverInputCustom;
	Xe::Drivers::Input::XInput g_DriverInputXInput;

	void RegisterRenderingDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverRenderingD3D11);
		Xe::Drivers::RegisterDriver(&g_DriverRenderingNull);
	}

	void RegisterAudioDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverSoundXAudio2);
		Xe::Drivers::RegisterDriver(&g_DriverSoundNull);
		//Xe::Drivers::RegisterDriver(&g_DriverSoundWasapi);
	}

	void RegisterInputDrivers()
	{
		//Xe::Drivers::RegisterDriver(&g_DriverInputCustom);
		Xe::Drivers::RegisterDriver(&g_DriverInputXInput);
	}

	namespace Core
	{
		static const HostInfo m_HostInfo;

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

#ifdef _MSVC_WINDOWS_SDK
				const char* msvcWindowsSdk = _MSVC_WINDOWS_SDK;

				// Search for 10.0.17134.0
				auto strMajor = msvcWindowsSdk;
				if (strMajor)
				{
					auto strMinor = Xe::String::FindCharacter(msvcWindowsSdk, '.') + 1;
					if (strMinor - 1)
					{
						auto strBuild = Xe::String::FindCharacter(strMinor, '.') + 1;
						if (strBuild - 1)
						{
							auto strRev = Xe::String::FindCharacter(strBuild, '.');
							int len = strRev ? (int)(strRev - strBuild) : Xe::String::GetLength(strBuild);

							// The format for 10.0.17134.0
							m_HostInfo.SdkVersion = Xe::String::Atoi(strBuild, len, 10);
						}
						else
						{
							// The format is 8.1
							int major = Xe::String::Atoi(strMajor, (int)(strMinor - strMajor), 10);
							int minor = Xe::String::Atoi(strMinor, Xe::String::GetLength(strMinor), 10);
							m_HostInfo.SdkVersion = (major << 8) | minor;
						}
					}
					else
					{
						m_HostInfo.SdkVersion = WINVER;
				}
			}
				else
				{
					m_HostInfo.SdkVersion = WINVER;
				}

#else
				m_HostInfo.SdkVersion = WINVER;
#endif


				// TODO https://docs.microsoft.com/en-us/uwp/api/Windows.System.Profile.AnalyticsInfo#Windows_System_Profile_AnalyticsInfo_VersionInfo
				m_HostInfo.CpuArch = CPUARCH;
				m_HostInfo.Platform = Platform_Uwp;
				m_HostInfo.OsVersion.Major = 10;
				m_HostInfo.OsVersion.Minor = 0;
				m_HostInfo.OsVersion.Build = 10240;
				m_HostInfo.OsVersion.Revision = 0;
				m_HostInfo.ProcessorsCount = systemInfo.dwNumberOfProcessors;
				m_HostInfo.MemorySize = memoryStatus.ullTotalPhys;
				m_HostInfo.FriendlyName = "UWP";
			}
		};

		void Initialize()
		{
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
