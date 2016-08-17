#pragma once
#include <XeSDK/IGraphicsResource.h>

namespace Xe {
	namespace Graphics {
		//! \brief Type of surface
		enum SurfaceType {
			//! \brief unknown type of surface
			//! \details it denotes when a surface is not valid
			SurfaceType_Unknown = 0,
			//! \brief surface in main memory
			/** \details this move the surface into the RAM. It can be useful
			* in order to modify data there before to upload it somewhere.
			* Can be written from:
			*	Type_Memory (FAST, CPU),
			*	Type_Texture (SLOW, GPU->RAM->CPU)
			* Can draw on:
			*	Type_Memory (FAST, CPU),
			*	Type_Texture (SLOW, RAM->GPU)
			* Limitations: scaling and rotation not supported
			*/
			//SurfaceType_Memory = 1,
			//! \brief surface as a texture
			/** \details the surface will be used simply as a GPU texture
			* Can be written from:
			*	Type_Memory (SLOW, RAM->GPU),
			* Can draw on:
			*	Type_Memory (SLOW, GPU->RAM),
			*	Type_Texture (FAST, GPU->DRAW)
			*	Type_Target (FAST, GPU->DRAW)
			* Limitations: cannot be used as drawn surface
			*/
			SurfaceType_Texture = 2,
			//! \brief surface as a render target
			/** \details the surface will be used simply as a GPU texture
			* Can be written from:
			*	Type_Texture (FAST, GPU->DRAW)
			*	Type_Target (FAST, GPU->DRAW),
			* Can draw on:
			*	Type_Texture (FAST, GPU->DRAW)
			*	Type_Target (FAST, GPU->DRAW)
			* Limitations: can't be locked or initialized from main memory
			*/
			SurfaceType_Target = 3,
		};

		//! \brief Surface used as texture or render target
		class ISurface : public IObject {
			SurfaceType m_type;
			Size m_size;
			Color::Format m_colorformat;
		public:
			static const UID ID = 0xffc01cf158aa4b71ULL;

			ISurface(SurfaceType type, const Size& size, Color::Format format) :
				m_type(type), m_size(size), m_colorformat(format) {}

			//! \brief get the type of the surface
			SurfaceType GetType() const {
				return m_type;
			}
			//! \brief get the size in pixel
			const Size &GetSize() const {
				return m_size;
			}
			//! \brief get the color format
			Color::Format GetColorFormat() const {
				return m_colorformat;
			}
		};
	}
}