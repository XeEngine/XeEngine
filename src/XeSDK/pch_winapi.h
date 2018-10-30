#pragma once

#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS
#endif

#ifndef HR
#define HR(x) (ASSERT(SUCCEEDED((x))))
#endif

#include <SDKDDKVer.h>
#include <windows.h>
#include "ShaderD3D11_VSDefault.h"
#include "ShaderD3D11_FSDefault.h"

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP

#define SETTINGS_LOADLIBRARY 1

#include <d3d11.h>
#include <d3d11_1.h>

#elif WINAPI_FAMILY == WINAPI_FAMILY_PC_APP || WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE

#define SETTINGS_LOADLIBRARY 0
#define PLATFORM_WINRT

// Stuff from Windows Runtime API
#include <wrl.h>
#include <Windows.ApplicationModel.Core.h>
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
#include <windows.graphics.display.h>
#endif
#include <windows.ui.viewmanagement.h>

#if WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
#include <d3d11.h>
#include <d3d11_1.h>
#endif

#else

#error The current WINAPI_FAMILY is not supported.

#endif