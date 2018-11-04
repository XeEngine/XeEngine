#include "pch.h"
#include "XeDriversSoundWasapiDriver.h"
#include "XAudioEngine.h"

namespace Xe { namespace Drivers {
	const char* WasapiDriver::GetDriverName() const
	{
		return "Wasapi";
	}

	DriverType WasapiDriver::GetDriverType() const
	{
		return DriverType_Sound;
	}

	bool WasapiDriver::Factory(Xe::Sound::IAudio** pContext, const Xe::Sound::AudioInitDesc& desc)
	{
		ASSERT(pContext != nullptr);
		//auto driver = nullptr;//new Xe::Sound::;

		//ASSERT(driver != nullptr);
		//*pContext = driver;

		//return true;

		return false;
	}
} }
