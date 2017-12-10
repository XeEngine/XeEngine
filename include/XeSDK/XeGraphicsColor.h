#pragma once

namespace Xe {
	namespace Graphics {
		struct Color {
			enum Format {
				Format_Unknown = 0,
				Format_BGRA8888,
				Format_RGBA8888,
				Format_BGRX8888,
				Format_RGBX8888,
				Format_BGR888,
				Format_RGB888,
				Format_BGR565,
				Format_RGB565,
				Format_BGRA5551,
				Format_ARGB1555,
				Format_BGRX5551,
				Format_XRGB1555,
				Format_BGRA4444,
				Format_ARGB4444,
				Format_BGRX4444,
				Format_XRGB4444,
				Format_Indexed1,
				Format_Indexed2,
				Format_Indexed4,
				Format_Indexed8,
				Format_Indexed16,
				Format_Indexed32,
			};
			static svar GetBitsPerPixel(Format format);
			static bool IsIndexed(Format format);
			static Color FromRgba(u8 r, u8 g, u8 b, u8 a = 0xFF);

			union {
				struct {
					float r, g, b;
				};
				struct {
					float h, s, v;
				};
			};
			float a;

			Color();
			Color(float r, float g, float b);
			Color(float r, float g, float b, float a);
			Color(u32 rgba8888);
			Color(const Color &color);
			Color(const Color &color, float a);

			bool operator == (const Color& v) const;
			bool operator != (const Color& v) const;
			Color operator + (const Color& v) const;
			Color& operator += (const Color& v);
			Color operator - (const Color& v) const;
			Color& operator -= (const Color& v);
			Color operator * (const Color& v) const;
			Color& operator *= (const Color& v);

			const Color& Normalize();

			u32 ToRgba8888() const;
			Color ToHsv() const;
			Color FromHsv() const;

			float GetSaturation() const;
			const Color& SetSaturation(float value);
			float GetLuminance() const;
			const Color& SetLuminance(float value);
			float GetContrast() const;

			static u32 ToRgba8888(const Color &color);
			static Color ToHsv(const Color &color);
			static Color FromHsv(const Color &color);
			static Color Lerp(const Color &c1, const Color &c2, double m);
			// https://www.w3.org/TR/AERT#color-contrast
			static float GetContrast(const Color& color);

			static const Color Transparent;
			static const Color AliceBlue;
			static const Color AntiqueWhite;
			static const Color Aqua;
			static const Color Aquamarine;
			static const Color Azure;
			static const Color Beige;
			static const Color Bisque;
			static const Color Black;
			static const Color BlanchedAlmond;
			static const Color Blue;
			static const Color BlueViolet;
			static const Color Brown;
			static const Color BurlyWood;
			static const Color CadetBlue;
			static const Color Chartreuse;
			static const Color Chocolate;
			static const Color Coral;
			static const Color CornflowerBlue;
			static const Color Cornsilk;
			static const Color Crimson;
			static const Color Cyan;
			static const Color DarkBlue;
			static const Color DarkCyan;
			static const Color DarkGoldenrod;
			static const Color DarkGray;
			static const Color DarkGreen;
			static const Color DarkKhaki;
			static const Color DarkMagenta;
			static const Color DarkOliveGreen;
			static const Color DarkOrange;
			static const Color DarkOrchid;
			static const Color DarkRed;
			static const Color DarkSalmon;
			static const Color DarkSeaGreen;
			static const Color DarkSlateBlue;
			static const Color DarkSlateGray;
			static const Color DarkTurquoise;
			static const Color DarkViolet;
			static const Color DeepPink;
			static const Color DeepSkyBlue;
			static const Color DimGray;
			static const Color DodgerBlue;
			static const Color Firebrick;
			static const Color FloralWhite;
			static const Color ForestGreen;
			static const Color Fuchsia;
			static const Color Gainsboro;
			static const Color GhostWhite;
			static const Color Gold;
			static const Color Goldenrod;
			static const Color Gray;
			static const Color Green;
			static const Color GreenYellow;
			static const Color Honeydew;
			static const Color HotPink;
			static const Color IndianRed;
			static const Color Indigo;
			static const Color Ivory;
			static const Color Khaki;
			static const Color Lavender;
			static const Color LavenderBlush;
			static const Color LawnGreen;
			static const Color LemonChiffon;
			static const Color LightBlue;
			static const Color LightCoral;
			static const Color LightCyan;
			static const Color LightGoldenrodYellow;
			static const Color LightGreen;
			static const Color LightGray;
			static const Color LightPink;
			static const Color LightSalmon;
			static const Color LightSeaGreen;
			static const Color LightSkyBlue;
			static const Color LightSlateGray;
			static const Color LightSteelBlue;
			static const Color LightYellow;
			static const Color Lime;
			static const Color LimeGreen;
			static const Color Linen;
			static const Color Magenta;
			static const Color Maroon;
			static const Color MediumAquamarine;
			static const Color MediumBlue;
			static const Color MediumOrchid;
			static const Color MediumPurple;
			static const Color MediumSeaGreen;
			static const Color MediumSlateBlue;
			static const Color MediumSpringGreen;
			static const Color MediumTurquoise;
			static const Color MediumVioletRed;
			static const Color MidnightBlue;
			static const Color MintCream;
			static const Color MistyRose;
			static const Color Moccasin;
			static const Color NavajoWhite;
			static const Color Navy;
			static const Color OldLace;
			static const Color Olive;
			static const Color OliveDrab;
			static const Color Orange;
			static const Color OrangeRed;
			static const Color Orchid;
			static const Color PaleGoldenrod;
			static const Color PaleGreen;
			static const Color PaleTurquoise;
			static const Color PaleVioletRed;
			static const Color PapayaWhip;
			static const Color PeachPuff;
			static const Color Peru;
			static const Color Pink;
			static const Color Plum;
			static const Color PowderBlue;
			static const Color Purple;
			static const Color Red;
			static const Color RosyBrown;
			static const Color RoyalBlue;
			static const Color SaddleBrown;
			static const Color Salmon;
			static const Color SandyBrown;
			static const Color SeaGreen;
			static const Color SeaShell;
			static const Color Sienna;
			static const Color Silver;
			static const Color SkyBlue;
			static const Color SlateBlue;
			static const Color SlateGray;
			static const Color Snow;
			static const Color SpringGreen;
			static const Color SteelBlue;
			static const Color Tan;
			static const Color Teal;
			static const Color Thistle;
			static const Color Tomato;
			static const Color Turquoise;
			static const Color Violet;
			static const Color Wheat;
			static const Color White;
			static const Color WhiteSmoke;
			static const Color Yellow;
			static const Color YellowGreen;
		};
	}
}