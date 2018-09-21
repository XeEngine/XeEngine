#pragma once
#include <XeGame/IGameSoundBgmEntity.h>
#include "CGameSoundEntity.h"

namespace Xe { namespace Game {
	class CSoundBgmEntity :
		public virtual CSoundEntity,
		public virtual ISoundBgmEntity,
		public virtual IObject
	{
		u64 m_BgmId;

		float GetMasterVolume() const;
	public:
		CSoundBgmEntity(CSoundManager& soundManager, u64 bgmId);

		u64 GetBgmId() const;
	};
} }