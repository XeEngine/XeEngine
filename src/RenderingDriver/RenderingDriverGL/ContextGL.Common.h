#pragma once

#if defined(PLATFORM_GL)
#include <XeSDK/ICoreView.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMathVector2.h>
#include "ContextGL.Library.h"
#include <gl/GL.h>

#define CHECKERROR CheckError(_XE_WIDE(__FILE__), __LINE__)

namespace Xe {
	namespace Graphics {
		class CContextGLCommon : public IContext {
		public:
			struct Format {
				u32 id;
				u8 r, g, b, a;
				u8 depth, stencil;
			};
			struct ColorFormatFallback
			{
				Color::Format format;
				GLint internalFormat;
				GLint glFormat;
				GLint type;
			};
			static const svar MaximumQuadsCount = 16383;
			static const ColorFormatFallback COLORFORMAT_FALLBACK[];

			CContextGLCommon(Core::IView *pView);
			~CContextGLCommon();

			virtual bool Initialize(const ContextProperties& properties);
			GLVersion LoadLibraries();
			GLVersion GetVersion() const;

			///////////////////////////////////////////////////////////////////
			// Common stuff
			void SetInternalResolution(const Size& size);
			const Color &GetClearColor() const;
			void SetClearColor(const Color &color);
			float GetClearDepth() const;
			void SetClearDepthSub(float depth);
			int GetClearStencil() const;
			void SetClearStencil(int stencil);
			
			void Clear(svar clearmask);
			void Draw(Primitive primitive, u32 count, u32 start);
			void DrawIndexed(Primitive primitive, u32 count, u32 start);

			///////////////////////////////////////////////////////////////////
			// Surface management
			class CSurface : public ISurface {
			public:
				static const UID ID = 0x44788e8f2019499cULL;
				IContext *m_context;
				GLuint m_GlId;

				bool Query(IObject **obj, UID id);
				CSurface(IContext *context, GLuint id, SurfaceType type, const Size &size, Color::Format format);
				~CSurface();

				// Inherited via ISurface
				bool SubLock(DataDesc & map, LockType type);
				void SubUnlock();
			};
			bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
			void SelectSurface(ISurface *surface, svar index);

			///////////////////////////////////////////////////////////////////
			// Buffer management
			bool CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData);
			void SelectBuffer(IBuffer *pBuffer);

			///////////////////////////////////////////////////////////////////
			// States management
			class CDepthStencilState : public IDepthStencilState {
			public:
				static const UID ID = 0xf66abe80e3fd46adULL;

				IContext *m_context;

				GLboolean m_zwriteenable;
				GLboolean m_zwritemask;
				GLenum m_zfunc;
				GLboolean m_stencilTestEnabled;
				GLuint m_stencilMask;
				GLenum m_stencilFrontFaceComp;
				GLenum m_stencilBackFaceComp;

				bool Query(IObject **obj, UID id);
				CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc);
				~CDepthStencilState();
			};
			bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc);
			void SelectDepthStencilState(IDepthStencilState *depthStencilState);

			///////////////////////////////////////////////////////////////////
			// Context specifics
			void SelectQuadIndexBuffer();
			void SelectSurfaceId(GLuint id, GLenum index);
			bool CompileShader(const char* source, const int length, const unsigned int type, GLint& id);
			void DeleteShader(GLint id);
			bool BuildProgram(GLint* shaders, unsigned int count, GLint& id);
			void DeleteProgram(GLint id);
			void ShaderError(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);

			// Implemented between operating systems
			static svar GetFormatsCount();
			static svar GetFormatsArray(Format *format, svar maxcount);
			static bool SetFormat(const Format &format);

			static GLenum GetPrimitive(Primitive primitive);

		protected:
			Core::IView *m_pView;
			IDrawing2d *m_Drawing;

			bool CheckError(ctstring filename, svar line);
			ctstring GetMessageError(GLenum error);
		private:
			bool m_LibrariesLoaded;
			GLVersion m_Version;
			Color m_ClearColor;
			float m_ClearDepth;
			int m_ClearStencil;
			CSurface *m_Surface[0x100];
			CDepthStencilState *m_DepthStencilState;
			GLuint m_QuadIndexBuffer;

			bool m_IsSamplingEnabled;
			bool m_IsFboSupported;
			GLuint m_idFboSampler;
		};
	}
}
#endif