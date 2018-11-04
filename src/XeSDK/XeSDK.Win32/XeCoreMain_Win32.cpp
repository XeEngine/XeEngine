#include "pch.h"
#include <XeSDK/XeCore.h>
#include <XeSDK/XeStorage.h>
#include <XeSDK/IStorageDirectory.h>
#include <XeSDK/XeDrivers.h>
#include <XeSDK/XeString.h>
#include "XeCoreCommon.h"

#include <VersionHelpers.h>

#include "XeDriversRenderingNullDriver.h"
#include "XeDriversRenderingD3D11Driver.h"
//#include "XeDriversRenderingOpenGlDriver.h"
#include "XeDriversSoundNullDriver.h"
#include "XeDriversSoundWasapiDriver.h"
#include "XeDriversSoundXAudioDriver.h"
#include "XeDriverInputWindowsCustom.h"
#include "XeDriverInputXInput.h"

namespace Xe {

	Xe::Drivers::NullRenderingDriver g_DriverRenderingNull;
	Xe::Drivers::D3D11Driver g_DriverRenderingD3D11;
	//Xe::Drivers::OpenGlDriver g_DriverRenderingOpenGl;
	Xe::Drivers::NullSoundDriver g_DriverSoundNull;
	Xe::Drivers::WasapiDriver g_DriverSoundWasapi;
	Xe::Drivers::XAudioDriver g_DriverSoundXAudio2;
	Xe::Drivers::WindowsCustom g_DriverInputCustom;
	Xe::Drivers::XInput g_DriverInputXInput;

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
		Xe::Drivers::RegisterDriver(&g_DriverInputCustom);
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

				int ver = 0;
				if (IsWindows10OrGreater()) ver = _WIN32_WINNT_WIN10;
				else if (IsWindows8Point1OrGreater()) ver = _WIN32_WINNT_WINBLUE;
				else if (IsWindows8OrGreater()) ver = _WIN32_WINNT_WIN8;
				else if (IsWindows7OrGreater()) ver = _WIN32_WINNT_WIN7;
				else if (IsWindowsVistaOrGreater()) ver = _WIN32_WINNT_VISTA;
				else if (IsWindowsXPOrGreater()) ver = _WIN32_WINNT_WINXP;
				else ver = _WIN32_WINNT_WIN2K;

#ifdef _MSVC_WINDOWS_SDK
				auto msvcWindowsSdk = _MSVC_WINDOWS_SDK;

				// Search for 10.0.17134.0
				StringSpan strMajor = msvcWindowsSdk;
				if (strMajor.IsEmpty())
				{
					int indexMinor = Xe::String(strMajor).IndexOf('.');
					if (indexMinor >= 0)
					{
						StringSpan strMinor = strMajor + indexMinor + 1;
						int indexBuild = Xe::String(strMinor).IndexOf('.');

						if (indexBuild > 0)
						{
							// The format for 10.0.17134.0, take only the build number
							StringSpan strBuild = strMinor + indexBuild + 1;
							m_HostInfo.SdkVersion = strBuild.ParseInt();
						}
						else
						{
							// The format is 8.1
							m_HostInfo.SdkVersion = (strMajor.ParseInt() << 8) | strMinor.ParseInt();
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

				m_HostInfo.CpuArch = CPUARCH;
				m_HostInfo.Platform = Platform_Windows;
				m_HostInfo.OsVersion.Major = (ver >> 8) & 0xFF;
				m_HostInfo.OsVersion.Minor = ver & 0xFF;
				m_HostInfo.OsVersion.Build = 0;
				m_HostInfo.OsVersion.Revision = 0;
				m_HostInfo.SdkVersion = WINVER;
				m_HostInfo.ProcessorsCount = systemInfo.dwNumberOfProcessors;
				m_HostInfo.MemorySize = memoryStatus.ullTotalPhys;
				m_HostInfo.FriendlyName = "Windows";
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
