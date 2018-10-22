#include "pch.h"
#include <XeSDK/XeSoundWavSoundFormatFactory.h>
#include "WavAudioSource.h"

namespace Xe { namespace Sound {
	ctstring WavSoundFormatFactory::GetName() const
	{
		return "WAVE";
	}

	ctstring WavSoundFormatFactory::GetExtension() const
	{
		return ".wav";
	}

	bool WavSoundFormatFactory::Open(IAudioSource*& pAudioSource, Xe::IO::IStream& pStream)
	{
		auto outAudioSource = new WavAudioSource;
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