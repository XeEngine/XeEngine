#pragma once
#include <XeGame/IGameSoundEntity.h>

namespace Xe { namespace Game {
	interface ISoundSfxEntity : public virtual ISoundEntity
	{
		virtual int GetPriority() const = 0;
	};
} }
