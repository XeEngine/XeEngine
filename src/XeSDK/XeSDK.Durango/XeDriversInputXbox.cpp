#include "pch.h"
#include "XeDriversInputXbox.h"
#include "WinRtUtilities.h"
#include "DefaultGamepadEventHandler.h"
#include "XboxGamepad.h"
#include "UwpFrameView.h"

#include <wrl/event.h>
#include <Windows.Foundation.h>
#include <Windows.Xbox.Input.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::ApplicationModel::Core;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Windows::UI::ViewManagement;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel;
using namespace ABI::Windows::ApplicationModel::Activation;

using namespace ABI::Windows::Xbox::Input;

namespace Xe { namespace Drivers { namespace Input {

	XboxInput::XboxInput(UwpFrameView& uwpFrameView) :
		m_UwpFrameView(uwpFrameView)
	{
		HR(CoInitializeEx(NULL, COINIT_MULTITHREADED));

		m_GamepadStatics = GetActivationFactory<IGamepadStatics>(RuntimeClass_Windows_Xbox_Input_Gamepad);
		m_ControllerStatics = GetActivationFactory<IControllerStatics>(RuntimeClass_Windows_Xbox_Input_Controller);
		m_GamepadEventHandler = new DefaultGamepadEventHandler;

		m_ControllerStatics->add_ControllerAdded(Callback<ControllerAddedEvent>(this, &XboxInput::OnControllerAdded).Get(), &tokenControllerAdded);
		m_ControllerStatics->add_ControllerRemoved(Callback<ControllerRemovedEvent>(this, &XboxInput::OnControllerRemoved).Get(), &tokenControllerRemoved);
	}

	XboxInput::~XboxInput()
	{
		m_ControllerStatics->remove_ControllerAdded(tokenControllerAdded);
		m_ControllerStatics->remove_ControllerRemoved(tokenControllerRemoved);
		m_GamepadEventHandler->Release();
		CoUninitialize();
	}

	int XboxInput::GetPadsCount() const
	{
		ComPtr<IVectorView<ABI::Windows::Xbox::Input::IGamepad*>> pads;
		HR(m_GamepadStatics->get_Gamepads(pads.GetAddressOf()));

		unsigned int size;
		pads->get_Size(&size);
		return size;
	}

	const char * XboxInput::GetDriverName() const
	{
		return "Xbox Input";
	}

	DriverType XboxInput::GetDriverType() const
	{
		return DriverType_Input;
	}

	InputDeviceType XboxInput::GetInputDeviceType() const
	{
		return InputDeviceType_Gamepad;
	}

	bool XboxInput::OpenDevice(Xe::IO::Gamepad** ppGamepad, const Xe::IO::GamepadEntry & entry)
	{
		ASSERT(ppGamepad != nullptr);

		ComPtr<ABI::Windows::Xbox::Input::IController> controller;
		HRESULT hr = m_ControllerStatics->GetControllerById((UINT64)entry, controller.GetAddressOf());
		if (hr == S_OK)
		{
			*ppGamepad = new Xe::IO::XboxGamepad(entry, controller);
			return true;
		}

		*ppGamepad = nullptr;
		return false;
	}

	size_t XboxInput::EnumerateDevices(Xe::IO::GamepadEntry *pEntries, size_t index, size_t count)
	{
		ComPtr<IVectorView<ABI::Windows::Xbox::Input::IGamepad*>> pads;
		HR(m_GamepadStatics->get_Gamepads(pads.GetAddressOf()));

		unsigned int size;
		pads->get_Size(&size);

		if (pEntries != nullptr)
		{
			for (unsigned int i = 0; i < size && i < count; ++i)
			{
				pEntries[index + i] = (Xe::IO::GamepadEntry)i;

				ComPtr<ABI::Windows::Xbox::Input::IGamepad> gamepad;
				HR(pads->GetAt(i, gamepad.GetAddressOf()));

				ComPtr<ABI::Windows::Xbox::Input::IController> controller;
				HR(gamepad.As(&controller));

				UINT64 id;
				HR(controller->get_Id(&id));

				pEntries[index + i] = id;
			}
		}

		return size;
	}

	void XboxInput::SetGamepadEventHandler(IGamepadEventHandler* pGamepadEventHandler)
	{
		ASSERT(pGamepadEventHandler != nullptr);

		m_GamepadEventHandler->Release();
		m_GamepadEventHandler = pGamepadEventHandler;
		m_GamepadEventHandler->AddRef();
	}

	HRESULT XboxInput::OnControllerAdded(IInspectable*, IControllerAddedEventArgs* args)
	{
		ComPtr<ABI::Windows::Xbox::Input::IController> controller;
		HR(args->get_Controller(controller.GetAddressOf()));

		UINT64 id;
		HR(controller->get_Id(&id));

		// Tremendous hacky way to wait until the controller is connected.
		int prevPadsCount = GetPadsCount();
		int triesCount = 0;
		do
		{
			SleepEx(10, true);
			triesCount++;
		} while (GetPadsCount() == prevPadsCount);
		LOGI("GetPadsCount called %i times.", triesCount);

		m_UwpFrameView.NotifyDevice();
		m_GamepadEventHandler->OnGamepadAttached((Xe::IO::GamepadEntry)id);

		return S_OK;
	}

	HRESULT XboxInput::OnControllerRemoved(IInspectable*, IControllerRemovedEventArgs* args)
	{
		ComPtr<ABI::Windows::Xbox::Input::IController> controller;
		HR(args->get_Controller(controller.GetAddressOf()));

		UINT64 id;
		HR(controller->get_Id(&id));

		auto entry = (Xe::IO::GamepadEntry)id;
		m_UwpFrameView.NotifyDevice();
		m_GamepadEventHandler->OnGamepadDeattached(entry);

		return S_OK;
	}
} } }