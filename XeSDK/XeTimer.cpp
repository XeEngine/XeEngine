#include "pch.h"
#include <XeSDK/XeTimer.h>

namespace Xe {
	Timer Timer::operator+(const Timer& x) const {
		Timer t;
		t.m_time = m_time + x.m_time;
		return t;
	}
	Timer Timer::operator-(const Timer& x) const {
		Timer t;
		t.m_time = m_time - x.m_time;
		return t;
	}
	const Timer& Timer::operator=(const Timer& x) {
		m_time = x.m_time;
		return *this;
	}
	const Timer& Timer::operator+=(const Timer& x) {
		m_time += x.m_time;
		return *this;
	}
	const Timer& Timer::operator-=(const Timer& x) {
		m_time -= x.m_time;
		return *this;
	}
	bool Timer::operator==(const Timer& x) {
		return m_time == x.m_time;
	}
	bool Timer::operator!=(const Timer& x) {
		return m_time != x.m_time;
	}
	bool Timer::operator>(const Timer& x) {
		return m_time > x.m_time;
	}
	bool Timer::operator>=(const Timer& x) {
		return m_time >= x.m_time;
	}
	bool Timer::operator<(const Timer& x) {
		return m_time < x.m_time;
	}
	bool Timer::operator<=(const Timer& x) {
		return m_time <= x.m_time;
	}
}