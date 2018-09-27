#pragma once
#include <XeSDK/XeDef.h>
#include <XeGame/XeGameEaseFunctions.h>
#include <vector>

namespace Xe { namespace Sound {
	interface IAudioSource;
} }

namespace Xe { namespace IO {
	interface IStream;
} }

namespace Xe { namespace Game {
	interface ISoundBgmEntity;
	interface ISoundSfxEntity;
	interface ISoundManager : public virtual IObject
	{
		virtual ~ISoundManager() = 0 { }

		//! \brief Update the logic for all the sound entities.
		//! \details This is required to have the fadein and fadeout logic working.
		virtual void Update(double deltaTime) = 0;

		//! \brief Get the current number of BGM.
		virtual int GetBgmCount() const = 0;

		//! \brief Get the maximum number of BGM that can be stacked.
		virtual int GetMaxBgmCount() const = 0;

		//! \brief Get the current number of SFX.
		virtual int GetSfxCount() const = 0;

		//! \brief Get the maximum number of SFX that can be played at the same time.
		virtual int GetMaxSfxCount() const = 0;

		//! \brief Get the overall volume.
		virtual float GetMasterVolume() const = 0;

		//! \brief Set the overall volume.
		virtual void SetMasterVolume(float volume) = 0;

		//! \brief Get the volume used for BGMs.
		virtual float GetBgmVolume() const = 0;
	
		//! \brief Set the volume used for BGMs.
		virtual void SetBgmVolume(float volume) = 0;

		//! \brief Get the volume used for SFXs.
		virtual float GetSfxVolume() const = 0;

		//! \brief Set the volume used for SFXs.
		virtual void SetSfxVolume(float volume) = 0;

		//! \brief Get the list of stacked BGMs.
		/** \details The returned ISoundBgmEntity could be deallocated
		 * at any time if Update, PushBgm, PopBgm or StopAllBgm are called.
		 */
		virtual std::vector<ISoundBgmEntity*> GetBgms() const = 0;

		//! \brif Get the list of playing SFXs.
		/** \details The returned ISoundSfxEntity could be deallocated
		 * at any time if Update, PlaySfx or StopAllSfx are called.
		 */
		virtual std::vector<ISoundSfxEntity*> GetSfxs() const = 0;

		//! \brief Push the specified BGM into the BGM's stack, pausing the previous track.
		//! \param bgmId An unique identifier for the specified source.
		//! \param source Audio source where to get the playing sound.
		//! \param speed Fading speed for the music to play and the previous one.
		//! \param ease Ease algorithm used for fading.
		//! \return The new BGM above the stack. Can be nullptr.
		//! \details If the previous popped music has the same bgmId then it will be resumed.
		virtual ISoundBgmEntity* PushBgm(
			u64 bgmId,
			Xe::Sound::IAudioSource& source,
			float seconds = 1.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		//! \brief Pop from the BGM's stack the executing music.
		//! \param pause Specify if the music must be paused instead of stopping it.
		//! \param speed Fading speed for the music to play and the previous one.
		//! \param ease Ease algorithm used for fading.
		//! \return The new BGM above the stack. Can be nullptr.
		/** \details The music is not removed when this function is called. Instead it
		 * does remain outside the stack and, when the same bgmId is going to be pushed
		 * again, the same IAudioSource can be recycled. If pause = true, the music
		 * can be even resumed.
		 * If this will be pop out and another bgmId is going to be pushed, the popped
		 * music will be removed.
		 */
		virtual ISoundBgmEntity* PopBgm(
			bool pause = false,
			float seconds = 1.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;
		virtual ISoundSfxEntity* PlaySfx(Xe::Sound::IAudioSource& source, int priority) = 0;

		//! \brief Same as PushBgm, but just replace the existing BGM.
		//! \param bgmId An unique identifier for the specified source.
		//! \param source Audio source where to get the playing sound.
		//! \param speed Fading speed for the music to play and the previous one.
		//! \param ease Ease algorithm used for fading.
		/** \details The playing BGM will be stopped with the specified fade-out
		 * and pushed one position above the stack, but without incrementing the
		 * number of BGM; in this way the previous BGM can be faded and stopped.
		 */
		virtual ISoundBgmEntity* PlayBgm(
			u64 bgmId,
			Xe::Sound::IAudioSource& source,
			float seconds = 1.0f,
			EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		//! \brief Get the playing BGM.
		//! \return The current BGM above the stack. Can be nullptr.
		virtual ISoundBgmEntity* PeekBgm() const = 0;

		virtual void StopAllSfxs(float seconds = 0.0f, EaseFuncf ease = Xe::Game::Ease::Linear) = 0;

		//virtual const Xe::Math::Vector3f& GetListenerPosition() const = 0;
		//virtual void SetListenerPosition(const Xe::Math::Vector3f& position) = 0;
		//virtual float GetListenerRange() const = 0;
		//virtual void SetListenerRange(float range) = 0;
	};
} }