#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/ISoundPlayable.h>
#include <XeSDK/ISoundAudioSource.h>
#include <XeSDK/ISound.h>

namespace Xe {
	namespace Sound {
		bool CreatePlayable(IPlayable **pPlayable, IAudio *audio, IAudioSource *src);
	}
}