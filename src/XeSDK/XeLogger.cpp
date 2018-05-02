#include "pch.h"
#include <XeSDK/XeLogger.h>
#include <XeSDK/XeMemory.h>
#include <cstdarg>

namespace Xe {
    static const ctstring STRLOGLEVEL[] = {
            "CRIT", "ERRR", "WARN", "INFO", "DEBG"
    };
    static const ctstring STRLOGLEVEL_SHORT[] = {
            "CRT", "ERR", "WRN", "INF", "DBG"
    };
    Logger Logger::Instance;

    int Xe_vsnprintf(tchar* str, int maxlen, const tchar *fmt, va_list args) {
#ifdef PLATFORM_UNICODE
        return _vsnwprintf(str, maxlen, fmt, args);
#else
        return vsnprintf(str, maxlen, fmt, args);
#endif
    }

    Logger::Logger()
        : m_LogLevel(LogLevel_Debug),
          m_HeaderInfo(HeaderInfo_Level | HeaderInfo_Timealive)
    { }
    Logger::~Logger()
    { }

    Logger::LogLevel Logger::GetLogLevel() const {
        return m_LogLevel;
    }
    void Logger::SetLogLevel(LogLevel logLevel) {
        m_LogLevel = logLevel;
    }
    bool Logger::IsHeaderInfoEnabled(HeaderInfo headerInfo) const {
        return !!(m_HeaderInfo & (1 << headerInfo));
    }
    void Logger::SetHeaderInfoEnabled(HeaderInfo headerInfo, bool isEnabled) {
        if (isEnabled)
            m_HeaderInfo |= (1 << headerInfo);
        else
            m_HeaderInfo &= ~(1 << headerInfo);
    }

    void Logger::Log(int level, ctstring str, ...) {
        if (level <= m_LogLevel) {
            va_list args;
            va_start(args, str);
            LogArgs(level, str, args);
            va_end(args);
        }
    }

    void Logger::LogArgs(int level, ctstring str, va_list args) {
        va_list argsTest;
        va_copy(argsTest, args);

        tchar buf[0x100];
        int len = Xe_vsnprintf(nullptr, 0, str, argsTest);
        va_end(argsTest);
        if (len >= lengthof(buf)) {
            tchar *heapbuf = (tchar*)Memory::Alloc((len + 1) * sizeof(tchar));
            Xe_vsnprintf(heapbuf, len + 1, str, args);
            LowLevelLog((LogLevel)level, heapbuf);
            Memory::Free(heapbuf);
        }
        else {
            Xe_vsnprintf(buf, lengthof(buf), str, args);
            LowLevelLog((LogLevel)level, buf);
        }
    }

    void Logger::LowLevelLog(LogLevel level, ctstring str) {
        bool isError = level <= LogLevel_Warning;
#ifdef PLATFORM_WINDOWS
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        OutputDebugString(str);

		WORD attributes;
		switch (level) {
		case LogLevel_Critical:
			attributes = FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_INTENSITY;
			break;
		case LogLevel_Error:
			attributes = FOREGROUND_RED | BACKGROUND_INTENSITY;
			break;
		case LogLevel_Warning:
			attributes = FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_INTENSITY;
			break;
		case LogLevel_Info:
			attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			break;
		case LogLevel_Debug:
			attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			break;
		case LogLevel_Trace:
			attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			break;
		default:
			attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			break;
		}
		SetConsoleTextAttribute(hConsole, attributes);
#endif

        fputs(str, isError ? stderr : stdout);
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
}