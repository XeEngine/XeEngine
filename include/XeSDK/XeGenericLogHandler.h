#pragma once
#include <XeSDK/ILogEventHandler.h>

namespace Xe
{
	class GenericLogHandler : public Xe::IDelegate<const LogArgs&>
	{
	protected:
		int m_LogLevel;

		bool CanLog(int logLevel) { return logLevel <= m_LogLevel; }
	public:
#ifdef _DEBUG
		GenericLogHandler() : m_LogLevel(LogLevel_Debug) { }
#else
		GenericLogHandler() : m_LogLevel(LogLevel_Warning) { }
#endif

		//! \brief Set a minimum log level as filter.
		void SetLogLevel(int logLevel) { m_LogLevel = logLevel; }
	};
}