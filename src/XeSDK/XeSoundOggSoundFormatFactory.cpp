#include "pch.h"
#include <XeSDK/XeSoundOggSoundFormatFactory.h>
#include "OggAudioSource.h"

namespace Xe { namespace Sound {
	ctstring OggSoundFormatFactory::GetName() const
	{
		return "Vorbis";
	}

	ctstring OggSoundFormatFactory::GetExtension() const
	{
		return ".ogg";
	}

	bool OggSoundFormatFactory::Open(IAudioSource*& pAudioSource, Xe::IO::IStream& pStream)
	{
		auto outAudioSource = new OggAudioSource;
		if (outAudioSource->Initialize(&pStream))
		{
			pAudioSource = outAudioSource;
			return true;
		}

		delete outAudioSource;
		pAudioSource = nullptr;
		return false;
	}
} }