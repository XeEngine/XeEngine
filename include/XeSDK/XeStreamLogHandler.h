#pragma once
#include <XeSDK/XeGenericLogHandler.h>

namespace Xe { namespace IO {
	interface IStream;
} }

namespace Xe
{
	class StreamLogHandler : public GenericLogHandler
	{
		Xe::IO::IStream& m_Stream;
		bool m_FlushOnLog;
		bool m_LogTimer, m_LogFilename, m_LogFunction;

		void OnLog(int level, double timer, ctstring fileName, int line, ctstring function, ctstring str);
	
	public:
		StreamLogHandler(Xe::IO::IStream& stream, bool flushOnLog = false);
		~StreamLogHandler();

		void LogTimer(bool enabled);
		void LogFilename(bool enabled);
		void LogFunction(bool enabled);
	};
}