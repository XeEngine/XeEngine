#pragma once
#include <XeSDK/IGraphicsFont.h>
#include <XeSDK/IGraphicsContext.h>

namespace Xe {
	namespace Graphics {
		namespace Font {
			//! \brief Font ready to be used as a graphic's surface
			class FontUV : public IObject {
			public:
				static const UID ID = 0xca6326cae4994187ULL;

				struct FontFrame {
					Size size;
					Math::Vector2f uv[4];
				};

				bool Query(IObject **obj, UID id);

				//! \brief Create the UV from an already opened font
				//! \param[in] font correctly opened
				//! \param[in] context graphics's context that will bind the font's surface
				//! \param[in] format of font's surface; Currently only B8G8R8A8 is valid.
				FontUV(IFont *font, Graphics::IContext *context, Graphics::Color::Format format);
				~FontUV();

				//! \brief get the surface where font's characters are located
				bool GetSurface(Graphics::ISurface **surface);

				//! \brief get the frame of specified character
				//! \param[out] frame that will be used for character's drawing
				//! \param[in] ch character to get
				//! \return true if specified character is present into the specified font
				bool GetCharacter(FontFrame &frame, uint ch);

				//! \brief get default character
				//! \return id of default character
				uint GetDefaultCharacter() const;

			private:
				IFont *m_font;
				Graphics::IContext *m_context;
				Graphics::ISurface *m_surface;
				Math::Vector2f *m_pUv;
				svar m_chFirst;
				svar m_chCount;
				uint m_chDefault;

				static svar GetNearestPowerOfTwo(svar x);
				static const Size GetSurfaceSize(const Size &size, svar count, bool powerOfTwo /*IGNORED*/);
				bool CreateSurface(Color::Format format);
				void CreateUV();
			};
		}
	}
}