#pragma once
#include <XeSDK/ICoreView.h>

namespace Xe { namespace Core {
	class DummyKeyboardHandler : public IKeyboardHandler
	{
		void OnCharacter(const IO::CharacterEvent& e) { }
		void OnKeyPressed(const IO::KeyboardEvent& e) { }
		void OnKeyReleased(const IO::KeyboardEvent& e) { }
	};
} }