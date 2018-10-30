#include "pch.h"
#include <XeSDK/XeStreamLogHandler.h>
#include <XeSDK/IIoStream.h>
#include <string>

namespace Xe
{
	StreamLogHandler::StreamLogHandler(Xe::IO::IStream& stream, bool flushOnLog) :
		m_Stream(stream),
		m_FlushOnLog(flushOnLog),
		m_LogTimer(true),
		m_LogFilename(true),
		m_LogFunction(true)
	{
		m_Stream.AddRef();
	}
	StreamLogHandler::~StreamLogHandler()
	{
		m_Stream.Release();
	}

	void StreamLogHandler::LogTimer(bool enabled) { m_LogTimer = enabled; }
	void StreamLogHandler::LogFilename(bool enabled) { m_LogFilename = enabled; }
	void StreamLogHandler::LogFunction(bool enabled) { m_LogFunction = enabled; }

	void StreamLogHandler::OnLog(int level, double timer, ctstring fileName, int line, ctstring function, ctstring str)
	{
		ctstring Space = " ";
		ctstring NewLine = "\n";
		if (!CanLog(level)) return;

		ctstring strLevel = "[---]";
		if (level <= LogLevel_Critical) strLevel = "[FTL]";
		else if (level <= LogLevel_Error) strLevel = "[ERR]";
		else if (level <= LogLevel_Warning) strLevel = "[WRN]";
		else if (level <= LogLevel_Info) strLevel = "[INF]";
		else if (level <= LogLevel_Debug) strLevel = "[DBG]";
		else if (level <= LogLevel_Trace) strLevel = "[TRC]";
		m_Stream.Write(strLevel, 0, 5);
	
		char buf[0x100];
		if (m_LogTimer)
		{
			int len = snprintf(buf, lengthof(buf), "[%4.3fs]", timer);
			m_Stream.Write(buf, 0, len);
		}

		if (m_LogFilename && !!fileName)
		{
			int len = snprintf(buf, lengthof(buf), "[%s:%i]", fileName, line);
			m_Stream.Write(buf, 0, len);
		}

		if (m_LogFunction && !!function)
		{
			int len = snprintf(buf, lengthof(buf), "[f:%s]", function);
			m_Stream.Write(buf, 0, len);
		}

		int len = strlen(str);
		if (len > 0)
		{
			m_Stream.Write(Space, 0, 1);
			m_Stream.Write(str, 0, len);

			switch (str[len - 1])
			{
			case '\r':
			case '\n':
				break;
			default:
				m_Stream.Write(NewLine, 0, 1);
				break;
			}
		}

		if (m_FlushOnLog)
		{
			m_Stream.Flush();
		}
	}
}