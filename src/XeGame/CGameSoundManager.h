#pragma once
#include <XeGame/IGameSoundManager.h>
#include <XeGame/XeGameEaseFunctions.h>

namespace Xe { namespace Sound {
	interface IAudio;
} }

namespace Xe { namespace Game {
	interface CSoundBgmEntity;
	interface CSoundSfxEntity;
	class CSoundManager : public ISoundManager
	{
		Xe::Sound::IAudio& m_Audio;
		const int m_MaxBgmCount;
		const int m_MaxSfxCount;
		int m_BgmCount;
		float m_MasterVolume;
		float m_BgmVolume;
		float m_SfxVolume;

		CSoundBgmEntity** m_Bgms;
		CSoundSfxEntity** m_Sfxs;
		
		Xe::Math::Vector3f m_ListenerPosition;
		float m_ListenerRange;

		void UpdateBgmVolumes();
		void UpdateSfxVolumes();

		bool IsSfxSlotGarbage(int sfxSlot) const;
	public:
		CSoundManager(Xe::Sound::IAudio& audio, int maxBgmCount, int maxSfxCount);
		~CSoundManager();

		void Update(double deltaTime);

		int GetBgmCount() const;
		int GetMaxBgmCount() const;

		int GetSfxCount() const;
		int GetMaxSfxCount() const;

		float GetMasterVolume() const;
		void SetMasterVolume(float volume);

		float GetBgmVolume() const;
		void SetBgmVolume(float volume);

		float GetSfxVolume() const;
		void SetSfxVolume(float volume);

		std::vector<ISoundBgmEntity*> GetBgms() const;
		std::vector<ISoundSfxEntity*> GetSfxs() const;

		const Xe::Math::Vector3f& GetListenerPosition() const;
		void SetListenerPosition(const Xe::Math::Vector3f& position);
		float GetListenerRange() const;
		void SetListenerRange(float range);

		ISoundBgmEntity* PushBgm(
			u64 bgmId, Xe::Sound::IAudioSource& source,
			float speed, EaseFuncf ease);
		ISoundBgmEntity* PopBgm(bool pause,
			float speed, EaseFuncf ease);
		ISoundBgmEntity* PlayBgm(
			u64 bgmId, Xe::Sound::IAudioSource& source,
			float speed, EaseFuncf ease);
		ISoundBgmEntity* PeekBgm() const;
		
		ISoundSfxEntity* PlaySfx(Xe::Sound::IAudioSource& source, int priority);

		void StopAllSfxs(float speed, EaseFuncf ease);
	};
} }