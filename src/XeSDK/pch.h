#pragma once
#if defined(PLATFORM_UWP)
#define SETTINGS_LOADLIBRARY 0
#define PLATFORM_WINRT
#elif defined(PLATFORM_WIN32)
#define SETTINGS_LOADLIBRARY 1
#endif
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINRT)
#define PLATFORM_WINDOWS
#endif

#ifdef PLATFORM_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
#endif
#include <SDKDDKVer.h>
#include <windows.h>
//#include <d3d11.h>
//#include <d3d11_1.h>
#include "ShaderD3D11_VSDefault.h"
#include "ShaderD3D11_FSDefault.h"

#if WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE
// Stuff from Windows Runtime API
#include <wrl.h>
#include <Windows.ApplicationModel.Core.h>
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
#include <windows.graphics.display.h>
#endif
#include <windows.ui.viewmanagement.h>
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

#if defined(_DEBUG) || defined(DEVELOPMENT)
#include <cassert>
#define ASSERT assert
#else
#define ASSERT
#endif