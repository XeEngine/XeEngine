#include "pch.h"
#include <XeSDK/XeCore.h>
#include "XeCoreCommon.h"

namespace Xe { namespace Core {
    const EngineInfo& GetEngineInfo()
    {
        static const tchar ENGINE_DATETIME[] = {
                __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], '-',
                __DATE__[0], __DATE__[1], __DATE__[2], '-',
                __DATE__[4], __DATE__[5], ' ',
                __TIME__[0], __TIME__[1], ':',
                __TIME__[3], __TIME__[4], ':',
                __TIME__[6], __TIME__[7], '\0',
        };
        static const EngineInfo ENGINE_INFO =
		{
                ENGINE_NAME,
                ENGINE_DEVELOPER,
                ENGINE_VERSION,
                ENGINE_DATETIME,
				XESDK_VER,
				COMPILER,
				COMPILER_VERSION
        };

        return ENGINE_INFO;
    }
} }