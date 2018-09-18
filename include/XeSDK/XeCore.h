#pragma once
#include <XeSDK/XeDef.h>

#define XESDK_VER_MAJ	0
#define XESDK_VER_MIN	8
#define XESDK_VER_BUILD	1
#define XESDK_VER ((XESDK_VER_MAJ << 24) | (XESDK_VER_MIN << 16) | (XESDK_VER_BUILD << 8))

//! \brief Basic functionalities of the engine
namespace Xe { namespace Core {

	enum CpuArch
	{
		CpuArch_Unknown,
		CpuArch_x86,
		CpuArch_x64,
		CpuArch_ARM,
		CpuArch_ARM64,
	};

	enum PlatformType
	{
		Platform_Unknown = 0x0000,
		Platform_Linux = 0x0100,
		Platform_Android = 0x0101,
		Platform_MacOs = 0x0200,
		Platform_Ios = 0x0201,
		Platform_Windows = 0x0300,
		Platform_Uwp = 0x0301,
		Platform_XboxOne = 0x0402,
		Platform_XboxOneS = 0x0403,
		Platform_XboxOneX = 0x0404,
		Platform_PSP2 = 0x0504,
		Platform_PS4 = 0x0505,
		Platform_PS4Pro = 0x0505,
		Platform_NX = 0x0606,
	};
    
	//! \brief Obtain basic information from XeEngine development kit.
    struct EngineInfo
	{
        ctstring Name;
        ctstring Developer;
        ctstring StrVersion;
        ctstring BuildDate;
		union
		{
			u32 Version;
			struct
			{
				s8 RESERVED;
				s8 VersionRevision;
				s8 VersionMinor;
				s8 VersionMajor;
			};
		};

		ctstring Compiler;
		u32 CompilerVersion;
    };

	struct HostInfo
	{
		CpuArch CpuArch;
		PlatformType Platform;
		struct
		{
			s32 Major;
			s32 Minor;
			s32 Build;
			s32 Revision;
		} OsVersion;

		u32 SdkVersion;
		u32 ProcessorsCount;
		u64 MemorySize;
		ctstring FriendlyName;
	};

	//! \brief Initialize the core and register the OS drivers.
	void Initialize();

    //! \brief Get versioning info about the running engine.
    const EngineInfo& GetEngineInfo();

	//! \brief Get the information about the operating system and the hardware host.
	const HostInfo& GetHostInfo();
} }