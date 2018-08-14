#pragma once
#include <XeSDK/IDriversDriver.h>

namespace Xe { namespace Core {
	class IView;
} }

namespace Xe { namespace Sound {
	class IAudio;
} }

namespace Xe { namespace Drivers { namespace Sound {
	//! \brief Contract for a Sound Driver
	struct ISoundDriver : public IDriver
	{
		virtual bool Factory(Xe::Sound::IAudio** pContext) = 0;
	};
} } }
