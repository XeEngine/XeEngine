#pragma once
#include <XeSDK/XeDef.h>

namespace Xe {
    //! \brief Basic functionalities of the engine
    namespace Core {
        //! \brief Obtain basic information from XeEngine development kit.
        struct EngineInfo {
            ctstring Name;
            ctstring Developer;
            ctstring Version;
            ctstring BuildDate;
            uvar VersionMajor;
            uvar VersionMinor;
            uvar VersionBuild;
        };

		void Initialize();

        //! \brief Fill the EngineInfo structure.
        void GetEngineInfo(EngineInfo& properties);

        //! \brief Print specified string to debugger's console.
        void DebugPrint(ctstring str);

        //! \brief Print a formatted string to debugger's console.
        void DebugPrintFormat(ctstring fmt, ...);
    }
}