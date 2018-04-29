#pragma once

#if defined(PLATFORM_GL)
#ifndef PLATFORM_GL
#define PLATFORM_GL
#endif
#include <gl/GL.h>
#elif defined(PLATFORM_GLES2)
#define PLATFORM_GL
#define PLATFORM_GLES
#include <EGL\egl.h>
#include <GLES\gl.h>
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#endif

#ifdef PLATFORM_GL

#define GL_LOAD_FUNCTIONS
#ifdef GL_LOAD_FUNCTIONS

#ifndef PLATFORM_GLES
// OpenGL 1.2
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

// OpenGL 1.3
#define GL_TEXTURE0 0x84C0
#define GL_MAX_TEXTURE_UNITS 0x84E2
typedef void(__stdcall * PFNGLACTIVETEXTUREPROC) (GLenum texture);
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

// OpenGL 1.5
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_WRITE_ONLY 0x88B9
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
typedef ptrdiff_t GLsizeiptr;
typedef void(__stdcall * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void(__stdcall * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void(__stdcall * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void(__stdcall * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void* (__stdcall * PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean(__stdcall * PFNGLUNMAPBUFFERPROC) (GLenum target);
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// OpenGL 2.0
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
typedef char GLchar;
typedef void(__stdcall * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void(__stdcall * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint(__stdcall * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint(__stdcall * PFNGLCREATESHADERPROC) (GLenum type);
typedef void(__stdcall * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void(__stdcall * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void(__stdcall * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void(__stdcall * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef GLint(__stdcall * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void(__stdcall * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void(__stdcall * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
typedef void(__stdcall * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void(__stdcall * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
typedef GLint(__stdcall * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void(__stdcall * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void(__stdcall * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length);
typedef void(__stdcall * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void(__stdcall * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void(__stdcall * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// OpenGL EXT
typedef GLboolean(__stdcall * PFNGLEXTSWAPINTERVAL) (int interval);
extern PFNGLEXTSWAPINTERVAL glExtSwapInterval;

#else

// GLES EXT
typedef void(*PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void(*PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
extern PFNGLMAPBUFFEROESPROC glMapBufferOES;
extern PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;

#endif

#endif

#if defined(PLATFORM_GL)
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeGraphicsColor.h>

namespace Xe {
	namespace Graphics {
		class Gl : public IContext {
		public:
			static const GLenum COMPARISON[];

			///////////////////////////////////////////////////////////////////
			// CLASS PROPERTIES
			class DepthStencilState : public IDepthStencilState {
			public:
				IContext *context;

				GLboolean m_zwriteenable;
				GLboolean m_zwritemask;
				GLenum m_zfunc;
				GLboolean m_stencilTestEnabled;
				GLuint m_stencilMask;
				GLenum m_stencilFrontFaceComp;
				GLenum m_stencilBackFaceComp;

				bool Query(IObject **obj, UID id);
				DepthStencilState(IContext *context, const DepthStencilStateDesc& desc);
				~DepthStencilState();
			};
			///////////////////////////////////////////////////////////////////

			GLuint AttribPos;
			GLuint AttribTex;
			GLuint AttribCol;
			GLuint AttribTex0;
			GLuint AttribTex1;

			Gl();
			bool LoadLibraries();
			void GetAttribsFromProgram(GLuint program);
			void FlushDrawElements(svar quadsCount);

			void SelectSurfaceId(GLuint, svar index);

			const Color &GetClearColor() const;
			void SetClearColor(const Color &color);
			float GetClearDepth() const;
			void SetClearDepthSub(float depth);
			int GetClearStencil() const;
			void SetClearStencil(int stencil);
			void Clear(svar clearmask);

			///////////////////////////////////////////////////////////////////
			// PROPERTIES MANAGEMENT
			bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc);
			void SelectDepthStencilState(IDepthStencilState *depthStencilState);
			///////////////////////////////////////////////////////////////////

			void DeleteShader(GLint id);
			void DeleteProgram(GLint id);

		private:
			bool m_LibrariesLoaded;
			Color m_clearColor;
			float m_clearDepth;
			int m_clearStencil;
			DepthStencilState *m_DepthStencilState;
		};
	}
}

#endif

#endif