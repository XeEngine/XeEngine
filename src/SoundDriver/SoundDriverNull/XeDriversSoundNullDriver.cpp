#include "pch.h"
#include "XeDriversSoundNullDriver.h"
#include "XeSoundNull.h"

namespace Xe { namespace Drivers { namespace Sound {
	const char* NullDriver::GetDriverName() const
	{
		return "SoundNull";
	}

	DriverType NullDriver::GetDriverType() const
	{
		return DriverType_Sound;
	}

	bool NullDriver::Factory(Xe::Sound::IAudio** pContext)
	{
		ASSERT(pContext != nullptr);
		*pContext = new Xe::Sound::AudioNull;

		ASSERT(pContext != nullptr);
		return true;
	}
} } }
