#pragma once

#ifdef PLATFORM_GL
#include <gl/GL.h>

namespace Xe {
	namespace Graphics {
		enum GLVersion {
			GL_11,
			GL_12,
			GL_13,
			GL_14,
			GL_15,
			GL_20,
			GL_21,
			GL_30,
			GL_31,
			GL_32,
			GL_33,
		};
	}
}

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
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
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
typedef void(__stdcall * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void(__stdcall * PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void(__stdcall * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void(__stdcall * PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void(__stdcall * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
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
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2IPROC glUniform2i;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// OpenGL 3.0
#define GL_RGBA8UI 0x8D7C
typedef ptrdiff_t GLintptr;
typedef void*(__stdcall * PFNGLMAPBUFFERRANGEPROC) (GLenum target​, GLintptr offset​, GLsizeiptr length​, GLbitfield access​);
typedef void(__stdcall * PFNGLUNIFORM2UIPROC) (GLint location, GLuint v0, GLuint v1);
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
extern PFNGLUNIFORM2UIPROC glUniform2ui;

// OpenGL 3.1
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
typedef void(__stdcall * PFNGLTEXBUFFERPROC) (GLenum target​, GLenum internalFormat​, GLuint buffer);
extern PFNGLTEXBUFFERPROC glTexBuffer;

// OpenGL EXT
typedef GLboolean(__stdcall * PFNGLEXTSWAPINTERVAL) (int interval);
extern PFNGLEXTSWAPINTERVAL glExtSwapInterval;

#else

// GLES EXT
typedef void(*PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void(*PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
extern PFNGLMAPBUFFEROESPROC glMapBufferOES;
extern PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;

#endif // PLATFORM_GLES

#endif // GL_LOAD_FUNCTIONS

#endif // PLATFORM_GL