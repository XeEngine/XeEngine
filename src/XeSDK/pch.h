#pragma once
#if defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
#define PLATFORM_WINRT
#endif
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINRT)
#define PLATFORM_WINDOWS
#endif

#ifdef PLATFORM_WINDOWS
// Stuff from Microsoft products
#include <windows.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include "ShaderD3D11_VSDefault.h"
#include "ShaderD3D11_FSDefault.h"
#ifdef PLATFORM_WINRT
// Stuff from Windows Runtime API
#include <wrl.h>
#include <Windows.ApplicationModel.Core.h>
#include <windows.graphics.display.h>
#include <windows.ui.viewmanagement.h>
#endif
#if !(defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE))
// Stuff that does not belong to Windows 6.3 or previous kernels
#endif

#elif defined(PLATFORM_UNIX) || defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)
#define _FILE_OFFSET_BITS = 64
#define __USE_FILE_OFFSET64
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <unistd.h>
#include <dirent.h>
#define _vsnwprintf vswprintf
#endif

#include <XeSDK/XeDef.h>
#if defined(__cplusplus)
#include "XeDebugLog.h"
#endif

#include <cstdio>
#include <cstdlib>