#pragma once
#include <XeSDK/IObject.h>

namespace Xe
{
	enum LogLevel
	{
		LogLevel_Critical,
		LogLevel_Error,
		LogLevel_Warning,
		LogLevel_Info,
		LogLevel_Debug,
		LogLevel_Trace,
	};

	interface ILogHandler : public virtual IObject
	{
		//! \brief Called when logging data is needed.
		//! \param level Level of logging, from 'critical' to 'trace'.
		//! \param timer Number of seconds since the application has started.
		//! \param fileName Optional. The name of the file which the log cames from
		//! \param line Optional. The line of code of the fileName.
		//! \param function Optional. The name of the callee function.
		//! \param str The acutal text to log.
		virtual void OnLog(int level, double timer, ctstring fileName, int line, ctstring function, ctstring str) = 0;
	};
}