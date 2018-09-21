#include "pch.h"
#include <XeGame/XeGameSoundManager.h>
#include "CGameSoundManager.h"
#include "CGameSoundBgmEntity.h"
#include "CGameSoundSfxEntity.h"
#include "CGameSoundCallback.h"

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
		ISoundBgmEntity* pBgm;

		if (m_BgmCount < m_MaxBgmCount)
		{
			// check if a music with the same id is already playing
			if (m_BgmCount > 0)
			{
				if (m_Bgms[m_BgmCount - 1]->GetBgmId() == bgmId)
				{
					pBgm = m_Bgms[m_BgmCount] = m_Bgms[m_BgmCount - 1];
					m_Bgms[m_BgmCount]->AddRef();

					m_BgmCount++;
					return pBgm; // Return instead of executing the giant IF-ELSE logic.
				}
				else
				{
					// If not, pause the current playing music.
					m_Bgms[m_BgmCount - 1]->Pause(speed, ease);
				}
			}

			// Check if the same Bgm ID was previously pushed, so we can recycle it
			if (m_Bgms[m_BgmCount] && m_Bgms[m_BgmCount]->GetBgmId() == bgmId)
			{
				pBgm = m_Bgms[m_BgmCount];
				m_Bgms[m_BgmCount++]->Play(speed, ease);
			}
			else
			{
				Xe::Sound::IAudioBuffer* pBuffer;
				auto callback = new CSoundCallback(source);
				if (m_Audio.CreateBuffer(&pBuffer, source.GetDesc()))
				{
					pBuffer->SetCallback(*callback);

					auto bgm = new CSoundBgmEntity(bgmId, *this, *pBuffer, source);
					if (bgm)
					{
						bgm->Play(speed, ease);

						// Kill the music above.
						if (m_Bgms[m_BgmCount])
						{
							m_Bgms[m_BgmCount]->Pause(0.0f, Xe::Game::Ease::Linear);
							m_Bgms[m_BgmCount]->Release();
						}

						pBgm = m_Bgms[m_BgmCount++] = bgm;
					}
					else
					{
						pBgm = nullptr;
					}

					pBuffer->Release();
				}
				else
				{
					pBgm = nullptr;
				}

				callback->Release();
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
			if (m_Bgms[m_BgmCount])
			{
				// If they has the same bgmId, try to recycle it.
				if (m_Bgms[m_BgmCount]->GetBgmId() == bgmId)
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
					// If not, just delete it.
					m_Bgms[m_BgmCount]->Stop(0.0f, Ease::Linear);
					m_Bgms[m_BgmCount]->Release();
					m_Bgms[m_BgmCount] = nullptr;
				}
			}

			// Use the above slot to stop the music.
			m_Bgms[m_BgmCount] = m_Bgms[m_BgmCount - 1];
			m_Bgms[m_BgmCount - 1]->Stop(speed, ease);
		}
		else
		{
			// It is not possible to use an above slot. Immediately stop the music.
			m_Bgms[m_BgmCount - 1]->Stop(0.0f, Ease::Linear);
			m_Bgms[m_BgmCount - 1]->Release();
			m_Bgms[m_BgmCount - 1] = nullptr;
		}

		CSoundBgmEntity* pBgm;

		Xe::Sound::IAudioBuffer* pBuffer;
		auto callback = new CSoundCallback(source);
		if (m_Audio.CreateBuffer(&pBuffer, source.GetDesc()))
		{
			pBuffer->SetCallback(*callback);

			auto bgm = new CSoundBgmEntity(bgmId, *this, *pBuffer, source);
			if (bgm)
			{
				bgm->Play(speed, ease);
				pBgm = m_Bgms[m_BgmCount - 1] = bgm;
			}

			pBuffer->Release();
			pBgm = nullptr;
		}
		else
		{
			pBgm = nullptr;
		}

		callback->Release();

		return pBgm;
	}

	//! \brief Get the playing BGM.
	//! \return The current BGM above the stack. Can be nullptr.
	ISoundBgmEntity* CSoundManager::PeekBgm() const
	{
		return m_BgmCount > 0 ? m_Bgms[m_BgmCount - 1] : nullptr;
	}

	ISoundSfxEntity* CSoundManager::PlaySfx(Xe::Sound::IAudioSource& source, int priority)
	{
		ISoundSfxEntity* pSfx;

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

		// If an empty slot has been found.
		if (sfxSlot >= 0)
		{
			Xe::Sound::IAudioBuffer* pBuffer;
			auto callback = new CSoundCallback(source);
			if (m_Audio.CreateBuffer(&pBuffer, source.GetDesc()))
			{
				pBuffer->SetCallback(*callback);

				if (!!m_Sfxs[sfxSlot])
				{
					delete m_Sfxs[sfxSlot];
				}

				pSfx = m_Sfxs[sfxSlot] = new CSoundSfxEntity(*this, *pBuffer, source, priority);
				pSfx->Play();
				pBuffer->Release();
			}
			else
			{
				pSfx = nullptr;
			}

			callback->Release();
		}
		else
		{
			pSfx = nullptr;
		}

		return pSfx;
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