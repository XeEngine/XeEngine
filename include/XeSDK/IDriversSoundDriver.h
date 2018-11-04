#pragma once
#include <XeSDK/IDriversDriver.h>

namespace Xe { namespace Sound {
	interface IAudio;

	struct AudioInitDesc
	{
		//! \brief Define the master sample rate; if 0, the default driver sample rate will be used.
		u32 SampleRate;
	};
} }

namespace Xe { namespace Drivers {
	//! \brief Contract for a Sound Driver
	interface ISoundDriver : public IDriver
	{
		virtual bool Factory(Xe::Sound::IAudio** pContext, const Xe::Sound::AudioInitDesc& desc) = 0;
	};
} }
