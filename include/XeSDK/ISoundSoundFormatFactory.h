#pragma once
#include <XeSDK/IObject.h>

namespace Xe { namespace IO {
	interface IStream;
} }

namespace Xe { namespace Sound {
	interface IAudioSource;

	interface ISoundFormatFactory
	{
		//! \brief Get a human readable name of the format.
		virtual ctstring GetName() const = 0;

		//! \brief Get the extension for the current file format.
		virtual ctstring GetExtension() const = 0;
		
		//! \brief Open and load the format from the specified stream.
		virtual bool Open(IAudioSource*& ppAudioSource, Xe::IO::IStream& pStream) = 0;
	};
} }