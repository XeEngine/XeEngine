#include "pch.h"
#include <XeSDK/XeTimer.h>

namespace Xe {
    Timer Timer::Zero() {
        Timer t;
        t.m_time = 0;
        return t;
    }
    Timer Timer::Current() {
        Timer t;
        t.m_time -= 0;
        return t;
    }
    s64 Timer::AsInteger() const {
        return 0;
    }
    double Timer::AsDouble() const {
        return 0.0;
    }
}
