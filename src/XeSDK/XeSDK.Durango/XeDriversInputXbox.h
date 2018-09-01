#pragma once
#include <XeSDK/IDriversGamepadDriver.h>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Xbox::Input;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;

typedef IEventHandler<ABI::Windows::Xbox::Input::ControllerAddedEventArgs*> ControllerAddedEvent;
typedef IEventHandler<ABI::Windows::Xbox::Input::ControllerRemovedEventArgs*> ControllerRemovedEvent;

namespace Xe { namespace Drivers { namespace Input {
	class XboxInput :
		public IGamepadDriver
	{
		ComPtr<IGamepadStatics> m_GamepadStatics;
		ComPtr<IControllerStatics> m_ControllerStatics;
		IGamepadEventHandler* m_GamepadEventHandler;

		EventRegistrationToken tokenControllerAdded;
		EventRegistrationToken tokenControllerRemoved;

		// Inherited via IGamepadDriver
		const char * GetDriverName() const;
		DriverType GetDriverType() const;
		InputDeviceType GetInputDeviceType() const;
		bool OpenDevice(Xe::IO::Gamepad ** ppGamepad, const Xe::IO::GamepadEntry & entry);
		size_t EnumerateDevices(Xe::IO::GamepadEntry * pEntries, size_t index, size_t count);
		void SetGamepadEventHandler(IGamepadEventHandler* pGamepadEventHandler);

		HRESULT OnControllerAdded(IInspectable*, IControllerAddedEventArgs* args);
		HRESULT OnControllerRemoved(IInspectable*, IControllerRemovedEventArgs* args);

	public:
		XboxInput();
		~XboxInput();
	};
} } }