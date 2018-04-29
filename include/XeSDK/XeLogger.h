//
// Created by xeeynamo on 15/11/17.
//

#pragma once
#include <XeSDK/XeDef.h>
#include <stdarg.h>

namespace Xe {
    class Logger {
    public:
        static Logger Instance;

        enum LogLevel {
            LogLevel_Critical,
            LogLevel_Error,
            LogLevel_Warning,
            LogLevel_Info,
			LogLevel_Debug,
			LogLevel_Trace,
        };
        enum HeaderInfo {
            //! Print the log level
            HeaderInfo_Level,
            //! Print the timestamp since the application is running in hh:mm:ss.zzz
            HeaderInfo_Timealive
        };

        Logger();
        virtual ~Logger();

        LogLevel GetLogLevel() const;
        void SetLogLevel(LogLevel logLevel);
        bool IsHeaderInfoEnabled(HeaderInfo headerInfo) const;
        void SetHeaderInfoEnabled(HeaderInfo headerInfo, bool isEnabled);

        void Log(int level, ctstring str, ...);

        static void Fatal(ctstring str, ...);
        static void Error(ctstring str, ...);
		static void DebugError(ctstring str, ...);
        static void Warning(ctstring str, ...);
		static void DebugWarning(ctstring str, ...);
        static void Info(ctstring str, ...);
        static void Debug(ctstring str, ...);
		static void Trace(ctstring str, ...);

    private:
        static const ctstring STRLOGLEVEL[];
        static const ctstring STRLOGLEVEL_SHORT[];

        LogLevel m_LogLevel;
        int m_HeaderInfo;

    protected:
        void LogArgs(int level, ctstring str, va_list args);
        virtual void LowLevelLog(LogLevel level, ctstring str);
    };
}