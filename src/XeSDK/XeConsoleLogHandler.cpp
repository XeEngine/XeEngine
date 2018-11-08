#include "pch.h"
#include <XeSDK/XeConsoleLogHandler.h>
#include <XeSDK/IIoStream.h>

namespace Xe
{
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
	HANDLE hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

	ConsoleLogHandler::ConsoleLogHandler(bool flushOnLog) :
		m_FlushOnLog(flushOnLog),
		m_LogTimer(true),
		m_LogFilename(true),
		m_LogFunction(true)
	{ }

	ConsoleLogHandler::~ConsoleLogHandler()
	{ }

	void ConsoleLogHandler::LogTimer(bool enabled) { m_LogTimer = enabled; }
	void ConsoleLogHandler::LogFilename(bool enabled) { m_LogFilename = enabled; }
	void ConsoleLogHandler::LogFunction(bool enabled) { m_LogFunction = enabled; }

	void ConsoleLogHandler::operator()(const LogArgs& args)
	{
		ctstring Space = " ";
		ctstring NewLine = "\n";
		if (!CanLog(args.Level)) return;

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
		WORD attribute;
		if (args.Level <= LogLevel_Critical) attribute = FOREGROUND_BLUE | FOREGROUND_RED;
		else if (args.Level <= LogLevel_Error) attribute = FOREGROUND_RED;
		else if (args.Level <= LogLevel_Warning) attribute = FOREGROUND_GREEN | FOREGROUND_RED;
		else if (args.Level <= LogLevel_Info) attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		else if (args.Level <= LogLevel_Debug) attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		else if (args.Level <= LogLevel_Trace) attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		else attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		SetConsoleTextAttribute(hOutConsole, attribute);
#endif

		ctstring strLevel = "[---]";
		if (args.Level <= LogLevel_Critical) strLevel = "[FTL]";
		else if (args.Level <= LogLevel_Error) strLevel = "[ERR]";
		else if (args.Level <= LogLevel_Warning) strLevel = "[WRN]";
		else if (args.Level <= LogLevel_Info) strLevel = "[INF]";
		else if (args.Level <= LogLevel_Debug) strLevel = "[DBG]";
		else if (args.Level <= LogLevel_Trace) strLevel = "[TRC]";
		Write(strLevel, 5);
	
		char buf[0x100];
		if (m_LogTimer)
		{
			int len = snprintf(buf, lengthof(buf), "[%4.3fs]", args.Timer);
			Write(buf, len);
		}

		if (m_LogFilename && !!args.FileName)
		{
			int len = snprintf(buf, lengthof(buf), "[%s:%i]", args.FileName, args.line);
			Write(buf, len);
		}

		if (m_LogFunction && !!args.Function)
		{
			int len = snprintf(buf, lengthof(buf), "[f:%s]", args.Function);
			Write(buf, len);
		}

		int len = strlen(args.Text);
		if (len > 0)
		{
			Write(Space, 1);
			Write(args.Text, len);

			switch (args.Text[len - 1])
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