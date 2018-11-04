#pragma once
#include <XeSDK/IDriversSoundDriver.h>

namespace Xe { namespace Drivers {
	struct XAudioDriver : public ISoundDriver
	{
		const char* GetDriverName() const;
		DriverType GetDriverType() const;
		bool Factory(Xe::Sound::IAudio** pContext, const Xe::Sound::AudioInitDesc& desc);
	};
} }
