#pragma once
#include <XeSDK/XeIOGamepad.h>

#include <wrl/client.h>
#include <Windows.Xbox.Input.h>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Xbox::Input;

namespace Xe { namespace IO {
	class XboxGamepad : public Gamepad
	{
		bool m_IsAvailable;
		ComPtr<IController> m_Controller;
		ComPtr<IController2> m_Controller2;
		ComPtr<IGamepad> m_Gamepad;

		bool IsAvailable() const;
		void Update();
		void SetRumbling(const Math::Vector2f& rumbling);
		void SetLightbar(const Graphics::Color& color);

	public:
		XboxGamepad(
			const GamepadEntry& entry,
			const ComPtr<IController>& controller);
		~XboxGamepad();
	};
} }