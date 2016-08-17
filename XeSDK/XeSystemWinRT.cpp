#include "pch.h"

#if defined(PLATFORM_WINRT)
extern "C" VOID WINAPI ExitProcess(_In_ UINT uExitCode) {

}

extern "C" VOID WINAPI _ExitProcess(_In_ UINT uExitCode) {

}
#endif