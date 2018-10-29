#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/IObject.h>
#include <XeSDK/IDelegate.h>

namespace Xe { namespace IO {
} }

namespace Xe { namespace Core {

	//! \brief Unique identifier for a keyboard button.
	typedef u32 ScanKey;

	//! \brief Translation from a scancode through globalization keyboard.
	typedef u32 VirtualKey;

	//! \brief Describe the event for a keyboard button.
	struct KeyboardEventArgs
	{
		//! \brief The physical ID of the key.
		ScanKey ScanCode;

		//! \brief Translated physical key as keyboard language describes.
		VirtualKey VirtualCode;
	};

	//! \brief Describe the event for a character
	struct CharacterEventArgs
	{
		//! \brief The physical ID of the key
		ScanKey ScanCode;

		//! \brief A character representation of the pressed key
		u32 Character;
	};

	struct CharPressedEventArgs : public CharacterEventArgs {};

	struct KeyPressedEventArgs : public KeyboardEventArgs {};

	struct KeyReleasedEventArgs : public KeyboardEventArgs {};

	interface IKeyboardEventHandler :
		public IObject,
		public IDelegate<const CharPressedEventArgs&>,
		public IDelegate<const KeyPressedEventArgs&>,
		public IDelegate<const KeyReleasedEventArgs&>
	{ };
} }


// DEPRECATED - Compatible layer

namespace Xe { namespace IO {
	//! \warning DEPRECATED
	//! \brief Unique identifier for a keyboard button.
	typedef Xe::Core::ScanKey ScanKey;

	//! \warning DEPRECATED
	//! \brief Translation from a scancode through globalization keyboard.
	typedef Xe::Core::VirtualKey VirtualKey;

	//! \brief Describe the event for a keyboard button.
	struct KeyboardEvent : public Xe::Core::KeyboardEventArgs
	{
		KeyboardEvent(const Xe::Core::KeyboardEventArgs& args) :
			Xe::Core::KeyboardEventArgs(args) {}
	};

	//! \brief Describe the event for a character.
	struct CharacterEvent : public Xe::Core::CharPressedEventArgs
	{
		CharacterEvent(const Xe::Core::CharPressedEventArgs& args) :
			Xe::Core::CharPressedEventArgs(args) {}
	};
} }

namespace Xe { namespace Core {
	//! \warning DEPRECATED
	interface IKeyboardHandler : public virtual IObject
	{
		virtual void OnCharacter(const IO::CharacterEvent& e) = 0;
		virtual void OnKeyPressed(const IO::KeyboardEvent& e) = 0;
		virtual void OnKeyReleased(const IO::KeyboardEvent& e) = 0;
	};
} }