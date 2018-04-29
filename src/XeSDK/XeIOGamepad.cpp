#include "pch.h"
#include <XeSDK/XeIOGamepad.h>
#include "IDriverInputEnumerator.h"

namespace Xe { namespace IO {

	void SubscribeEnumerator(Xe::Driver::Input::Enumerator& enumerator)
	{
		g_Enumerators.push_back(&enumerator);
	}

	bool OpenDevice(
		Xe::IO::Gamepad** ppGamepad,
		const Xe::IO::GamepadEntry& entry)
	{
		for (auto it = g_Enumerators.begin(); it != g_Enumerators.end(); ++it)
		{
			if ((*it)->OpenDevice(ppGamepad, entry))
			{
				return true;
			}
		}

		return false;
	}

	size_t EnumerateDevices(
		Xe::IO::GamepadEntry* pEntries,
		size_t index,
		size_t count)
	{
		size_t found = 0;

		for (auto it = g_Enumerators.begin();
			it != g_Enumerators.end() && index + found < count;
			++it)
		{
			found += (*it)->EnumerateDevices(pEntries, index + found, count);
		}

		return found;
	}

} }