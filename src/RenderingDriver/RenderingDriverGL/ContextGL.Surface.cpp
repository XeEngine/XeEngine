#include "pch.h"
#ifdef PLATFORM_GL
#include "ContextGL.Common.h"
#include <XeSDK/IGraphicsDrawing2d.h>

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// ISurface implementation
		bool CContextGLCommon::CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc) {
			static const GLenum texType = GL_TEXTURE_2D;
			GLuint glId;
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &glId);
			glBindTexture(texType, glId);
			glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP);
			//glTexParameteri(texType, GL_TEXTURE_WRAP_R, GL_CLAMP); // 3D textures only
			glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			GLint alignment;
			if (dataDesc.pitch & 7)
			{
				if (dataDesc.pitch & 3)
				{
					if (dataDesc.pitch & 1)
						alignment = 1;
					else
						alignment = 2;
				}
				else
					alignment = 4;
			}
			else
				alignment = 8;
			glPixelStorei(GL_PACK_ALIGNMENT, alignment);
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

			const ColorFormatFallback& glFormat = COLORFORMAT_FALLBACK[format];
			glTexImage2D(texType, 0, glFormat.internalFormat, (GLsizei)size.x, (GLsizei)size.y,
				0, glFormat.glFormat, glFormat.type, dataDesc.data);
			//glActiveTexture(GL_TEXTURE0); // rendundant
			CSurface *pPrevSurface = m_Surface[0];
			if (pPrevSurface) glBindTexture(texType, pPrevSurface->m_GlId);
			else glBindTexture(texType, 0);
			if (!CHECKERROR) {
				*surface = nullptr;
				return false;
			}
			*surface = new CSurface(this, glId, type, size, format);
			return true;
		}
		void CContextGLCommon::SelectSurface(ISurface *surface, svar index) {
			if (index >= 0 && index < lengthof(m_Surface) && m_Surface[index] != surface) {
				if (surface != nullptr) {
					CSurface *p;
					if (surface->Query((IObject**)&p, CSurface::ID)) {
						if (p->m_context == this) {
							m_Drawing->Flush();
							if (m_Surface[index]) m_Surface[index]->Release();
							m_Surface[index] = p;
							m_Surface[index]->AddRef();
							glActiveTexture(GL_TEXTURE0 + index);
							glBindTexture(GL_TEXTURE_2D, p->m_GlId);
						}
						p->Release();
					}
					// If the object does not belong to current context, then ignore it.
				}
				else {
					if (m_Surface[index]) m_Surface[index]->Release();
					m_Surface[index] = nullptr;
					glActiveTexture(GL_TEXTURE0 + index);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}

		bool CContextGLCommon::CSurface::Query(IObject **obj, UID id) {
			switch (id) {
			case CSurface::ID:
			case ISurface::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextGLCommon::CSurface::CSurface(IContext *context, GLuint id, SurfaceType type, const Size &size, Color::Format format) :
			ISurface(type, size, format),
			m_context(context),
			m_GlId(id) {
		}
		CContextGLCommon::CSurface::~CSurface() {
		}
	}
}
#endif