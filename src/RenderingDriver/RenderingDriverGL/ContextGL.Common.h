#pragma once

#include <XeSDK/ICoreView.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeObjPtr.h>
#include "ContextGL.Library.h"
#include "ContextGL.Buffer.h"
#include <gl/GL.h>

#define CHECKERROR CheckError(__func__, XE__FILE__, __LINE__)

namespace Xe { namespace Graphics {
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

		CContextGLCommon(Xe::Core::IFrameView *pView);
		~CContextGLCommon();

		virtual bool Initialize(const ContextInitDesc& properties);
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

		bool IsScissorEnabled() const;
		void SetScissorEnabled(bool enabled);
		const Xe::Math::Rectangle<int> GetScissor() const;
		void SetScissor(const Xe::Math::Rectangle<int>& scissor);
			
		void Clear(svar clearmask);
		void Draw(Primitive primitive, u32 count, u32 start);
		void DrawIndexed(Primitive primitive, u32 count, u32 start);

		///////////////////////////////////////////////////////////////////
		// Surface management
		class CSurface : public ISurface {
		public:
			IContext *m_context;
			GLuint m_GlId;

			CSurface(IContext *context, GLuint id, SurfaceType type, const Size &size, Color::Format format);
			~CSurface();

			// Inherited via ISurface
			bool SubLock(DataDesc & map, LockType type);
			void SubUnlock();
		};
		bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
		void SelectSurface(ISurface *surface, svar index);

		void GetRenderingSurface(ISurface** ppSurface);
		void SetRenderingSurface(ISurface* surface);

		///////////////////////////////////////////////////////////////////
		// Buffer management
		bool CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData);
		void GetVertexBuffer(IBuffer** ppBuffer);
		void SetVertexBuffer(IBuffer* pBuffer);
		void GetIndexBuffer(IBuffer** ppBuffer);
		void SetIndexBuffer(IBuffer* pBuffer);

		///////////////////////////////////////////////////////////////////
		// States management
		class CDepthStencilState : public IDepthStencilState {
		public:
			IContext *m_context;

			GLboolean m_zwriteenable;
			GLboolean m_zwritemask;
			GLenum m_zfunc;
			GLboolean m_stencilTestEnabled;
			GLuint m_stencilMask;
			GLenum m_stencilFrontFaceComp;
			GLenum m_stencilBackFaceComp;

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
		virtual svar GetFormatsCount() const = 0;
		virtual svar GetFormatsArray(Format *format, svar maxcount) const = 0;
		virtual bool SetFormat(const Format &format) const = 0;

		static GLenum GetPrimitive(Primitive primitive);

	protected:
		ObjPtr<Core::IFrameView> m_pView;

		bool CheckError(ctstring function, ctstring filename, svar line);
		ctstring GetMessageError(GLenum error);
	private:
		bool m_LibrariesLoaded;
		GLVersion m_Version;
		Color m_ClearColor;
		float m_ClearDepth;
		int m_ClearStencil;

		CSurface *m_Surface[0x100];
		ObjPtr<CDepthStencilState> m_DepthStencilState;
		ObjPtr<CGLBuffer> m_VertexBuffer, m_IndexBuffer;
		Xe::Math::Rectangle<int> m_Scissor;
		
		GLuint m_QuadIndexBuffer;
		GLint m_ShaderVertex, m_ShaderFragment, m_Program;
		GLint m_AttribPos, m_AttribTex, m_AttribCol;
		GLint m_AttribTex0, m_AttribTex1;

		bool m_IsSamplingEnabled;
		bool m_IsFboSupported;
		GLuint m_idFboSampler;

		void InitializeDefaultShaders();
		void DestroyDefaultShaders();
	};
} }
