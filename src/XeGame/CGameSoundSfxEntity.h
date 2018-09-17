#pragma once
#include <XeGame/IGameSoundSfxEntity.h>
#include "CGameSoundEntity.h"

namespace Xe { namespace Game {
	class CSoundSfxEntity : public CSoundEntity, public ISoundSfxEntity
	{
		int m_Priority;

		float GetMasterVolume() const;
	public:
		CSoundSfxEntity(
			CSoundManager& soundManager,
			Xe::Sound::IAudioBuffer& buffer,
			Xe::Sound::IAudioSource& source,
			int priority);

		int GetPriority() const;
	};
} }