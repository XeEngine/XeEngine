#pragma once
#include <XeSDK/IDriversDriver.h>

namespace Xe { namespace Sound {
	interface IAudio;
} }

namespace Xe { namespace Drivers { namespace Sound {
	//! \brief Contract for a Sound Driver
	interface ISoundDriver : public IDriver
	{
		virtual bool Factory(Xe::Sound::IAudio** pContext) = 0;
	};
} } }
