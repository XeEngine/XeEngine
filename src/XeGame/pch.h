#pragma once
#include <XeSDK.h>

#if defined(_DEBUG) && !defined(DEVELOPMENT)
#define DEVELOPMENT
#endif

#if defined(_DEBUG) || defined(DEVELOPMENT)
#include <cassert>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif