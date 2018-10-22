#include "pch.h"
#include <XeSDK/XeLogger.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeTimer.h>
#include <XeSDK/XeConsoleLogHandler.h>
#include <cstdarg>

namespace Xe {
    Logger Logger::Instance;

    int Xe_vsnprintf(tchar* str, int maxlen, const tchar *fmt, va_list args) {
#ifdef PLATFORM_UNICODE
        return _vsnwprintf(str, maxlen, fmt, args);
#else
        return vsnprintf(str, maxlen, fmt, args);
#endif
    }

    Logger::Logger()
        : m_LogLevel(LogLevel_Debug)
    {
		m_pLogHandler = new ConsoleLogHandler;
	}
    Logger::~Logger()
    {
		m_pLogHandler->Release();
	}

    LogLevel Logger::GetLogLevel() const
	{
        return m_LogLevel;
    }

    void Logger::SetLogLevel(LogLevel logLevel)
	{
        m_LogLevel = logLevel;
    }

	void Logger::SetLogHandler(ILogHandler& logHandler)
	{
		m_pLogHandler->Release();
		m_pLogHandler = &logHandler;
		m_pLogHandler->AddRef();
	}

    void Logger::Log(int level, ctstring str, ...)
	{
        if (level <= m_LogLevel)
		{
            va_list args;
            va_start(args, str);
            LogArgs(level, str, args);
            va_end(args);
        }
    }

	void Logger::LogArgs(int level, ctstring str, va_list args)
	{
		LogArgs(level, nullptr, 0, nullptr, str, args);
	}

	void Logger::LogArgs(int level, ctstring fileName, int line, ctstring function, ctstring str, va_list args)
	{
		if (level <= Instance.m_LogLevel)
		{
			double timer = Timer::Current().AsDouble();

			va_list argsTest;
			va_copy(argsTest, args);

			tchar buf[0x100];
			int len = Xe_vsnprintf(nullptr, 0, str, argsTest);
			va_end(argsTest);

			if (len >= lengthof(buf))
			{
				tchar *heapbuf = (tchar*)Memory::Alloc((len + 1) * sizeof(tchar));
				Xe_vsnprintf(heapbuf, len + 1, str, args);
				LowLevelLog((LogLevel)level, timer, fileName, line, function, heapbuf);
				Memory::Free(heapbuf);
			}
			else
			{
				Xe_vsnprintf(buf, lengthof(buf), str, args);
				LowLevelLog((LogLevel)level, timer, fileName, line, function, buf);
			}
		}
	}

    void Logger::LowLevelLog(LogLevel level, double timer, ctstring fileName, int line, ctstring function, ctstring str)
	{
		m_pLogHandler->OnLog(level, timer, fileName, line, function, str);
	}

    void Logger::Fatal(ctstring str, ...) {
        if (LogLevel_Critical <= Instance.m_LogLevel) {
            va_list args;
            va_start(args, str);
            Instance.LogArgs(LogLevel_Critical, str, args);
            va_end(args);
        }
    }
	void Logger::Error(ctstring str, ...) {
		if (LogLevel_Error <= Instance.m_LogLevel) {
			va_list args;
			va_start(args, str);
			Instance.LogArgs(LogLevel_Error, str, args);
			va_end(args);
		}
	}
	void Logger::DebugError(ctstring str, ...) {
#if defined(_DEBUG) || defined(DEVELOPMENT)
		if (LogLevel_Error <= Instance.m_LogLevel) {
			va_list args;
			va_start(args, str);
			Instance.LogArgs(LogLevel_Error, str, args);
			va_end(args);
		}
#endif
	}
	void Logger::Warning(ctstring str, ...) {
		if (LogLevel_Warning <= Instance.m_LogLevel) {
			va_list args;
			va_start(args, str);
			Instance.LogArgs(LogLevel_Warning, str, args);
			va_end(args);
		}
	}
	void Logger::DebugWarning(ctstring str, ...) {
#if defined(_DEBUG) || defined(DEVELOPMENT)
		if (LogLevel_Warning <= Instance.m_LogLevel) {
			va_list args;
			va_start(args, str);
			Instance.LogArgs(LogLevel_Warning, str, args);
			va_end(args);
		}
#endif
	}
    void Logger::Info(ctstring str, ...) {
        if (LogLevel_Info <= Instance.m_LogLevel) {
            va_list args;
            va_start(args, str);
            Instance.LogArgs(LogLevel_Info, str, args);
            va_end(args);
        }
    }
	void Logger::Debug(ctstring str, ...) {
		if (LogLevel_Debug <= Instance.m_LogLevel) {
			va_list args;
			va_start(args, str);
			Instance.LogArgs(LogLevel_Debug, str, args);
			va_end(args);
		}
	}
	void Logger::Trace(ctstring str, ...) {
		if (LogLevel_Trace <= Instance.m_LogLevel) {
			va_list args;
			va_start(args, str);
			Instance.LogArgs(LogLevel_Debug, str, args);
			va_end(args);
		}
	}

	void Logger::FatalEx(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		Instance.LogArgs(LogLevel_Critical, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::ErrorEx(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		Instance.LogArgs(LogLevel_Error, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::WarningEx(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		Instance.LogArgs(LogLevel_Warning, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::InfoEx(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		Instance.LogArgs(LogLevel_Info, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::DebugEx(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		Instance.LogArgs(LogLevel_Debug, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::TraceEx(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		Instance.LogArgs(LogLevel_Trace, fileName, line, function, str, args);
		va_end(args);
	}

}