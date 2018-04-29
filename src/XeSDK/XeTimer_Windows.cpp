#include "pch.h"
#include <XeSDK/XeTimer.h>

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)

namespace Xe {
	struct XeTimer_Zero {
		LARGE_INTEGER cpuFrequency;
		LARGE_INTEGER startTime;
		XeTimer_Zero() {
			QueryPerformanceFrequency(&cpuFrequency);
			QueryPerformanceCounter(&startTime);
		}
	};
	XeTimer_Zero xetimer_base;

	Timer Timer::Zero() {
		Timer t;
		t.m_time = xetimer_base.startTime.QuadPart;
		return t;
	}
	Timer Timer::Current() {
		Timer t;
		QueryPerformanceCounter((LARGE_INTEGER*)&t.m_time);
		t.m_time -= xetimer_base.startTime.QuadPart;
		return t;
	}
	s64 Timer::AsInteger() const {
		return m_time * 1000000LL / xetimer_base.cpuFrequency.QuadPart;
	}
	double Timer::AsDouble() const {
		return (double)m_time / (double)xetimer_base.cpuFrequency.QuadPart;
	}
}

#endif