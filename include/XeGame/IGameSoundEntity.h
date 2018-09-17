#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeMathVector3.h>
#include <XeGame/XeGameEaseFunctions.h>
#include <list>

namespace Xe { namespace Game {
	struct SoundLoop
	{
		int Start, End;
	};

	interface ISoundEntity
	{
		virtual ~ISoundEntity() = 0 { }

		//! \brief Get the current volume.
		virtual float GetVolume() const = 0;

		//! \brief Change the volume with a specified ease function.
		//! \param[in] speed Speed of fading; can be negative.
		//! \param[in] ease Ease function used for fading; default is Ease::One.
		/** \details Fade state is updated only when Update is launched. The value
		 * from GetMasterVolume does not change, but GetVolume does.
		 * When Fade is launched, the X passed as argument goes between GetVolume
		 * and the volume parameter.
		 */
		virtual void SetVolume(float volume,
			float speed = 1.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		virtual const Xe::Math::Vector3f& GetPosition() const = 0;
		virtual void SetPosition(const Xe::Math::Vector3f& position) = 0;

		//! \brief Check if a sound is currently playing.
		virtual bool IsPlaying() const = 0;

		//! \brief Play the current sound.
		//! \details If it is already playing, then do nothing.
		virtual void Play(float speed = 0.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		//! \brief Pause the current playing sound.
		//! \details If it is paused, then do nothing.
		virtual void Pause(float speed = 0.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		//! \brief Pause the current playing sound and go to the beginning.
		virtual void Stop(float speed = 0.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		//! \brief Specify a group of loops.
		//! \param[in] loops Array of loops. Ignored if loopsCount is 0.
		//! \param[in] loopsCount Number of loops in the array.
		/** \details Once a set of loops has been specified, the LoopIndex will
		 * be set to 0.
		 */
		virtual void SetLoops(const SoundLoop* loops, int loopsCount) = 0;

		//! \brief Returns the loopsCount specified by SetLoops; 0 by default.
		virtual int GetLoopsCount() const = 0;

		//! \brief Get the current loop index.
		virtual int GetLoopIndex() const = 0;

		//! \brief Advance by one the loop index, if less than loopsCount.
		virtual void SetNextLoopIndex() = 0;
	};
} }
