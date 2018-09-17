#pragma once
#include <XeGame/IGameSoundEntity.h>

namespace Xe { namespace Game {
	interface ISoundBgmEntity : public virtual ISoundEntity
	{
		virtual u64 GetBgmId() const = 0;
	};
} }
