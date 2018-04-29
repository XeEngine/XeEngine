#include "pch.h"
#include <XeSDK/XeMemory.h>
#include <cstring>
#include <Windows.h>

void* Xe::Memory::Alloc(uvar length) {
	return HeapAlloc(GetProcessHeap(), 0, length);
}

void* Xe::Memory::AllocZero(uvar length) {
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, length);
}

void* Xe::Memory::Resize(void* mem, uvar length) {
	if (mem == nullptr)
		return Alloc(length);
	return HeapReAlloc(GetProcessHeap(), 0, mem, length);
}

void* Xe::Memory::Free(void* mem) {
	if (mem != nullptr)
		HeapFree(GetProcessHeap(), 0, mem);
	return nullptr;
}
