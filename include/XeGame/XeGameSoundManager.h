#pragma once
#include <XeSDK/ISound.h>

namespace Xe { namespace Sound {
	interface IAudio;
} }
namespace Xe { namespace Game {
	interface ISoundManager;
	void SoundManagerFactory(ISoundManager*& soundManager, Xe::Sound::IAudio& audio, int maxBgmCount, int maxSfxCount);
} }