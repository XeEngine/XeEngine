#pragma once
#include <XeGame/IGameSoundSfxEntity.h>
#include "CGameSoundEntity.h"

namespace Xe { namespace Game {
	class CSoundSfxEntity :
		public virtual CSoundEntity,
		public virtual ISoundSfxEntity
	{
		int m_Priority;

		float GetMasterVolume() const;
	public:
		CSoundSfxEntity(
			CSoundManager& soundManager,
			int priority);

		int GetPriority() const;
	};
} }