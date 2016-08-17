#include "pch.h"
#include <XeSDK/IGraphicsContext.h>
#include "XeGraphicsCommon.h"
#include "XeGraphicsGl.h"
#include <XeSDK/IGraphicsDrawing2d.h>

#ifdef PLATFORM_GL

#if defined(PLATFORM_WIN32)
#define GL_LOAD_FUNCTIONS
#define GETPROCADDRESS(name) wglGetProcAddress((LPCSTR)name)
#else
#define GETPROCADDRESS(name)
#endif


using namespace Xe::Graphics::Common;

namespace Xe {
	namespace Graphics {
		Gl::Gl() :
			m_LibrariesLoaded(false),
			m_clearColor(Color::Black),
			m_clearDepth(0.0f),
			m_clearStencil(0)
		{}
		bool Gl::LoadLibraries() {
			if (m_LibrariesLoaded)
				return true;
			m_LibrariesLoaded = true;
			bool r = false;
#ifdef PLATFORM_GLES
			// GLES 2.0
			glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
			glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
#else
			bool r2;
			// OpenGL 1.3
			r |= (glActiveTexture = (PFNGLACTIVETEXTUREPROC)GETPROCADDRESS("glActiveTexture")) == NULL;
			// OpenGL 1.5
			r |= (glBindBuffer = (PFNGLBINDBUFFERPROC)GETPROCADDRESS("glBindBuffer")) == NULL;
			r |= (glBufferData = (PFNGLBUFFERDATAPROC)GETPROCADDRESS("glBufferData")) == NULL;
			r |= (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GETPROCADDRESS("glDeleteBuffers")) == NULL;
			r |= (glGenBuffers = (PFNGLGENBUFFERSPROC)GETPROCADDRESS("glGenBuffers")) == NULL;
			r |= (glMapBuffer = (PFNGLMAPBUFFERPROC)GETPROCADDRESS("glMapBuffer")) == NULL;
			r |= (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)GETPROCADDRESS("glUnmapBuffer")) == NULL;
			// OpenGL 2.0
			r |= (glAttachShader = (PFNGLATTACHSHADERPROC)GETPROCADDRESS("glAttachShader")) == NULL;
			r |= (glCompileShader = (PFNGLCOMPILESHADERPROC)GETPROCADDRESS("glCompileShader")) == NULL;
			r |= (glCreateProgram = (PFNGLCREATEPROGRAMPROC)GETPROCADDRESS("glCreateProgram")) == NULL;
			r |= (glCreateShader = (PFNGLCREATESHADERPROC)GETPROCADDRESS("glCreateShader")) == NULL;
			r |= (glDeleteProgram = (PFNGLDELETEPROGRAMPROC)GETPROCADDRESS("glDeleteProgram")) == NULL;
			r |= (glDeleteShader = (PFNGLDELETESHADERPROC)GETPROCADDRESS("glDeleteShader")) == NULL;
			r |= (glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)GETPROCADDRESS("glDisableVertexAttribArray")) == NULL;
			r |= (glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GETPROCADDRESS("glEnableVertexAttribArray")) == NULL;
			r |= (glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)GETPROCADDRESS("glGetAttribLocation")) == NULL;
			r |= (glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)GETPROCADDRESS("glGetProgramInfoLog")) == NULL;
			r |= (glGetProgramiv = (PFNGLGETPROGRAMIVPROC)GETPROCADDRESS("glGetProgramiv")) == NULL;
			r |= (glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)GETPROCADDRESS("glGetShaderInfoLog")) == NULL;
			r |= (glGetShaderiv = (PFNGLGETSHADERIVPROC)GETPROCADDRESS("glGetShaderiv")) == NULL;
			r |= (glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)GETPROCADDRESS("glGetUniformLocation")) == NULL;
			r |= (glLinkProgram = (PFNGLLINKPROGRAMPROC)GETPROCADDRESS("glLinkProgram")) == NULL;
			r |= (glShaderSource = (PFNGLSHADERSOURCEPROC)GETPROCADDRESS("glShaderSource")) == NULL;
			r |= (glUniform1i = (PFNGLUNIFORM1IPROC)GETPROCADDRESS("glUniform1i")) == NULL;
			r |= (glUseProgram = (PFNGLUSEPROGRAMPROC)GETPROCADDRESS("glUseProgram")) == NULL;
			r |= (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GETPROCADDRESS("glVertexAttribPointer")) == NULL;
			// OpenGL EXT
			r2 = false;
			r2 |= (glExtSwapInterval = (PFNGLEXTSWAPINTERVAL)GETPROCADDRESS("wglSwapIntervalEXT")) == NULL;
			if (r2) r2 |= (glExtSwapInterval = (PFNGLEXTSWAPINTERVAL)GETPROCADDRESS("glXSwapIntervalEXT")) == NULL;
			r |= r2;
#endif

			return !r;
		}
		void Gl::GetAttribsFromProgram(GLuint program) {
			AttribPos = glGetAttribLocation(program, "a_position");
			AttribTex = glGetAttribLocation(program, "a_texture");
			AttribCol = glGetAttribLocation(program, "a_color");
			AttribTex0 = glGetUniformLocation(program, "tImage");
			AttribTex1 = glGetUniformLocation(program, "tClut");
			glUniform1i(AttribTex0, 0);
			glUniform1i(AttribTex1, 1);
		}
		void Gl::FlushDrawElements(svar quadsCount) {
			glVertexAttribPointer(AttribPos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
			glVertexAttribPointer(AttribTex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)8);
			glVertexAttribPointer(AttribCol, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)20);

			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glEnableVertexAttribArray(AttribPos);
			glEnableVertexAttribArray(AttribTex);
			glEnableVertexAttribArray(AttribCol);
			glDrawElements(GL_TRIANGLES, 6 * quadsCount, GL_UNSIGNED_SHORT, 0);
			glDisableVertexAttribArray(AttribCol);
			glDisableVertexAttribArray(AttribTex);
			glDisableVertexAttribArray(AttribPos);
			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
		}


		void Gl::SelectSurfaceId(GLuint id, svar index) {
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, id);
		}

		const Color &Gl::GetClearColor() const {
			return m_clearColor;
		}
		void Gl::SetClearColor(const Color &color) {
			m_clearColor = color;
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glClearColor(color.r, color.g, color.b, color.a);
		}
		float Gl::GetClearDepth() const {
			return m_clearDepth;
		}
		void Gl::SetClearDepthSub(float depth) {
			m_clearDepth = depth;
		}
		int Gl::GetClearStencil() const {
			return m_clearStencil;
		}
		void Gl::SetClearStencil(int stencil) {
			m_clearStencil = stencil;
			glClearStencil((GLint)stencil);
		}
		void Gl::Clear(svar clearmask) {
			GLbitfield mask = 0;
			if (clearmask & Clear_Color)
				mask |= GL_COLOR_BUFFER_BIT;
			if (clearmask & Clear_Depth)
				mask |= GL_DEPTH_BUFFER_BIT;
			if (clearmask & Clear_Stencil)
				mask |= GL_STENCIL_BUFFER_BIT;
			glClear(mask);
		}
		void Gl::DeleteShader(GLint id) {
			glDeleteShader(id);
		}
		void Gl::DeleteProgram(GLint id) {
			glDeleteShader(id);
		}

		///////////////////////////////////////////////////////////////////
		// PROPERTIES MANAGEMENT
		bool Gl::CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) {
			*depthStencilState = new DepthStencilState(this, desc);
			return true;
		}
		void Gl::SelectDepthStencilState(IDepthStencilState *depthStencilState) {
			DepthStencilState *state = (DepthStencilState*)depthStencilState;
			if (state->context == this) {
				if (m_DepthStencilState == depthStencilState)
					return;
				IDrawing2d *pDrawing;
				if (Query((IObject**)&pDrawing, IDrawing2d::ID)) {
					pDrawing->Flush();
					pDrawing->Release();
				}
				if (m_DepthStencilState != nullptr)
					m_DepthStencilState->Release();
				m_DepthStencilState = state;
				if (state) {
					if (state->m_zwriteenable)
					{
						glEnable(GL_DEPTH_TEST);
						glDepthMask(state->m_zwritemask);
						glDepthFunc(state->m_zfunc);
					}
					else
						glDisable(GL_DEPTH_TEST);

					if (0)
					{
						int ref = 0;
						glEnable(GL_STENCIL_TEST);
						//glStencilFuncSeparate(state->m_stencilFrontFaceComp, state->m_stencilBackFaceComp, ref, state->m_stencilMask);
					}
					else
						glDisable(GL_STENCIL_TEST);
					state->AddRef();
				}
			}
		}
		///////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////
		// CLASS PROPERTIES
		bool Gl::DepthStencilState::Query(IObject **obj, UID id) {
			switch (id) {
			case IDepthStencilState::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			case IContext::ID:
				context->AddRef();
				*obj = context;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		Gl::DepthStencilState::DepthStencilState(IContext *context, const DepthStencilStateDesc& desc) :
			context(context),
			m_zwriteenable((GLboolean)desc.depthTestEnabled),
			m_zwritemask((GLboolean)desc.depthWriteEnabled),
			m_zfunc(COMPARISON[desc.depthComparison]),
			m_stencilTestEnabled((GLboolean)desc.stencilTestEnabled),
			m_stencilMask((GLint)desc.stencilMask),
			m_stencilFrontFaceComp(COMPARISON[desc.stencilFrontFaceComp]),
			m_stencilBackFaceComp(COMPARISON[desc.stencilBackFaceComp]) {
			context->AddRef();
		}
		Gl::DepthStencilState::~DepthStencilState() {
			context->Release();
		}

		const GLenum Gl::COMPARISON[] =
		{
			GL_NEVER,
			GL_LESS,
			GL_EQUAL,
			GL_LEQUAL,
			GL_GREATER,
			GL_NOTEQUAL,
			GL_GEQUAL,
			GL_ALWAYS,
		};
		///////////////////////////////////////////////////////////////////
	}
}

#endif