#pragma once
#include <XeGame/IGameSoundBgmEntity.h>
#include "CGameSoundEntity.h"

namespace Xe { namespace Game {
	class CSoundBgmEntity : public virtual CSoundEntity, public virtual ISoundBgmEntity, public virtual IObject
	{
		u64 m_BgmId;

		float GetMasterVolume() const;
	public:
		CSoundBgmEntity(
			u64 bgmId,
			CSoundManager& soundManager,
			Xe::Sound::IAudioBuffer& buffer,
			Xe::Sound::IAudioSource& source);

		u64 GetBgmId() const;
	};
} }