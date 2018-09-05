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
		ILogHandler* m_pLogHandler;

		static InternalLogger Instance;

		InternalLogger()
			: m_LogLevel(LogLevel_Debug)
		{
			m_pLogHandler = new ConsoleLogHandler;
		}
		~InternalLogger()
		{
			m_pLogHandler->Release();
		}

		void LogArgs(int level, ctstring fileName, int line, ctstring function, ctstring str, va_list args)
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
					m_pLogHandler->OnLog((LogLevel)level, timer, fileName, line, function, heapbuf);
					Memory::Free(heapbuf);
				}
				else
				{
					Xe_vsnprintf(buf, lengthof(buf), str, args);
					m_pLogHandler->OnLog((LogLevel)level, timer, fileName, line, function, buf);
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

	void Logger::SetLogHandler(ILogHandler& logHandler)
	{
		InternalLogger::Instance.m_pLogHandler->Release();
		InternalLogger::Instance.m_pLogHandler = &logHandler;
		InternalLogger::Instance.m_pLogHandler->AddRef();
	}

	void Logger::Fatal(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(LogLevel_Critical, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Error(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(LogLevel_Error, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Warning(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(LogLevel_Warning, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Info(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(LogLevel_Info, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Debug(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(LogLevel_Debug, fileName, line, function, str, args);
		va_end(args);
	}

	void Logger::Trace(ctstring fileName, int line, ctstring function, ctstring str, ...)
	{
		va_list args;
		va_start(args, str);
		InternalLogger::Instance.LogArgs(LogLevel_Trace, fileName, line, function, str, args);
		va_end(args);
	}

}