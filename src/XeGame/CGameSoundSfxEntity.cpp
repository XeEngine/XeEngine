#include "pch.h"
#include <XeGame/XeGameSoundManager.h>
#include "CGameSoundSfxEntity.h"
#include "CGameSoundManager.h"

namespace Xe { namespace Game {
	CSoundSfxEntity::CSoundSfxEntity(
		CSoundManager& soundManager,
		int priority) :
		CSoundEntity(soundManager),
		m_Priority(priority)
	{

	}

	int CSoundSfxEntity::GetPriority() const
	{
		return m_Priority;
	}

	float CSoundSfxEntity::GetMasterVolume() const
	{
		return m_SoundManager.GetMasterVolume() * m_SoundManager.GetSfxVolume();
	}
} }