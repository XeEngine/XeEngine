#include "pch.h"
#include <XeSDK/XeConsoleLogHandler.h>
#include <XeSDK/IIoStream.h>

namespace Xe
{
	ConsoleLogHandler::ConsoleLogHandler(bool flushOnLog) :
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		hOutConsole(nullptr),
#endif
		m_FlushOnLog(flushOnLog),
		m_LogTimer(true),
		m_LogFilename(true),
		m_LogFunction(true)
	{
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}
	ConsoleLogHandler::~ConsoleLogHandler()
	{
	}

	void ConsoleLogHandler::LogTimer(bool enabled) { m_LogTimer = enabled; }
	void ConsoleLogHandler::LogFilename(bool enabled) { m_LogFilename = enabled; }
	void ConsoleLogHandler::LogFunction(bool enabled) { m_LogFunction = enabled; }

	void ConsoleLogHandler::OnLog(int level, double timer, ctstring fileName, int line, ctstring function, ctstring str)
	{
		ctstring Space = " ";
		ctstring NewLine = "\n";
		if (!CanLog(level)) return;

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		WORD attribute;
		if (level <= LogLevel_Critical) attribute = FOREGROUND_BLUE | FOREGROUND_RED;
		else if (level <= LogLevel_Error) attribute = FOREGROUND_RED;
		else if (level <= LogLevel_Warning) attribute = FOREGROUND_GREEN | FOREGROUND_RED;
		else if (level <= LogLevel_Info) attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		else if (level <= LogLevel_Debug) attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		else if (level <= LogLevel_Trace) attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		else attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		SetConsoleTextAttribute(hOutConsole, attribute);
#endif

		ctstring strLevel = "[---]";
		if (level <= LogLevel_Critical) strLevel = "[FTL]";
		else if (level <= LogLevel_Error) strLevel = "[ERR]";
		else if (level <= LogLevel_Warning) strLevel = "[WRN]";
		else if (level <= LogLevel_Info) strLevel = "[INF]";
		else if (level <= LogLevel_Debug) strLevel = "[DBG]";
		else if (level <= LogLevel_Trace) strLevel = "[TRC]";
		Write(strLevel, 5);
	
		char buf[0x100];
		if (m_LogTimer)
		{
			int len = snprintf(buf, lengthof(buf), "[%4.3fs]", timer);
			Write(buf, len);
		}

		if (m_LogFilename && !!fileName)
		{
			int len = snprintf(buf, lengthof(buf), "[%s:%i]", fileName, line);
			Write(buf, len);
		}

		if (m_LogFunction && !!function)
		{
			int len = snprintf(buf, lengthof(buf), "[f:%s]", function);
			Write(buf, len);
		}

		int len = strlen(str);
		if (len > 0)
		{
			Write(Space, 1);
			Write(str, len);

			switch (str[len - 1])
			{
			case '\r':
			case '\n':
				break;
			default:
				Write(NewLine, 1);
				break;
			}
		}

		if (m_FlushOnLog)
		{
			fflush(stdout);
		}
	}

	void ConsoleLogHandler::Write(ctstring str, int len)
	{
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		OutputDebugStringA(str);
#endif
		fwrite(str, sizeof(tchar), len, stdout);
	}
}