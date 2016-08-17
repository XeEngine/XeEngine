#pragma once
#include <XeSDK/XeGraphicsColor.h>
#include <XeSDK/XeMathVector2.h>

namespace Xe {
	namespace IO {
		class Gamepad {
		public:
			static const svar MaxGamepads = 8;

			struct State {
				union {
					struct {
						// left digital pad
						u32 LPad_Up : 1;
						u32 LPad_Down : 1;
						u32 LPad_Left : 1;
						u32 LPad_Right : 1;
						// right digital pad
						u32 RPad_Up : 1;
						u32 RPad_Down : 1;
						u32 RPad_Left : 1;
						u32 RPad_Right : 1;
						// triggers
						u32 LTrigger_1 : 1;
						u32 LTrigger_2 : 1;
						u32 RTrigger_1 : 1;
						u32 RTrigger_2 : 1;
						// buttons on analogs
						u32 LTrigger_3 : 1;
						u32 RTrigger_3 : 1;
						// buttons on center pad
						u32 BPause : 1;
						u32 BMenu : 1;
					};
					u32 Buttons;
				};
				//! \brief Bit mask of avaliable buttons.
				u32 AvailableButtons;
				//! \brief Left analog.
				Math::Vector2f Left;
				//! \brief Right analog.
				Math::Vector2f Right;
				//! \brief Left trigger pressure.
				float LTrigger_Pressure;
				//! \brief Right trigger pressure.
				float RTrigger_Pressure;
			};
			struct Properties {
				//! \brief Used to vibrate the two vibration engines.
				Math::Vector2f Vibration;
				//! \brief Used to set a color to the gamepad.
				/** \details Currently only DS4 supports this feature.
				 * #RealSonaro
				 */
				Graphics::Color BarColor;
			};

			//! \brief Process the current state of specified pad.
			//! \param[out] state of gamepad.
			//! \param[in] index of gamepad to get the status. [0, MaxGamepads)
			//! \return false if gamepad is disconnected.
			/** \details When this function is called it tried to get the
			 * current state of specified gamepad as soon as possible. This
			 * function is slow on some devices, so should not be called
			 * more than one time per frame.
			 */
			static bool ProcessState(State& state, svar index);

			//! \brief Get the last processed state of specified pad,
			//! \param[out] state of gamepad.
			//! \param[in] index of gamepad to get the status. [0, MaxGamepads)
			//! \return false if gamepad is disconnected.
			/** \details This version get the last processed state of specified
			 * gamepad, processed as fast as possible from a different thread.
			 * The refresh update depends from gamepad device or OS.
			 */
			static bool GetState(State& state, svar index);

			//! \brief Set gamepad properties
			//! \param[in] properties of gamepad to set
			//! \param[in] index of gamepad. [0, MaxGamepads)
			static void SetProperties(const Properties& properties, svar index);
		};
	}
}