#pragma once

#define STR(x) #x
#define VERSION(vermaj, vermin, verrev) (STR(vermaj) "." STR(vermin) "." STR(verrev))

#define ENGINE_NAME "XeEngine"
#define ENGINE_DEVELOPER "Luciano Ciccariello (Xeeynamo)"
#define ENGINE_VERSION VERSION(XESDK_VER_MAJ, XESDK_VER_MIN, XESDK_VER_BUILD)

#if defined(_MSC_VER)
#define COMPILER "MSVC"
#define COMPILER_VERSION _MSC_VER

#elif defined(__GCC__)
#define COMPILER "GCC"
#define COMPILER_VERSION (__GNUC__ * 10000 \
	+ __GNUC_MINOR__ * 100 \
	+ __GNUC_PATCHLEVEL__)

#else
#define COMPILER "UNK"
#define COMPILER_VERSION STR(0)
#endif

#if defined(_M_ARM64) || defined(__aarch64__)
#define CPUARCH CpuArch_ARM64
#elif defined(_M_ARM) || defined(__arm__)
#define CPUARCH CpuArch_ARM
#elif defined(_M_X64) || defined(_LP64) || defined(__x86_64__)
#define CPUARCH CpuArch_x64
#elif defined(_M_IX86) || defined(__i386__) || defined(_X86_)
#define CPUARCH CpuArch_x86
#else
#define CPUARCH CpuArch_Unknown
#endif