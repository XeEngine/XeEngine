#include "pch.h"
#include "XeDriversSoundXAudioDriver.h"
#include "XAudioEngine.h"

namespace Xe { namespace Drivers {
	const char* XAudioDriver::GetDriverName() const
	{
		return "XAudio2";
	}

	DriverType XAudioDriver::GetDriverType() const
	{
		return DriverType_Sound;
	}

	bool XAudioDriver::Factory(Xe::Sound::IAudio** pContext, const Xe::Sound::AudioInitDesc& desc)
	{
		ASSERT(pContext != nullptr);
		auto driver = new Xe::Sound::XAudioEngine;

		ASSERT(driver != nullptr);
		bool result = driver->Initialize(desc);
		if (result)
		{
			*pContext = driver;
		}

		return result;
	}
} }
