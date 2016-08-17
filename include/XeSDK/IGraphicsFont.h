#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeString.h>
#include <XeSDK/IGraphicsContext.h>

namespace Xe {
	namespace Graphics {
		//! \brief Font management
		namespace Font {
			class IFont : public IObject {
			public:
				static const UID ID = 0x151c8b12d76f4db4ULL;
				
				//! \brief Used to describe a single character
				struct CharacterDesc {
					//! \brief width in pixel of character 
					svar width;
					//! \brief character id
					tchar ch;
				};

				//! \brief check if every character has fixed width
				//! \return true if width is fixed, else false
				bool IsFixedWidth() const;

				//! \brief get the maximum width through the characters
				//! \return greatest width between all characters
				/** \details when IsFixedWidth is true, every character of
				 * current font shares this returned width.
				 */
				//! \sa IsFixedWidth, GetPxHeight
				svar GetPxWidth() const;

				//! \brief get the height of each letter
				/** \details every character must have the same height.
				 */
				//! \sa IsFixedWidth, GetPxWidth
				svar GetPxHeight() const;

				//! \brief get the first character supported by this font
				//! \sa GetChCount
				svar GetChFirst() const;

				//! \brief get how characters are supported starting from the first one
				//! \sa GetChFirst
				svar GetChCount() const;

				//! \brief get the character used to replace a not-found character
				//! \return id of default character
				uint GetDefaultCharacter() const;

				//! \brief get the format used from raster font
				//! return color format used by the font on GetCharacter
				/** \details Every time that a font is loaded, internally it
				 * will be decompressed in a specific format defined by
				 * GetFormat. Indexed raster formats must be decompressed
				 into Format_Indexed8, while non-indexed data must be
				 * decompressed into Format_BGRA8888.
				 */
				Color::Format GetFormat() const;

				//! \brief get the name of the font
				//! \return an empty string or the font's name
				virtual const String &GetName() const = 0;

				//! \brief get specified character
				//! \param[out] chDesc description of specified character
				//! \param[in] ch character to obtain
				//! \return false if specified character was not found
				/** \details When specified character was not found by current
				 * font, chDesc.data is nullptr, chDesc.length and chDesc.width
				 * are 0 but chDesc.ch contains default character that it's
				 * used when a character is not found.
				 */
				virtual bool GetCharacter(CharacterDesc &chDesc, uint ch) const = 0;

				//! \brief read a character into specified pointer
				virtual bool ReadCharater(void *dst, svar pitch, svar height, Color::Format format, uint ch) = 0;

			protected:
				bool m_isFixedWidth;
				svar m_pxWidth;
				svar m_pxHeight;
				svar m_chFirst;
				svar m_chCount;
				uint m_chDefault;
				Color::Format m_format;
			};
		}
	}
}