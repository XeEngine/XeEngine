#pragma once

#ifdef _WIN32
#include "pch_winapi.h"

#elif defined(PLATFORM_UNIX) || defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)
#define _FILE_OFFSET_BITS = 64
#define __USE_FILE_OFFSET64
#define _vsnwprintf vswprintf
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <unistd.h>
#include <dirent.h>

#else
#error Platform or SDK not supported

#endif

#include <XeSDK/XeDef.h>
#include <XeSDK/XeLogger.h>

#include <cstdio>
#include <cstdlib>

#if defined(_DEBUG) && !defined(DEVELOPMENT)
#define DEVELOPMENT
#endif

#if defined(DEVELOPMENT)
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