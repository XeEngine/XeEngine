#include "pch.h"
#include <XeSDK/XeMemory.h>
#include <cstdlib>

void* Xe::Memory::Alloc(uvar length) {
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
	return HeapAlloc(GetProcessHeap(), 0, length);
#else
	return malloc(length);
#endif
}

void* Xe::Memory::AllocZero(uvar length) {
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, length);
#else
	return calloc(1, length);
#endif
}

void* Xe::Memory::Resize(void* mem, uvar length) {
	if (mem == nullptr)
		return Alloc(length);
	else
	{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
		return HeapReAlloc(GetProcessHeap(), 0, mem, length);
#else
		return realloc(mem, length);
#endif
	}
}

void* Xe::Memory::Free(void* mem) {
	if (mem != nullptr)
	{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
		HeapFree(GetProcessHeap(), 0, mem);
#else
		free(mem);
#endif
	}
	return nullptr;
}

void Xe::Memory::Copy(void* dst, const void* src, uvar length) {
	if (src == 0 || dst == 0 || length == 0) return;
	memcpy(dst, src, length);
}
void Xe::Memory::Move(void* dst, const void* src, uvar length) {
	if (src == 0 || dst == 0 || length == 0) return;
	memmove(dst, src, length);
}

void Xe::Memory::Fill(void* dst, uvar value, uvar length) {
	memset(dst, (int)value, (size_t)length);
}
void Xe::Memory::Fill(void* dst, uvar value, svar position, uvar length) {
	memset((u8*)dst + position, (int)value, (size_t)length);
}

svar Xe::Memory::Compare(const void* mem1, svar pos1, const void* mem2, svar pos2, uvar length) {
	return memcmp((tchar*)mem1 + pos1, (tchar*)mem2 + pos2, length);
}
