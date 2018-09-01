#include "pch.h"
#include <XeSDK/XeStorage.h>
#include <XeSDK/IStorageDirectory.h>
#include <XeSDK/XeDrivers.h>

#include "XeDriversRenderingNullDriver.h"
#include "XeDriversRenderingD3D11Driver.h"
//#include "XeDriversRenderingOpenGlDriver.h"
#include "XeDriversSoundNullDriver.h"
//#include "XeDriversSoundWasapiDriver.h"
//#include "XeDriversSoundXAudioDriver.h"
//#include "XeDriverInputWindowsCustom.h"
#include "XeDriversInputXbox.h"

namespace Xe {

	Xe::Drivers::Rendering::NullDriver g_DriverRenderingNull;
	Xe::Drivers::Rendering::D3D11Driver g_DriverRenderingD3D11;
	//Xe::Drivers::Rendering::OpenGlDriver g_DriverRenderingOpenGl;
	Xe::Drivers::Sound::NullDriver g_DriverSoundNull;
	//Xe::Drivers::Sound::WasapiDriver g_DriverSoundWasapi;
	//Xe::Drivers::Sound::XAudioDriver g_DriverSoundXAudio2;
	//Xe::Drivers::Input::WindowsCustom g_DriverInputCustom;
	Xe::Drivers::Input::XboxInput g_DriverInputXbox;

	void RegisterRenderingDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverRenderingNull);
		Xe::Drivers::RegisterDriver(&g_DriverRenderingD3D11);
		//Xe::Drivers::RegisterDriver(&g_DriverRenderingOpenGl);
	}

	void RegisterAudioDrivers()
	{
		Xe::Drivers::RegisterDriver(&g_DriverSoundNull);
		//Xe::Drivers::RegisterDriver(&g_DriverSoundWasapi);
		//Xe::Drivers::RegisterDriver(&g_DriverSoundXAudio2);
	}

	void RegisterInputDrivers()
	{
		//Xe::Drivers::RegisterDriver(&g_DriverInputCustom);
		Xe::Drivers::RegisterDriver(&g_DriverInputXbox);
	}

	namespace Core
	{
		void Initialize()
		{
			HRESULT hr = RoInitialize(RO_INIT_MULTITHREADED);
			hr = hr;

			RegisterRenderingDrivers();
			RegisterAudioDrivers();
			RegisterInputDrivers();
		}
	}
}
