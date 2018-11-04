#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeGraphicsColor.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeMathVector3.h>

namespace Xe {
	namespace IO {
		const int Button_LUp = 0;
		const int Button_LDown = 1;
		const int Button_LLeft = 2;
		const int Button_LRight = 3;
		const int Button_RUp = 4;
		const int Button_RDown = 5;
		const int Button_RLeft = 6;
		const int Button_RRight = 7;
		const int Button_TriggerL1 = 8;
		const int Button_TriggerR1 = 9;
		const int Button_TriggerL2 = 10;
		const int Button_TriggerR2 = 11;
		const int Button_TriggerL3 = 12;
		const int Button_TriggerR3 = 13;
		const int Button_Select = 14;
		const int Button_Start = 15;
		const int Button_Home = 16;

		const int Axis_LX = 0;
		const int Axis_LY = 1;
		const int Axis_LZ = 2;
		const int Axis_RX = 3;
		const int Axis_RY = 4;
		const int Axis_RZ = 5;

        struct Gamepad : public Xe::IObject {
			union Buttons {
				struct {
					u32 LUp : 1;
					u32 LDown : 1;
					u32 LLeft : 1;
					u32 LRight : 1;
					u32 RUp : 1;
					u32 RDown : 1;
					u32 RLeft : 1;
					u32 RRight : 1;
					u32 TriggerL1 : 1;
					u32 TriggerR1 : 1;
					u32 TriggerL2 : 1;
					u32 TriggerR2 : 1;
					u32 TriggerL3 : 1;
					u32 TriggerR3 : 1;
					u32 Select : 1;
					u32 Start : 1;
					u32 Home : 1;
					u32 Tap : 1;
				};
				u32 Data;
			};
            union Capabilities {
                struct {
                    // Buttons
                    u32 LUp : 1;
                    u32 LDown : 1;
                    u32 LLeft : 1;
                    u32 LRight : 1;
                    u32 RUp : 1;
                    u32 RDown : 1;
                    u32 RLeft : 1;
                    u32 RRight : 1;
                    u32 TriggerL1 : 1;
                    u32 TriggerR1 : 1;
                    u32 TriggerL2 : 1;
                    u32 TriggerR2 : 1;
                    u32 TriggerL3 : 1;
                    u32 TriggerR3 : 1;
                    u32 Select : 1;
                    u32 Start : 1;
                    u32 Home : 1;
                    u32 Tap : 1;

                    // Axes
                    u32 LX : 1;
                    u32 LY : 1;
                    u32 LZ : 1;
                    u32 RX : 1;
                    u32 RY : 1;
                    u32 RZ : 1;

                    // Etc.
                    u32 RumbleX : 1;
                    u32 RumbleY : 1;
                    u32 Gyroscope: 1;
                    u32 Accelerometer : 1;
                    u32 Battery : 1;
                    u32 Audio : 1;
                    u32 Lightbar : 1;
                    u32 Touchpad : 1;

                };
                u32 Data;
            };

            Capabilities Capabilities;
            Buttons PressedButtons;
            Math::Vector3f LAxis;
            Math::Vector3f RAxis;
            Math::Vector3f Gyroscope;
            Math::Vector3f Accelerometer;
            Math::Vector2f Touchpad;
            float Battery;
			bool IsBatteryCharging;

            virtual ~Gamepad() {}

            //! \brief Check if the device is available or lost.
            //! \details A lost device is not recoverable.
            virtual bool IsAvailable() const = 0;

            //! \brief Update joypad state.
			virtual void Update() = 0;

            virtual void SetRumbling(const Math::Vector2f& rumbling) = 0;

            virtual void SetLightbar(const Graphics::Color& color) = 0;
        };

		typedef u64 GamepadEntry;

		enum class GamepadResult {
			Ok,
			NotFound = -1,
			NotSupported = -2,
			Busy = -3,
		};

		//! \brief Open the specified gamepad.
		//! \param[in] entry Gamepad entry that was returned by QueryGamepads.
		//! \param[out] ppGamepad The opened gamepad; Cannot be nullptr.
		//! \return true if the gamepad was created with success, else false.
		bool OpenDevice(
			Xe::IO::Gamepad** ppGamepad,
			const Xe::IO::GamepadEntry& entry);

		//! \brief Get all the connected gamepads
		//! \param[out] pEntries Contiguous memory that will be populated by connected pads.
		//! \param[in] index Start index of pEntries
		//! \param[in] count Maximum number of gamepads to retrieve; GamepadEntry[count].
		size_t EnumerateDevices(
			Xe::IO::GamepadEntry* pEntries,
			size_t index,
			size_t count);
	}
}
