﻿#pragma once
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
#define HR(x) (ASSERT(SUCCEEDED((x))))

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
#undef SETTINGS_LOADLIBRARY
#define SETTINGS_LOADLIBRARY 0

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
#include <XeSDK/XeLogger.h>

#include <cstdio>
#include <cstdlib>

#if defined(_DEBUG) || defined(DEVELOPMENT)
#include <cassert>
#define ASSERT assert
#else
#define ASSERT
#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) | \
	((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 ))
#endif // defined(MAKEFOURCC)