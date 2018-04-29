#pragma once
#include <XeSDK/IIOStream.h>
#include <XeSDK/ISoundAudioSource.h>

namespace Xe {
	namespace Sound {
		bool OpenSourceStreamWav(IAudioSource **pAudioSource, IO::IStream *pStream);
	}
}