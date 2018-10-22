//
// Created by xeeynamo on 15/11/17.
//

#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/ILogHandler.h>
#include <stdarg.h>

#if defined(_DEBUG) || defined(DEVELOPMENT)
#define LOGF(fmt, ...) Xe::Logger::FatalEx(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGE(fmt, ...) Xe::Logger::ErrorEx(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGW(fmt, ...) Xe::Logger::WarningEx(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGI(fmt, ...) Xe::Logger::InfoEx(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGD(fmt, ...) Xe::Logger::DebugEx(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGT(fmt, ...) Xe::Logger::TraceEx(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#else
#define LOGF(fmt, ...) (((void)0)())
#define LOGE(fmt, ...) (((void)0)())
#define LOGW(fmt, ...) (((void)0)())
#define LOGI(fmt, ...) (((void)0)())
#define LOGD(fmt, ...) (((void)0)())
#define LOGT(fmt, ...) (((void)0)())
#endif

namespace Xe {
	interface ILogHandler;
    class Logger {
    public:
        static Logger Instance;

        Logger();
        virtual ~Logger();

        LogLevel GetLogLevel() const;
        void SetLogLevel(LogLevel logLevel);
		void SetLogHandler(ILogHandler& logHandler);

        void Log(int level, ctstring str, ...);

        static void Fatal(ctstring str, ...);
        static void Error(ctstring str, ...);
		static void DebugError(ctstring str, ...);
        static void Warning(ctstring str, ...);
		static void DebugWarning(ctstring str, ...);
        static void Info(ctstring str, ...);
        static void Debug(ctstring str, ...);
		static void Trace(ctstring str, ...);

		static void FatalEx(ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void ErrorEx(ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void WarningEx(ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void InfoEx(ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void DebugEx(ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void TraceEx(ctstring fileName, int line, ctstring function, ctstring str, ...);

    private:
        LogLevel m_LogLevel;
		ILogHandler* m_pLogHandler;

    protected:
		void LogArgs(int level, ctstring str, va_list args);
		void LogArgs(int level, ctstring fileName, int line, ctstring function, ctstring str, va_list args);
        virtual void LowLevelLog(LogLevel level, double timer, ctstring fileName, int line, ctstring function, ctstring str);
    };
}