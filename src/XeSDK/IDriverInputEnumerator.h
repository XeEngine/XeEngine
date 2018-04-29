#pragma once
#include <list>

namespace Xe { namespace Driver { namespace Input {
	struct Enumerator;
} } }

namespace Xe { namespace IO {
	typedef unsigned long long GamepadEntry;
	struct Gamepad;

	static std::list<Xe::Driver::Input::Enumerator*> g_Enumerators;

	void SubscribeEnumerator(Xe::Driver::Input::Enumerator& enumerator);
} }

namespace Xe { namespace Driver { namespace Input {
	struct Enumerator {
		virtual bool OpenDevice(
			Xe::IO::Gamepad** ppGamepad,
			const Xe::IO::GamepadEntry& entry) = 0;

		virtual size_t EnumerateDevices(
			Xe::IO::GamepadEntry* pEntries,
			size_t index,
			size_t count) = 0;
	};
} } }