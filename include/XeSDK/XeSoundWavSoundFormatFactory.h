#pragma once
#include <XeSDK/ISoundSoundFormatFactory.h>

namespace Xe { namespace Sound {
	class WavSoundFormatFactory : public ISoundFormatFactory
	{
	public:
		ctstring GetName() const;
		ctstring GetExtension() const;
		bool Open(IAudioSource*& ppAudioSource, Xe::IO::IStream& pStream);
	};
} }