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

	Xe::Drivers::Rendering::NullDriver g_DriverRenderingNull;
	Xe::Drivers::Rendering::D3D11Driver g_DriverRenderingD3D11;
	//Xe::Drivers::Rendering::OpenGlDriver g_DriverRenderingOpenGl;
	Xe::Drivers::Sound::NullDriver g_DriverSoundNull;
	Xe::Drivers::Sound::WasapiDriver g_DriverSoundWasapi;
	Xe::Drivers::Sound::XAudioDriver g_DriverSoundXAudio2;
	Xe::Drivers::Input::WindowsCustom g_DriverInputCustom;
	Xe::Drivers::Input::XInput g_DriverInputXInput;

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
