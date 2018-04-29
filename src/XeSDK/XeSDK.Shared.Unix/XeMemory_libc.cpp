#include "pch.h"
#include <XeSDK/XeMemory.h>
#include <cstring>

void* Xe::Memory::Alloc(uvar length) {
	return malloc(length);
}

void* Xe::Memory::AllocZero(uvar length) {
	return calloc(1, length);
}

void* Xe::Memory::Resize(void* mem, uvar length) {
	if (mem == nullptr)
		return Alloc(length);
	return realloc(mem, length);
}

void* Xe::Memory::Free(void* mem) {
	if (mem != nullptr)
		free(mem);
	return nullptr;
}
