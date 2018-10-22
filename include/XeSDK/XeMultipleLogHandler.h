#pragma once
#include <XeSDK/XeGenericLogHandler.h>
#include <list>

namespace Xe
{
	class MultipleLogHandler : public GenericLogHandler
	{
		std::list<ILogHandler*> m_LogHandlers;

		void OnLog(int level, double timer, ctstring fileName, int line, ctstring function, ctstring str);

	public:
		~MultipleLogHandler();

		void AddLogHandler(ILogHandler& pLogHandler);
	};
}