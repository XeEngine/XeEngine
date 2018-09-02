#pragma once
#include <XeSDK/IDriversSoundDriver.h>

namespace Xe { namespace Drivers { namespace Sound {
	struct NullDriver : public ISoundDriver
	{
		const char* GetDriverName() const;
		DriverType GetDriverType() const;
		bool Factory(Xe::Sound::IAudio** pContext, const Xe::Sound::AudioInitDesc& desc);
	};
} } }
