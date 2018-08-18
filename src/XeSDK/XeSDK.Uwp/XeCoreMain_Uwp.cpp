#include "pch.h"
#include <XeSDK/XeCore.h>
#include <XeSDK/XeDrivers.h>

#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>

#include "XeDriversRenderingNullDriver.h"
#include "XeDriversRenderingD3D11Driver.h"
#include "XeDriversSoundNullDriver.h"
//#include "XeDriversSoundWasapiDriver.h"
#include "XeDriversSoundXAudioDriver.h"
//#include "XeDriverInputWindowsCustom.h"
#include "XeDriverInputXInput.h"

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

namespace Xe {
	Xe::Drivers::Rendering::NullDriver g_DriverRenderingNull;
	Xe::Drivers::Rendering::D3D11Driver g_DriverRenderingD3D11;
	Xe::Drivers::Sound::NullDriver g_DriverSoundNull;
	//Xe::Drivers::Sound::WasapiDriver g_DriverSoundWasapi;
	Xe::Drivers::Sound::XAudioDriver g_DriverSoundXAudio2;
	//Xe::Drivers::Input::WindowsCustom g_DriverInputCustom;
	Xe::Drivers::Input::XInput g_DriverInputXInput;

	void RegisterRenderingDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverRenderingNull);
		Xe::Drivers::RegisterDriver(&g_DriverRenderingD3D11);
	}

	void RegisterAudioDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverSoundNull);
		//Xe::Drivers::RegisterDriver(&g_DriverSoundWasapi);
		Xe::Drivers::RegisterDriver(&g_DriverSoundXAudio2);
	}

	void RegisterInputDrivers()
	{
		//Xe::Drivers::RegisterDriver(&g_DriverInputCustom);
		Xe::Drivers::RegisterDriver(&g_DriverInputXInput);
	}

	namespace Core
	{
		void Initialize()
		{
			RegisterRenderingDrivers();
			RegisterAudioDrivers();
			RegisterInputDrivers();
		}
	}
}
