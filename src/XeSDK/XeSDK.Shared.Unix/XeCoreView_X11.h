#pragma once
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace Xe {
    namespace Core {
        struct FrameView_X11_SystemWindow {
            Display* display;
            XVisualInfo* visualInfo;
            ::Window window;
        };
    }
}