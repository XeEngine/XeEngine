#pragma once
#include <XeSDK/IIoStream.h>
#include <XeSDK/ISoundAudioSource.h>

namespace Xe {
	namespace Sound {
		bool OpenSourceStreamWav(IAudioSource **pAudioSource, IO::IStream *pStream);
	}
}