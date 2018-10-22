#include "pch.h"
#include <XeSDK/XeMultipleLogHandler.h>

namespace Xe
{
	MultipleLogHandler::~MultipleLogHandler()
	{
		for (auto it = m_LogHandlers.begin(); it != m_LogHandlers.end(); ++it)
		{
			(*it)->Release();
		}
	}

	void MultipleLogHandler::AddLogHandler(ILogHandler& pLogHandler)
	{
		pLogHandler.AddRef();
		m_LogHandlers.push_back(&pLogHandler);
	}

	void MultipleLogHandler::OnLog(int level, double timer, ctstring fileName, int line, ctstring function, ctstring str)
	{
		if (!CanLog(level)) return;

		for (auto it = m_LogHandlers.begin(); it != m_LogHandlers.end(); ++it)
		{
			(*it)->OnLog(level, timer, fileName, line, function, str);
		}
	}
}