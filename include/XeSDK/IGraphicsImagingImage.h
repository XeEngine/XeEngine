#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/IIoStream.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeGraphicsColor.h>

namespace Xe {
	namespace Graphics {
		namespace Imaging {
			class IImage : public IObject {
				Size m_size;
				Color::Format m_format;
				uvar m_colorsCount;
				uvar m_stride;

			public:
				//! \brief interface id
				static const UID ID = 0x060b75906c5c4158ULL;

				IImage(const Size& size, Color::Format format, uvar colorsCount, uvar stride) :
					m_size(size), m_format(format), m_stride(stride) {}

				//! \brief get the size in pixel
				//! \return size
				const Size& GetSize() const {
					return m_size;
				}
				//! \brief get the color format
				Color::Format GetFormat() const {
					return m_format;
				}
				//! \brief get how colors there are in current palette
				//! \return a number between 1 and 256, 0 if palette is unused.
				uvar GetColorsCount() const {
					return m_colorsCount;
				}
				//! \brief get how bytes per line are used
				//! \return number of bytes used for every read line
				/** \details This value is used to align data in order to speed-up image decoding. Usually, when
				 * GetSize().x is multiple of 2, this value is equal to GetSize().x*GetBitsPerPixel(GetFormat())/8.
				 */
				uvar GetStride() const {
					return m_stride;
				}

				//! \brief read a single line of pixels
				//! \param[in,out] dst destination pointer where the read data will be written
				//! \param[in] offset of dst
				//! \return false if the last line was already reached
				/** \details for every line read, a fixed number of bytes equals to GetStride will be written to dst.
				 */
				virtual bool ReadLine(void *dst, svar offset) = 0;

				//! \brief read a palette from image
				//! \param[in,out] dst destination palette in BGRA format
				//! \param[in] count number of colors to read
				//! \return number of colors read
				virtual uvar ReadPalette(u32 *dst, uvar count) = 0;
			};
		}
	}
}