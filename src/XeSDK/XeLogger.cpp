#include "pch.h"
#include <XeSDK/XeLogger.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeTimer.h>
#include <XeSDK/XeConsoleLogHandler.h>
#include <cstdarg>

namespace Xe {

	__forceinline int Xe_vsnprintf(tchar* str, int maxlen, const tchar *fmt, va_list args) {
#ifdef PLATFORM_UNICODE
		return _vsnwprintf(str, maxlen, fmt, args);
#else
		return vsnprintf(str, maxlen, fmt, args);
#endif
	}

	struct InternalLogger
	{
		LogLevel m_LogLevel;
		Event<const LogArgs&> m_LogEvent;

		static InternalLogger Instance;

		InternalLogger()
			: m_LogLevel(LogLevel_Debug)
		{ }

		~InternalLogger()
		{ }

		void LogArgs(int counter, int level, ctstring fileName, int line, ctstring function, ctstring str, va_list args)
		{
			if (level <= Instance.m_LogLevel)
			{
				double timer = Timer::Current().AsDouble();

				va_list argsTest;
				va_copy(argsTest, args);

				tchar buf[0x100];
				int len = Xe_vsnprintf(nullptr, 0, str, argsTest);
				va_end(argsTest);

				struct LogArgs logArgs;
				logArgs.Id = counter;
				logArgs.Level = level;
				logArgs.Timer = timer;
				logArgs.FileName = fileName;
				logArgs.line = line;
				logArgs.Function = function;
				logArgs.Text = str;

				if (len >= lengthof(buf))
				{
					tchar *heapbuf = (tchar*)Memory::Alloc((len + 1) * sizeof(tchar));
					Xe_vsnprintf(heapbuf, len + 1, str, args);
					logArgs.Text = heapbuf;
					m_LogEvent(logArgs);
					Memory::Free(heapbuf);
				}
				else
				{
					Xe_vsnprintf(buf, lengthof(buf), str, args);
					logArgs.Text = buf;
					m_LogEvent(logArgs);
				}
			}
		}
	};

	InternalLogger InternalLogger::Instance;

    LogLevel Logger::GetLogLevel()
	{
        return InternalLogger::Instance.m_LogLevel;
    }

    void Logger::SetLogLevel(LogLevel logLevel)
	{
		InternalLogger::Instance.m_LogLevel = logLevel;
    }

	Event<const LogArgs&>& Logger::GetLogEvent()
	{
		return InternalLogger::Instance.m_LogEvent;
	}

	void Logger::Fatal(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(counter, LogLevel_Critical, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Error(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(counter, LogLevel_Error, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Warning(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(counter, LogLevel_Warning, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Info(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(counter, LogLevel_Info, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Debug(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(counter, LogLevel_Debug, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Trace(int counter, ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(counter, LogLevel_Trace, fileName, line, function, str, args);
		va_end(args);
	}

}