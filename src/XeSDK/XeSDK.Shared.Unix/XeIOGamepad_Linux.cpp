#include "pch.h"
#include <XeSDK/XeIOGamepad.h>
#include <XeSDK/XeCore.h>

#include <dirent.h>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <cerrno>

namespace Xe {
    namespace IO {
        int _TransformButton(int buttonId) {
            switch (buttonId) {
                case 0: return Button_RDown;
                case 1: return Button_RRight;
                case 2: return Button_RUp;
                case 3: return Button_RLeft;
                case 4: return Button_TriggerL1;
                case 5: return Button_TriggerR1;
                case 6: return Button_TriggerL2;
                case 7: return Button_TriggerR2;
                case 8: return Button_Select;
                case 9: return Button_Start;
                case 10: return Button_Home;
                case 11: return Button_TriggerL3;
                case 12: return Button_TriggerR3;
                default: return -1;
            }
        }

        void _RegisterButton(GamepadEntry& entry, int buttonId) {
            int button = _TransformButton(buttonId);
            if (button > 0) {
                entry.PressedButtons |= 1 << button;
            }
        }

        void _RegisterAxis(GamepadEntry& entry, int axisId) {
            switch (axisId) {
                case 0:
                    entry.Axes |= 1 << Axis_LX;
                    break;
                case 1:
                    entry.Axes |= 1 << Axis_LY;
                    break;
                case 2:
                    entry.Axes |= 1 << Axis_LZ;
                    break;
                case 3:
                    entry.Axes |= 1 << Axis_RX;
                    break;
                case 4:
                    entry.Axes |= 1 << Axis_RY;
                    break;
                case 5:
                    entry.Axes |= 1 << Axis_RZ;
                    break;
                case 6:
                    entry.PressedButtons |= 1 << Button_LLeft;
                    entry.PressedButtons |= 1 << Button_LRight;
                    break;
                case 7:
                    entry.PressedButtons |= 1 << Button_LUp;
                    entry.PressedButtons |= 1 << Button_LDown;
                    break;
            }
        }

        void _RegisterPad(GamepadEntry& entry, int fd) {
            entry.PressedButtons = 0;
            entry.Axes = 0;

            ioctl(fd, JSIOCGNAME(sizeof(GamepadEntry::DeviceName)), entry.DeviceName);
            int buttonsCount = 0, axesCount = 0;
            ioctl (fd, JSIOCGBUTTONS, &buttonsCount);
            ioctl (fd, JSIOCGAXES, &axesCount);

            struct js_event e;
            while( read(fd, &e, sizeof(e)) == sizeof(e) ) {
                if (e.type & JS_EVENT_INIT) {

                    if (e.type & JS_EVENT_BUTTON) {
                        _RegisterButton(entry, e.number);
                        buttonsCount--;
                    }
                    if (e.type & JS_EVENT_AXIS) {
                        _RegisterAxis(entry, e.number);
                        axesCount--;
                    }

                    if (buttonsCount == 0 && axesCount == 0)
                        break;
                }
            }
        }

        int QueryGamepads(GamepadEntry* pEntries, int count) {
            const char BASE_PATH[] = "/dev/input/";

            DIR *d;
            struct dirent *dir;
            int found = 0;

            if (d = opendir(BASE_PATH)) {
                while (!!(dir = readdir(d)) && found < count) {
                    if (dir->d_type == DT_CHR)
                    {
                        if (dir->d_name[0] == 'j' && dir->d_name[1] == 's' &&
                            dir->d_name[2] >= '0' && dir->d_name[2] <= '9') {
                            GamepadEntry& entry = pEntries[found++];
                            memcpy(entry.DevicePath, BASE_PATH, sizeof(BASE_PATH));
                            memcpy(entry.DevicePath + sizeof(BASE_PATH) - 1, dir->d_name, strlen(dir->d_name) + 1);

                            int fd = open(entry.DevicePath, O_RDONLY);
                            if (fd > 0) {
                                _RegisterPad(entry, fd);
                                close(fd);
                            }
                            else
                                found--;
                        }
                    }
                }
            }
            return found;
        }

        struct CGamepad : public Gamepad {
            int m_fd;

            CGamepad(const GamepadEntry& entry)
            {
                PressedButtons.Data = 0;
                m_fd = open(entry.DevicePath, O_RDONLY | O_NONBLOCK);
                if (m_fd > 0) {
                    GamepadEntry tmpEntry;
                    _RegisterPad(tmpEntry, m_fd);

                    ButtonsSupported = tmpEntry.PressedButtons;
                    AxesSupported = tmpEntry.Axes;
                    IsRumblingSupported = false;
                    IsLightbarSuported = false;
                    IsAudioSupported = false;
                }
            }

            ~CGamepad() {
                if (m_fd > 0)
                    close(m_fd);
            }

            bool IsAvailable() const {
                return m_fd > 0;
            }

            void Update() {
                if (m_fd > 0) {
                    struct js_event e;
                    while( read(m_fd, &e, sizeof(e)) == sizeof(e) ) {
                        ProcessEvent(e);
                    }

                    if (errno != EAGAIN) {
                        LOGW("Pad disconnected (errno: %08X).\n", errno);
                        close(m_fd);
                        m_fd = 0;
                    }

                }
            }

            void SetRumbling(const Math::Vector2f& rumbling) {

            }
            void SetLightbar(const Graphics::Color& color) {

            }

            void ProcessEvent(struct js_event e) {
                const float DIVISORF = 32767.0f;

                int id = e.number;
                switch (e.type) {
                    case JS_EVENT_BUTTON:
                        id = _TransformButton(id);
                        if (id > 0) {
                            if (e.value == 0) {
                                PressedButtons.Data &= ~(1 << id);
                            }
                            else {
                                PressedButtons.Data |= 1 << id;
                            }
                        }
                        break;
                    case JS_EVENT_AXIS:
                        switch (e.number) {
                            case 0:
                                LAxis.x = e.value / DIVISORF;
                                break;
                            case 1:
                                LAxis.y = e.value / DIVISORF;
                                break;
                            case 2:
                                LAxis.z = e.value / DIVISORF;
                                break;
                            case 3:
                                RAxis.x = e.value / DIVISORF;
                                break;
                            case 4:
                                RAxis.y = e.value / DIVISORF;
                                break;
                            case 5:
                                RAxis.z = e.value / DIVISORF;
                                break;
                            case 6:
                                if (e.value < 0) {
                                    PressedButtons.LRight = 0;
                                    PressedButtons.LLeft = 1;
                                }
                                else if (e.value > 0) {
                                    PressedButtons.LRight = 1;
                                    PressedButtons.LLeft = 0;
                                }
                                else {
                                    PressedButtons.LRight = 0;
                                    PressedButtons.LLeft = 0;
                                }
                                break;
                            case 7:
                                if (e.value < 0) {
                                    PressedButtons.LUp = 1;
                                    PressedButtons.LDown = 0;
                                }
                                else if (e.value > 0) {
                                    PressedButtons.LUp = 0;
                                    PressedButtons.LDown = 1;
                                }
                                else {
                                    PressedButtons.LUp = 0;
                                    PressedButtons.LDown = 0;
                                }
                                break;
                        }
                        break;
                }
            }
        };

        void CreateGamepad(const GamepadEntry& entry, Gamepad** ppGamepad) {
            *ppGamepad = new CGamepad(entry);
        }
    }
}
