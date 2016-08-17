#include "pch.h"
#include <XeSDK/XeTime.h>
#include <time.h>

struct XeTime_Windows_Init {
	timespec t;
	XeTime_Windows_Init() {
		clock_gettime(CLOCK_MONOTONIC, &t);
	}
};
XeTime_Windows_Init xetime_windows_init;

u64 Xe::Time::GetCurrentTime() {
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (u64)t.tv_sec * 1000000ULL + (u64)t.tv_nsec / 1000ULL;
}

Xe::Time::Time() {
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	if (t.tv_nsec < xetime_windows_init.t.tv_nsec) {
		t.tv_nsec += 999999999UL;
		t.tv_sec--;
	}
	t.tv_nsec -= xetime_windows_init.t.tv_nsec;
	t.tv_sec -= xetime_windows_init.t.tv_sec;
	m_time = (u64)t.tv_nsec + (u64)t.tv_sec * 1000000000ULL;
}
u64 Xe::Time::GetTime() const {
	return m_time / 1000ULL;
}
u64 Xe::Time::Distance(const Time& time) const {
	return (m_time - time.m_time) / 1000ULL;
}