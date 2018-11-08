#pragma once
#include <XeSDK/XeGenericLogHandler.h>

namespace Xe { namespace IO {
	interface IStream;
} }

namespace Xe
{
	class ConsoleLogHandler : public GenericLogHandler
	{
		bool m_FlushOnLog;
		bool m_LogTimer, m_LogFilename, m_LogFunction;

		void operator()(const LogArgs&);
		void Write(ctstring str, int len);
	
	public:
		ConsoleLogHandler(bool flushOnLog = false);
		~ConsoleLogHandler();

		void LogTimer(bool enabled);
		void LogFilename(bool enabled);
		void LogFunction(bool enabled);
	};
}