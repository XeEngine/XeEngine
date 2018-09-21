#include "pch.h"
#include <XeGame/XeGameSoundManager.h>
#include "CGameSoundManager.h"
#include "CGameSoundBgmEntity.h"
#include "CGameSoundSfxEntity.h"

namespace Xe { namespace Game {
	CSoundManager::CSoundManager(
		Xe::Sound::IAudio& audio, int maxBgmCount, int maxSfxCount) :
		m_Audio(audio),
		m_MaxBgmCount(maxBgmCount),
		m_MaxSfxCount(maxSfxCount),
		m_BgmCount(0),
		m_MasterVolume(1.0f),
		m_BgmVolume(1.0f),
		m_SfxVolume(1.0f)
	{
		m_Audio.AddRef();

		m_Bgms = (CSoundBgmEntity**)Xe::Memory::AllocZero(m_MaxBgmCount * sizeof(m_Bgms));
		m_Sfxs = (CSoundSfxEntity**)Xe::Memory::AllocZero(m_MaxSfxCount * sizeof(m_Sfxs));
	}

	CSoundManager::~CSoundManager()
	{
		for (int i = 0; i < m_MaxBgmCount; i++)
		{
			if (m_Bgms[i])
			{
				m_Bgms[i]->Release();
			}
		}

		for (int i = 0; i < m_MaxSfxCount; i++)
		{
			if (m_Sfxs[i])
			{
				delete m_Sfxs[i];
			}
		}

		m_Audio.Release();
		Xe::Memory::Free(m_Bgms);
		Xe::Memory::Free(m_Sfxs);
	}

	void CSoundManager::Update(double deltaTime)
	{
		for (int i = 0; i < m_MaxBgmCount; i++)
		{
			if (m_Bgms[i])
			{
				m_Bgms[i]->Update(deltaTime);
			}
		}

		for (int i = 0; i < m_MaxSfxCount; i++)
		{
			if (!IsSfxSlotGarbage(i))
			{
				m_Sfxs[i]->Update(deltaTime);
			}
		}
	}

	int CSoundManager::GetBgmCount() const
	{
		return m_BgmCount;
	}

	int CSoundManager::GetMaxBgmCount() const
	{
		return m_MaxBgmCount;
	}

	int CSoundManager::GetSfxCount() const
	{
		int count = 0;

		for (int i = 0; i < m_MaxSfxCount; i++)
		{
			if (!IsSfxSlotGarbage(i))
			{
				count++;
			}
		}

		return count;
	}

	int CSoundManager::GetMaxSfxCount() const
	{
		return m_MaxSfxCount;
	}

	float CSoundManager::GetMasterVolume() const
	{
		return m_MasterVolume;
	}

	void CSoundManager::SetMasterVolume(float volume)
	{
		m_MasterVolume = volume;
		UpdateBgmVolumes();
		UpdateSfxVolumes();
	}

	float CSoundManager::GetBgmVolume() const
	{
		return m_BgmVolume;
	}

	void CSoundManager::SetBgmVolume(float volume)
	{
		m_BgmVolume = volume;
		UpdateBgmVolumes();
	}

	float CSoundManager::GetSfxVolume() const
	{
		return m_SfxVolume;
	}

	void CSoundManager::SetSfxVolume(float volume)
	{
		m_SfxVolume = volume;
		UpdateSfxVolumes();
	}

	std::vector<ISoundBgmEntity*> CSoundManager::GetBgms() const
	{
		std::vector<ISoundBgmEntity*> list(m_BgmCount);
		for (int i = 0; i < m_BgmCount; i++)
		{
			list.push_back(m_Bgms[i]);
		}

		return list;
	}

	std::vector<ISoundSfxEntity*> CSoundManager::GetSfxs() const
	{
		std::vector<ISoundSfxEntity*> list(GetSfxCount());
		for (int i = 0; i < m_MaxSfxCount; i++)
		{
			if (!IsSfxSlotGarbage(i))
			{
				list.push_back(m_Sfxs[i]);
			}
		}

		return list;
	}

	const Xe::Math::Vector3f& CSoundManager::GetListenerPosition() const
	{
		return m_ListenerPosition;
	}

	void CSoundManager::SetListenerPosition(const Xe::Math::Vector3f& position)
	{
		m_ListenerPosition = position;
	}

	float CSoundManager::GetListenerRange() const
	{
		return m_ListenerRange;
	}

	void CSoundManager::SetListenerRange(float range)
	{
		if (m_ListenerRange != range)
		{
			m_ListenerRange = range;
		}
	}

	void CSoundManager::UpdateBgmVolumes()
	{
		for (int i = 0; i < m_BgmCount; i++)
		{
			m_Bgms[i]->UpdateVolume();
		}
	}

	void CSoundManager::UpdateSfxVolumes()
	{
		for (int i = 0; i < m_MaxSfxCount; i++)
		{
			if (!IsSfxSlotGarbage(i))
			{
				m_Sfxs[i]->UpdateVolume();
			}
		}
	}

	ISoundBgmEntity* CSoundManager::PushBgm(
		u64 bgmId,
		Xe::Sound::IAudioSource& source,
		float speed, EaseFuncf ease)
	{
		CSoundBgmEntity* pBgm = nullptr;

		if (m_BgmCount < m_MaxBgmCount)
		{
			// pause the executing music
			if (m_BgmCount > 0)
			{
				m_Bgms[m_BgmCount - 1]->Pause(speed, ease);
			}

			// Check if the same Bgm ID was previously pushed, so we can recycle it
			if (m_Bgms[m_BgmCount] && m_Bgms[m_BgmCount]->GetBgmId() == bgmId)
			{
				pBgm = m_Bgms[m_BgmCount];
				m_Bgms[m_BgmCount++]->Play(speed, ease);
			}
			else
			{
				if (!m_Bgms[m_BgmCount])
				{
					m_Bgms[m_BgmCount] = new CSoundBgmEntity(*this, bgmId);
				}

				pBgm = m_Bgms[m_BgmCount];
				if (pBgm->SetAudioSource(source))
				{
					pBgm->Play(speed, ease);
					m_BgmCount++;

					// Pause the music above.
					if (m_Bgms[m_BgmCount])
					{
						m_Bgms[m_BgmCount]->Pause(0.0f, Xe::Game::Ease::Linear);
					}
				}
				else
				{
					delete pBgm;
					pBgm = nullptr;
				}
			}
		}
		else
		{
			pBgm = nullptr;
			LOGE("Unable to push, maximum number of BGM reached: %i.", m_BgmCount);
		}

		return pBgm;
	}

	ISoundBgmEntity* CSoundManager::PopBgm(
		bool pause,
		float speed,
		EaseFuncf ease)
	{
		ISoundBgmEntity* pBgm;

		if (m_BgmCount > 0)
		{
			pBgm = m_Bgms[m_BgmCount - 1];

			// If it is a duplicate of a previous stacked song, do nothing.
			if (m_BgmCount > 1 && m_Bgms[m_BgmCount - 2] == m_Bgms[m_BgmCount - 1])
			{
				// Do nothing but decreasing the prevBgm count.
				m_BgmCount--;
			}
			else
			{
				if (pause)
				{
					m_Bgms[--m_BgmCount]->Pause(speed, ease);
				}
				else
				{
					m_Bgms[--m_BgmCount]->Stop(speed, ease);
				}

				if (m_BgmCount > 0)
				{
					m_Bgms[m_BgmCount - 1]->Play(speed, ease);
					return m_Bgms[m_BgmCount - 1];
				}
			}
		}
		else
		{
			pBgm = nullptr;
			LOGE("Unable to pop, there are no BGM playing.");
		}

		return pBgm;
	}

	ISoundBgmEntity* CSoundManager::PlayBgm(
		u64 bgmId,
		Xe::Sound::IAudioSource& source,
		float speed = 1.0f,
		EaseFuncf ease = Xe::Game::Ease::Linear)
	{
		if (m_BgmCount == 0) return nullptr;

		// Check if the previous bgm is the same bgm that we want to play.
		if (m_Bgms[m_BgmCount - 1]->GetBgmId() == bgmId)
		{
			return m_Bgms[m_BgmCount - 1];
		}

		// Recycle the existing next BGM, if possible.
		if (m_BgmCount < m_MaxBgmCount)
		{
			// Check if a previous BGM was playing.
			if (m_Bgms[m_BgmCount - 1])
			{
				// If they has the same bgmId then recycle it.
				if (m_Bgms[m_BgmCount - 1]->GetBgmId() == bgmId)
				{
					auto prevBgm = m_Bgms[m_BgmCount - 1];
					prevBgm->Stop(speed, ease);

					m_Bgms[m_BgmCount]->Play(speed, ease);
					m_Bgms[m_BgmCount - 1] = m_Bgms[m_BgmCount];

					m_Bgms[m_BgmCount] = prevBgm;
					return m_Bgms[m_BgmCount - 1];
				}
				else
				{
					// If not, just stop it.
					auto prevBgm = m_Bgms[m_BgmCount - 1];
					if (!m_Bgms[m_BgmCount])
					{
						m_Bgms[m_BgmCount] = new CSoundBgmEntity(*this, bgmId);
					}

					// Use the above slot to stop the music.
					prevBgm->Stop(speed, ease);
					m_Bgms[m_BgmCount]->SetAudioSource(source);
					m_Bgms[m_BgmCount]->Play(speed, ease);
					m_Bgms[m_BgmCount - 1] = m_Bgms[m_BgmCount];
					m_Bgms[m_BgmCount] = prevBgm;
				}
			}
		}
		else
		{
			// It is not possible to use an above slot. Immediately stop the music.
			m_Bgms[m_BgmCount - 1]->Stop(0.0f, Ease::Linear);
		}

		if (!m_Bgms[m_BgmCount - 1])
		{
			m_Bgms[m_BgmCount - 1] = new CSoundBgmEntity(*this, bgmId);
		}

		auto bgm = m_Bgms[m_BgmCount - 1];
		if (bgm->SetAudioSource(source))
		{
			bgm->Play(speed, ease);
		}
		else
		{
			delete bgm;
			bgm = nullptr;
		}

		return bgm;
	}

	//! \brief Get the playing BGM.
	//! \return The current BGM above the stack. Can be nullptr.
	ISoundBgmEntity* CSoundManager::PeekBgm() const
	{
		return m_BgmCount > 0 ? m_Bgms[m_BgmCount - 1] : nullptr;
	}

	ISoundSfxEntity* CSoundManager::PlaySfx(Xe::Sound::IAudioSource& source, int priority)
	{
		// Search for a free slot
		int sfxSlot = -1;
		for (int i = 0; i < m_MaxSfxCount; i++)
		{
			if (IsSfxSlotGarbage(i) || !m_Sfxs[i]->IsPlaying())
			{
				sfxSlot = i;
				break;
			}
		}

		// If no slots are available, then replace the SFX with the lowest priority
		// or with the oldest sound effect.
		if (sfxSlot < 0)
		{
			int lowestPriority = 0x7FFFFFFF;
			u64 lowestTimestamp = 0xFFFFFFFFFFFFFFFFULL;
			for (int i = 0; i < m_MaxSfxCount; i++)
			{
				int sfxPriority = m_Sfxs[i]->GetPriority();
				u64 creationTime = m_Sfxs[i]->GetCreationTime();
				if (lowestPriority >= sfxPriority)
				{
					if (lowestPriority > sfxPriority)
					{
						lowestPriority = sfxPriority;
						lowestTimestamp = creationTime;
						sfxPriority = i;
					}
					else if (lowestTimestamp > creationTime)
					{
						lowestTimestamp = creationTime;
						sfxPriority = i;
					}
				}
			}
		}

		CSoundSfxEntity* sfx;

		// If an empty slot has been found.
		if (sfxSlot >= 0)
		{
			if (!m_Sfxs[sfxSlot])
			{
				m_Sfxs[sfxSlot] = new CSoundSfxEntity(*this, priority);
			}

			sfx = m_Sfxs[sfxSlot];
			if (sfx->SetAudioSource(source))
			{
				m_Sfxs[sfxSlot] = sfx;
				sfx->Play(0.0f, Xe::Game::Ease::One);
			}
			else
			{
				delete sfx;
				sfx = nullptr;
			}
		}
		else
		{
			sfx = nullptr;
		}

		return sfx;
	}
	
	void CSoundManager::StopAllSfxs(float speed, EaseFuncf ease)
	{
		for (int i = 0; i < m_BgmCount; i++)
		{
			m_Sfxs[i]->Stop(speed, ease);
		}
	}

	bool CSoundManager::IsSfxSlotGarbage(int sfxSlot) const
	{
		return !m_Sfxs[sfxSlot] || !m_Sfxs[sfxSlot]->IsPlaying();
	}

	void SoundManagerFactory(ISoundManager*& soundManager, Xe::Sound::IAudio& audio, int maxBgmCount, int maxSfxCount)
	{
		soundManager = new CSoundManager(audio, maxBgmCount, maxSfxCount);
	}
} }