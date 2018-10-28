#pragma once
#include <XeSDK/ICoreApplicationEventHandler.h>

namespace Xe { namespace Core {
	class DummyApplicationHandler : public IApplicationHandler
	{
		bool OnInitialize() { return true; }
		void OnDestroy() { }
		void OnSuspend() { }
		void OnResume() { }
		void OnRun() { }
		void OnDraw() { }
		void OnDevice(const DeviceArgs& args) { }
	};
} }