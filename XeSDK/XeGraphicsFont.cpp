#include "pch.h"
#include <XeSDK/IGraphicsFont.h>

namespace Xe {
	namespace Graphics {
		namespace Font {
			bool IFont::IsFixedWidth() const {
				return m_isFixedWidth;
			}
			svar IFont::GetPxWidth() const {
				return m_pxWidth;
			}
			svar IFont::GetPxHeight() const {
				return m_pxHeight;
			}
			svar IFont::GetChFirst() const {
				return m_chFirst;
			}
			svar IFont::GetChCount() const {
				return m_chCount;
			}
			uint IFont::GetDefaultCharacter() const {
				return m_chDefault;
			}
			Color::Format IFont::GetFormat() const {
				return m_format;
			}
		}
	}
}