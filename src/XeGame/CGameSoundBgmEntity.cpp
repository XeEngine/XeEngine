#include "pch.h"
#include <XeGame/XeGameSoundManager.h>
#include "CGameSoundBgmEntity.h"
#include "CGameSoundManager.h"

namespace Xe { namespace Game {
	CSoundBgmEntity::CSoundBgmEntity(CSoundManager& soundManager, u64 bgmId) :
		CSoundEntity(soundManager),
		m_BgmId(bgmId)
	{

	}

	u64 CSoundBgmEntity::GetBgmId() const
	{
		return m_BgmId;
	}

	float CSoundBgmEntity::GetMasterVolume() const
	{
		return m_SoundManager.GetMasterVolume() * m_SoundManager.GetBgmVolume();
	}
} } 