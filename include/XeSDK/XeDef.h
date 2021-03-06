/* XeSDK
 * XeEngine definitions
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share without permission.
 *////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(_WIN64) || defined(_M_X64) || defined(_LP64)
#define PLATFORM_64
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

//#include <XeSDK/XeValue.h>

// Types
typedef signed char	        sbyte;
typedef signed char         s8;
typedef unsigned char       byte;
typedef unsigned char       u8;
typedef signed short        sshort;
typedef signed short        s16;
typedef unsigned short      ushort;
typedef unsigned short      u16;
typedef signed int          sint;
typedef signed int          s32;
typedef unsigned int        uint;
typedef unsigned int        u32;
typedef signed long long    s64;
typedef unsigned long long  u64;

#ifdef PLATFORM_64
typedef signed long long	svar;
typedef unsigned long long	uvar;
#else
typedef signed int			svar;
typedef unsigned int		uvar;
#endif

// Compiler compatibility
#ifdef _MSC_VER
// This prevent MSC warnings / errors
#define _CRT_SECURE_NO_WARNINGS_GLOBALS
//
#define INLINE __forceinline
#define NOOP __noop

#if _MSC_VER < 1500
// Previous MSC compilers does not support C++11's nullptr
#define nullptr (0)
#endif

#else // not MSC compiler
#define INLINE inline __attribute__((always_inline))
//#define __stdcall __attribute__((stdcall))
#ifndef STDCALL_DEFINED
#define STDCALL_DEFINED
#define __stdcall
#endif

#if (__cplusplus < 201100L)
// Previous MSC compilers does not support C++11's nullptr
#define nullptr (0)
#endif

#endif // _MSC_VER

// Utilities
#ifdef interface
#undef interface
#endif
#define interface struct

#ifdef lengthof
#undef lengthof
#endif
#define lengthof(x) (sizeof(x) / sizeof(*x))

#define NAMEOF(x) #x

#ifndef NULL
#define NULL 0
#endif

// Unicode support
#if defined(UNICODE) || defined(_UNICODE)
//#define PLATFORM_UNICODE
//#define _SS(x) _S(x)
//#define __TFILE__	_SS(__FILE__)
//#define _S(str) L ## str
//#define _T(str) L ## str
//typedef wchar_t tchar;
//typedef wchar_t* tstring;
//typedef const wchar_t* ctstring;
#define __TFILE__	__FILE__
#define _S(str) str
#define _T(str) str
typedef char tchar;
typedef char* tstring;
typedef const char* ctstring;
#else
#define __TFILE__	__FILE__
#define _S(str) str
#define _T(str) str
typedef char tchar;
typedef char* tstring;
typedef const char* ctstring;
#endif

#if defined(WINAPI_FAMILY)
#undef PLATFORM_WIN32
#undef PLATFORM_WINPHONE
#undef PLATFORM_WINRT
#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#define PLATFORM_WIN32
#elif (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP)
#define PLATFORM_WINRT
#elif (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#define PLATFORM_WINRT
#define PLATFORM_WINPHONE
#endif
#elif defined(_WIN32)
#define PLATFORM_WIN32
#endif


#define _XE_STRINGIZE_(x) #x
#define _XE_STRINGIZE(x) _XE_STRINGIZE_(x)

#if defined(PLATFORM_UNICODE)
#define _XE_WIDE_(s) L ## s
#else
#define _XE_WIDE_(s) s
#endif
#define _XE_WIDE(s) _XE_WIDE_(s)

#define _XE_CONCATENATE_(a, b) a ## b
#define _XE_CONCATENATE(a, b)  _XE_CONCATENATE_(a, b)

#define _XE_UNPARENTHESIZE_(...) __VA_ARGS__
#define _XE_UNPARENTHESIZE(...)  _XE_UNPARENTHESIZE_ __VA_ARGS__
