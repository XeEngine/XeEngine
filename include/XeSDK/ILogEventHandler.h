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

	//! \brief Used when logging data is needed.
	struct LogArgs
	{
		//! \brief Unique ID where the log has been invoked. Can change on each compilation.
		int Id;
		
		//! \brief Level of logging, from 'critical' to 'trace'.
		int Level;

		//! \brief Number of seconds since the application has started.
		double Timer;

		//! \brief Optional. The name of the file which the log cames from
		ctstring FileName;

		//! \brief Optional. The line of code of the fileName.
		int line;

		//! \brief Optional. The name of the callee function.
		ctstring Function;

		//! \brief The acutal text to log.
		ctstring Text;
	};
}