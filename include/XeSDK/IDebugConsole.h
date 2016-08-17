#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeGraphicsColor.h>

namespace Xe {
	namespace Debug {
		class IConsole : public IObject {
		public:
			typedef Xe::Math::Vector2<svar> Coord;
			typedef Xe::Graphics::Color Color;

			struct Properties {
				Coord coord;
				Color backColor;
				Color foreColor;
				bool isCursorVisible;

				Properties() :
					backColor(Graphics::Color::Black),
					foreColor(Graphics::Color::White),
					isCursorVisible(true)
					{}
			};

			static const UID ID = 0x3e254c5495c34185ULL;

			//! \brief get the current property state of console
			//! \return state of console
			virtual const Properties &GetProperties() const = 0;
			virtual void SetProperties(const Properties& properties) = 0;
			virtual const Color &GetForeColor() const = 0;
			virtual void SetForeColor(const Color &color) = 0;
			virtual void Clear() = 0;
			virtual void PutChar(tchar ch) = 0;
			virtual void Print(ctstring fmt, ...) = 0;
		};
	}
}