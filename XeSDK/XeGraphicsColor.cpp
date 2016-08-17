#include "pch.h"
#include <XeSDK/XeGraphicsColor.h>
#include <XeSDK/XeMath.h>

#define COLOR_FLOAT8 255
#define COLOR_8FLOAT (1.0 / COLOR_FLOAT8)

namespace Xe {
	namespace Graphics {
		svar Color::GetBitsPerPixel(Format format) {
			static const u8 BPC[] =
			{
				0,  //ColorFormat_Unknown
				32,	//ColorFormat_BGRA8888
				32,	//ColorFormat_RGBA8888/
				32,	//ColorFormat_BGRX8888
				32,	//ColorFormat_RGBX8888
				24,	//ColorFormat_BGR888
				24,	//ColorFormat_RGB888
				16,	//ColorFormat_BGR565
				16,	//ColorFormat_RGB565
				16,	//ColorFormat_BGRX5551
				16,	//ColorFormat_RGBX5551
				16,	//ColorFormat_BGRA5551
				16,	//ColorFormat_RGBA5551
				16,	//ColorFormat_BGRX4444
				16,	//ColorFormat_RGBX4444
				16,	//ColorFormat_BGRA4444
				16,	//ColorFormat_RGBA4444
				1,	//ColorFormat_Indexed1
				2,	//ColorFormat_Indexed2
				4,	//ColorFormat_Indexed4
				8,	//ColorFormat_Indexed8
				16,	//ColorFormat_Indexed16
				32,	//ColorFormat_Indexed32
			};
			return (svar)BPC[format];
		}
		bool Color::IsIndexed(Format format) {
			static const bool ISINDEXED[] =
			{
				false,	//ColorFormat_Unknown
				false,	//ColorFormat_BGRA8888
				false,	//ColorFormat_RGBA8888
				false,	//ColorFormat_BGRX8888
				false,	//ColorFormat_RGBX8888
				false,	//ColorFormat_BGR888
				false,	//ColorFormat_RGB888
				false,	//ColorFormat_BGR565
				false,	//ColorFormat_RGB565
				false,	//ColorFormat_BGRX5551
				false,	//ColorFormat_RGBX5551
				false,	//ColorFormat_BGRA5551
				false,	//ColorFormat_RGBA5551
				false,	//ColorFormat_BGRX4444
				false,	//ColorFormat_RGBX4444
				false,	//ColorFormat_BGRA4444
				false,	//ColorFormat_RGBA4444
				true,	//ColorFormat_Indexed1
				true,	//ColorFormat_Indexed2
				true,	//ColorFormat_Indexed4
				true,	//ColorFormat_Indexed8
				true,	//ColorFormat_Indexed16
				true,	//ColorFormat_Indexed32
			};
			return ISINDEXED[format];
		}

		Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
		Color::Color(float r, float g, float b) :
			r(r), g(g), b(b), a(1.0f) {}
		Color::Color(float r, float g, float b, float a) :
			r(r), g(g), b(b), a(a) {}
		Color::Color(u32 rgba8888) :
			r(((rgba8888 >> 16) & 0xFF) / (float)COLOR_FLOAT8),
			g(((rgba8888 >> 8) & 0xFF) / (float)COLOR_FLOAT8),
			b((rgba8888 & 0xFF) / (float)COLOR_FLOAT8),
			a(((rgba8888 >> 24) & 0xFF) / (float)COLOR_FLOAT8) {}
		Color::Color(const Color &color) :
			r(color.r), g(color.g), b(color.b), a(color.a) { }
		Color::Color(const Color &color, float a) :
			r(color.r), g(color.g), b(color.b), a(a) { }

		bool Color::operator == (const Color& v) const {
			static const float EPSILON = 0.00001f;
			return (Math::Abs(r - v.r) < EPSILON) &&
				(Math::Abs(g - v.g) < EPSILON) &&
				(Math::Abs(b - v.b) < EPSILON) &&
				(Math::Abs(a - v.a) < EPSILON);
		}
		bool Color::operator != (const Color& v) const {
			return !(*this == v);
		}
		Color Color::operator + (const Color& v) const {
			return Color(r + v.r, g + v.g, b + v.b, a + v.a);
		}
		Color& Color::operator += (const Color& v) {
			r += v.r;
			g += v.g;
			b += v.b;
			a += v.a;
			return *this;
		}
		Color Color::operator - (const Color& v) const {
			return Color(r - v.r, g - v.g, b - v.b, a - v.a);
		}
		Color& Color::operator -= (const Color& v) {
			r -= v.r;
			g -= v.g;
			b -= v.b;
			a -= v.a;
			return *this;
		}
		Color Color::operator * (const Color& v) const {
			return Color(r * v.r, g * v.g, b * v.b, a * v.a);
		}
		Color& Color::operator *= (const Color& v) {
			r *= v.r;
			g *= v.g;
			b *= v.b;
			a *= v.a;
			return *this;
		}

		const Color& Color::Normalize() {
			if (r < 0.0f) r = 0.0f;
			else if (r > 1.0f) r = 1.0f;
			if (g < 0.0f) g = 0.0f;
			else if (g > 1.0f) g = 1.0f;
			if (b < 0.0f) b = 0.0f;
			else if (b > 1.0f) b = 1.0f;
			if (a < 0.0f) a = 0.0f;
			else if (a > 1.0f) a = 1.0f;
			return *this;
		}

		u32 Color::ToRgba8888() const {
			return ToRgba8888(*this);
		}
		Color Color::ToHsv() const {
			return ToHsv(*this);
		}
		Color Color::FromHsv() const {
			return FromHsv(*this);
		}

		float Color::GetSaturation() const {
			float min, max, delta;
			if (r > g) {
				min = g;
				max = r;
			}
			else {
				min = r;
				max = g;
			}
			if (min > b) min = b;
			if (max < b) max = b;
			delta = max - min;
			return (max != 0.0f) ? delta / max : 0.0f;
		}
		const Color& Color::SetSaturation(float value) {
			Color tmp = ToHsv();
			tmp.s = value;
			return *this = FromHsv(tmp);
		}
		float Color::GetLuminance() const {
			float max = r > g ? r : g;
			return max > b ? max : b;
		}
		const Color& Color::SetLuminance(float value) {
			/*if (value > 0.0f) {
				float min, max;
				if (r > g) {
					min = g;
					max = r;
				}
				else {
					min = r;
					max = g;
				}
				if (min > b) min = b;
				if (max < b) max = b;

				if (max > 0.0f) {
					float m = min / max * value + min;
					r += m;
					g += m;
					b += m;
				}
				else
					r = g = b = value;
			}
			else
				r = g = b = 0.0f;
			return *this;*/
			Color tmp = ToHsv();
			tmp.v = value;
			return *this = FromHsv(tmp);
		}
		float Color::GetContrast() const {
			return GetContrast(*this);
		}

		u32 Color::ToRgba8888(const Color &color) {
			struct Rgba8888 {
				union {
					struct {
						u8 b, g, r, a;
					};
					u32 rgba;
				};
			};
			Rgba8888 c;
			c.b = (u8)Math::Truncate(Math::Max(0.0f, Math::Min(1.0f, color.b)) * COLOR_FLOAT8);
			c.g = (u8)Math::Truncate(Math::Max(0.0f, Math::Min(1.0f, color.g)) * COLOR_FLOAT8);
			c.r = (u8)Math::Truncate(Math::Max(0.0f, Math::Min(1.0f, color.r)) * COLOR_FLOAT8);
			c.a = (u8)Math::Truncate(Math::Max(0.0f, Math::Min(1.0f, color.a)) * COLOR_FLOAT8);
			return c.rgba;
		}
		Color Color::ToHsv(const Color &color) {
			float min, max, delta, hue;
			if (color.r > color.g) {
				min = color.g;
				max = color.r;
			}
			else {
				min = color.r;
				max = color.g;
			}
			if (min > color.b) min = color.b;
			if (max < color.b) max = color.b;
			delta = max - min;

			if (delta != 0.0f) {
				if (color.r == max)
					hue = (color.g - color.b) / delta;
				else if (color.g == max)
					hue = 2.f + (color.b - color.r) / delta;
				else if (color.b == max)
					hue = 4.f + (color.r - color.g) / delta;
				else
					hue = 0.0f;
			}
			else
				hue = 0.0f;
			if (hue < 0.0f)
				hue += 6.0f;
			return Color(hue * 60.0f, (max != 0.0f) ? delta / max : 0.0f, max);
		}
		Color Color::FromHsv(const Color &color) {
			float r, g, b;
			float s = Math::Min(1.0f, color.s);
			if (s < 0.0000001f)
				return Color(color.v, color.v, color.v, color.a);
			float h = Math::Fmod(color.h, 360.0f) / 60.0f;
			float v = Math::Min(1.0f, color.v);

			float chroma = s * v;
			float x = chroma * (1.0f - Math::Abs((Math::Fmod(h, 2.0f) - 1.0f)));
			float m = v - chroma;
			
			switch ((svar)(h) % 6) {
			case 0:
				r = chroma;
				g = x;
				b = 0.0f;
				break;
			case 1:
				r = x;
				g = chroma;
				b = 0.0f;
				break;
			case 2:
				r = 0.0f;
				g = chroma;
				b = x;
				break;
			case 3:
				r = 0.0f;
				g = x;
				b = chroma;
				break;
			case 4:
				r = x;
				g = 0.0f;
				b = chroma;
				break;
			case 5:
				r = chroma;
				g = 0.0f;
				b = x;
				break;
			}
			return Color(r + m, g + m, b + m, color.a);
		}
		Color Color::Lerp(const Color &c1, const Color &c2, double m) {
			return Color((float)Math::Lerp(m, (double)c1.b, (double)c2.b),
				(float)Math::Lerp(m, (double)c1.g, (double)c2.g),
				(float)Math::Lerp(m, (double)c1.r, (double)c2.r),
				(float)Math::Lerp(m, (double)c1.a, (double)c2.a));
		}
		float Color::GetContrast(const Color& color) {
			return color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
		}
		
		const Color Color::AliceBlue            (0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f);
		const Color Color::AntiqueWhite         (0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f);
		const Color Color::Aqua                 (0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::Aquamarine           (0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f);
		const Color Color::Azure                (0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::Beige                (0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f);
		const Color Color::Bisque               (1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f);
		const Color Color::Black                (0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::BlanchedAlmond       (1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f);
		const Color Color::Blue                 (0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::BlueViolet           (0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f);
		const Color Color::Brown                (0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f);
		const Color Color::BurlyWood            (0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f);
		const Color Color::CadetBlue            (0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f);
		const Color Color::Chartreuse           (0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::Chocolate            (0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f);
		const Color Color::Coral                (1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f);
		const Color Color::CornflowerBlue       (0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f);
		const Color Color::Cornsilk             (1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f);
		const Color Color::Crimson              (0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f);
		const Color Color::Cyan                 (0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::DarkBlue             (0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f);
		const Color Color::DarkCyan             (0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f);
		const Color Color::DarkGoldenrod        (0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f);
		const Color Color::DarkGray             (0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f);
		const Color Color::DarkGreen            (0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f);
		const Color Color::DarkKhaki            (0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f);
		const Color Color::DarkMagenta          (0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f);
		const Color Color::DarkOliveGreen       (0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f);
		const Color Color::DarkOrange           (1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f);
		const Color Color::DarkOrchid           (0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f);
		const Color Color::DarkRed              (0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::DarkSalmon           (0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f);
		const Color Color::DarkSeaGreen         (0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f);
		const Color Color::DarkSlateBlue        (0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f);
		const Color Color::DarkSlateGray        (0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f);
		const Color Color::DarkTurquoise        (0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f);
		const Color Color::DarkViolet           (0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f);
		const Color Color::DeepPink             (1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f);
		const Color Color::DeepSkyBlue          (0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f);
		const Color Color::DimGray              (0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f);
		const Color Color::DodgerBlue           (0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f);
		const Color Color::Firebrick            (0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f);
		const Color Color::FloralWhite          (1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f);
		const Color Color::ForestGreen          (0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f);
		const Color Color::Fuchsia              (1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::Gainsboro            (0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f);
		const Color Color::GhostWhite           (0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f);
		const Color Color::Gold                 (1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f);
		const Color Color::Goldenrod            (0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f);
		const Color Color::Gray                 (0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f);
		const Color Color::Green                (0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f);
		const Color Color::GreenYellow          (0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f);
		const Color Color::Honeydew             (0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f);
		const Color Color::HotPink              (1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f);
		const Color Color::IndianRed            (0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f);
		const Color Color::Indigo               (0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f);
		const Color Color::Ivory                (1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f);
		const Color Color::Khaki                (0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f);
		const Color Color::Lavender             (0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f);
		const Color Color::LavenderBlush        (1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f);
		const Color Color::LawnGreen            (0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f);
		const Color Color::LemonChiffon         (1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f);
		const Color Color::LightBlue            (0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f);
		const Color Color::LightCoral           (0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f);
		const Color Color::LightCyan            (0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::LightGoldenrodYellow (0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f);
		const Color Color::LightGreen           (0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f);
		const Color Color::LightGray            (0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f);
		const Color Color::LightPink            (1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f);
		const Color Color::LightSalmon          (1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f);
		const Color Color::LightSeaGreen        (0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f);
		const Color Color::LightSkyBlue         (0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f);
		const Color Color::LightSlateGray       (0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f);
		const Color Color::LightSteelBlue       (0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f);
		const Color Color::LightYellow          (1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f);
		const Color Color::Lime                 (0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::LimeGreen            (0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f);
		const Color Color::Linen                (0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f);
		const Color Color::Magenta              (1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::Maroon               (0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::MediumAquamarine     (0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f);
		const Color Color::MediumBlue           (0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f);
		const Color Color::MediumOrchid         (0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f);
		const Color Color::MediumPurple         (0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f);
		const Color Color::MediumSeaGreen       (0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f);
		const Color Color::MediumSlateBlue      (0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f);
		const Color Color::MediumSpringGreen    (0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f);
		const Color Color::MediumTurquoise      (0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f);
		const Color Color::MediumVioletRed      (0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f);
		const Color Color::MidnightBlue         (0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f);
		const Color Color::MintCream            (0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f);
		const Color Color::MistyRose            (1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f);
		const Color Color::Moccasin             (1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f);
		const Color Color::NavajoWhite          (1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f);
		const Color Color::Navy                 (0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f);
		const Color Color::OldLace              (0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f);
		const Color Color::Olive                (0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f);
		const Color Color::OliveDrab            (0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f);
		const Color Color::Orange               (1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f);
		const Color Color::OrangeRed            (1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f);
		const Color Color::Orchid               (0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f);
		const Color Color::PaleGoldenrod        (0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f);
		const Color Color::PaleGreen            (0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f);
		const Color Color::PaleTurquoise        (0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f);
		const Color Color::PaleVioletRed        (0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f);
		const Color Color::PapayaWhip           (1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f);
		const Color Color::PeachPuff            (1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f);
		const Color Color::Peru                 (0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f);
		const Color Color::Pink                 (1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f);
		const Color Color::Plum                 (0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f);
		const Color Color::PowderBlue           (0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f);
		const Color Color::Purple               (0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f);
		const Color Color::Red                  (1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::RosyBrown            (0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f);
		const Color Color::RoyalBlue            (0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f);
		const Color Color::SaddleBrown          (0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f);
		const Color Color::Salmon               (0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f);
		const Color Color::SandyBrown           (0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f);
		const Color Color::SeaGreen             (0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f);
		const Color Color::SeaShell             (1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f);
		const Color Color::Sienna               (0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f);
		const Color Color::Silver               (0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f);
		const Color Color::SkyBlue              (0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f);
		const Color Color::SlateBlue            (0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f);
		const Color Color::SlateGray            (0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f);
		const Color Color::Snow                 (1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f);
		const Color Color::SpringGreen          (0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f);
		const Color Color::SteelBlue            (0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f);
		const Color Color::Tan                  (0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f);
		const Color Color::Teal                 (0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f);
		const Color Color::Thistle              (0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f);
		const Color Color::Tomato               (1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f);
		const Color Color::Transparent          (0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f);
		const Color Color::Turquoise            (0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f);
		const Color Color::Violet               (0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f);
		const Color Color::Wheat                (0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f);
		const Color Color::White                (1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
		const Color Color::WhiteSmoke           (0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f);
		const Color Color::Yellow               (1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f);
		const Color Color::YellowGreen          (0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f);
	}
}