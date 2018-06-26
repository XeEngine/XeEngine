#pragma once
#include <XeSDK/XeDef.h>

namespace Xe { namespace Game {
	struct Achievement
	{
		const char* Id;
		u32 Timestamp;
		float Progress;
	};
} }