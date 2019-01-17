//
// Created by xeeynamo on 15/11/17.
//

#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/ILogEventHandler.h>
#include <XeSDK/XeEvent.h>
#include <stdarg.h>

// Configuration:
//
// XE_LOG_ACTIVE
// Define globally to use the logging system outside Debug and Dev configurations.
// It is possible to exclude some logging information redefining LOG* macros.
//
// XE_LOG_PATH
// By default the full path is not specifed on logs, only the source file name.
// Define globally to override this behaviour and use the file path instead.
//

#if defined(_DEBUG) || defined(DEVELOPMENT) || defined(XE_LOG_ACTIVE)
constexpr const char* _LogGetFilenameWithoutPath(const char* path, int cursor) {
	return cursor > 0 ? path[cursor] != '/' && path[cursor] != '\\' ?
		_LogGetFilenameWithoutPath(path, cursor - 1) : path + cursor + 1 : path;
}
template <int length>
constexpr const char* _LogGetFilenameWithoutPath(const char(&path)[length]) {
	return _LogGetFilenameWithoutPath(path, length - 1);
}

#ifdef XE_LOG_PATH
#define XE__FILE__ __FILE__
#else
#define XE__FILE__ _LogGetFilenameWithoutPath(__FILE__)
#endif

#define LOGFA(expression) (!!(expression) || (LOGF(#expression), 0))
#define LOGA(expression) (!!(expression) || (LOGE(#expression), 0))
#define LOGF(fmt, ...) Xe::Logger::Fatal(__COUNTER__, XE__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGE(fmt, ...) Xe::Logger::Error(__COUNTER__, XE__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGW(fmt, ...) Xe::Logger::Warning(__COUNTER__, XE__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGI(fmt, ...) Xe::Logger::Info(__COUNTER__, XE__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGD(fmt, ...) Xe::Logger::Debug(__COUNTER__, XE__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOGT(fmt, ...) Xe::Logger::Trace(__COUNTER__, XE__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#else
#define LOGFA(expression) (expression)
#define LOGA(expression) (expression)
#define LOGF(fmt, ...) (NOOP())
#define LOGE(fmt, ...) (NOOP())
#define LOGW(fmt, ...) (NOOP())
#define LOGI(fmt, ...) (NOOP())
#define LOGD(fmt, ...) (NOOP())
#define LOGT(fmt, ...) (NOOP())

#define XE__FILE__ ""
#endif

namespace Xe {
	interface ILogHandler;
    struct Logger
	{
        static LogLevel GetLogLevel();
        static void SetLogLevel(LogLevel logLevel);
		static Event<const LogArgs&>& GetLogEvent();

		static void Fatal(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void Error(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void Warning(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void Info(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void Debug(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...);
		static void Trace(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...);
    };
}