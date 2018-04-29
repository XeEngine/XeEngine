#include "pch.h"
#include <XeSDK/XeMemory.h>
#include <cstdlib>

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
