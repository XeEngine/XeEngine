#include "pch.h"
#include "XeDriversSoundNullDriver.h"
#include "XeSoundNull.h"

namespace Xe { namespace Drivers {
	const char* NullSoundDriver::GetDriverName() const
	{
		return "SoundNull";
	}

	DriverType NullSoundDriver::GetDriverType() const
	{
		return DriverType_Sound;
	}

	bool NullSoundDriver::Factory(Xe::Sound::IAudio** pContext, const Xe::Sound::AudioInitDesc& desc)
	{
		ASSERT(pContext != nullptr);
		*pContext = new Xe::Sound::AudioNull;

		ASSERT(pContext != nullptr);
		return true;
	}
} }
