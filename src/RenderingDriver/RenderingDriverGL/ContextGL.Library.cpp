#include "pch.h"
#include "ContextGL.Library.h"
#include "ContextGL.Common.h"

#if defined(PLATFORM_WIN32)
#define GL_LOAD_FUNCTIONS
#define GETPROCADDRESS(name) HeyGetProcAddress(name)
#define _GETPROCADDRESS(name) wglGetProcAddress((LPCSTR)name)
#else
#define GETPROCADDRESS(name)
#endif

#define LOAD(t, n) r |= (n = (t)GETPROCADDRESS(#n)) == NULL

#ifndef PLATFORM_GLES
// OpenGL 1.3
PFNGLACTIVETEXTUREPROC glActiveTexture;

// OpenGL 1.5
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLMAPBUFFERPROC glMapBuffer;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// OpenGL 2.0
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// OpenGL 3.0
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLUNIFORM2UIPROC glUniform2ui;
PFNGLGENVERTEXARRAYS glGenVertexArrays;
PFNGLBINDVERTEXARRAY glBindVertexArray;

// OpenGL 3.1
PFNGLTEXBUFFERPROC glTexBuffer;

// OpenGL 3.2
PFNWGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB;

// OpenGL EXT
PFNGLEXTSWAPINTERVAL glExtSwapInterval;
#else
// GLES EXT
PFNGLMAPBUFFEROESPROC glMapBufferOES;
PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;
#endif

namespace Xe {
	namespace Graphics {
		PROC WINAPI HeyGetProcAddress(const char *func) {
			PROC f = _GETPROCADDRESS(func);
			if (f == nullptr) LOGD("%s not found.", func);
			return f;
		}
		GLVersion CContextGLCommon::LoadLibraries() {
			if (m_LibrariesLoaded)
				return GetVersion();
			m_LibrariesLoaded = true;
			bool r = false;
#ifdef PLATFORM_GLES
			// GLES 2.0
			glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
			glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
#else
			GLVersion ver = GL_11;
			bool r2;

			// OpenGL 1.3
			r |= (glActiveTexture = (PFNGLACTIVETEXTUREPROC)GETPROCADDRESS("glActiveTexture")) == NULL;
			if (!r) ver = GL_13;

			// OpenGL 1.5
			r |= (glBindBuffer = (PFNGLBINDBUFFERPROC)GETPROCADDRESS("glBindBuffer")) == NULL;
			r |= (glBufferData = (PFNGLBUFFERDATAPROC)GETPROCADDRESS("glBufferData")) == NULL;
			r |= (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GETPROCADDRESS("glDeleteBuffers")) == NULL;
			r |= (glGenBuffers = (PFNGLGENBUFFERSPROC)GETPROCADDRESS("glGenBuffers")) == NULL;
			r |= (glMapBuffer = (PFNGLMAPBUFFERPROC)GETPROCADDRESS("glMapBuffer")) == NULL;
			r |= (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)GETPROCADDRESS("glUnmapBuffer")) == NULL;

			if (!r) ver = GL_15;
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
			r |= (glUniform1f = (PFNGLUNIFORM1FPROC)GETPROCADDRESS("glUniform1f")) == NULL;
			r |= (glUniform2f = (PFNGLUNIFORM2FPROC)GETPROCADDRESS("glUniform2f")) == NULL;
			r |= (glUniform1i = (PFNGLUNIFORM1IPROC)GETPROCADDRESS("glUniform1i")) == NULL;
			r |= (glUniform2i = (PFNGLUNIFORM2IPROC)GETPROCADDRESS("glUniform2i")) == NULL;
			r |= (glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)GETPROCADDRESS("glUniformMatrix4fv")) == NULL;
			r |= (glUseProgram = (PFNGLUSEPROGRAMPROC)GETPROCADDRESS("glUseProgram")) == NULL;
			r |= (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GETPROCADDRESS("glVertexAttribPointer")) == NULL;
			if (!r) ver = GL_20;

			// OpenGL 3.0
			LOAD(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange);
			LOAD(PFNGLUNIFORM2UIPROC, glUniform2ui);
			LOAD(PFNGLGENVERTEXARRAYS, glGenVertexArrays);
			LOAD(PFNGLBINDVERTEXARRAY, glBindVertexArray);
			if (!r) ver = GL_30;

			// OpenGL 3.1
			LOAD(PFNGLTEXBUFFERPROC, glTexBuffer);
			if (!r) ver = GL_31;

			// OpenGL 3.2
			LOAD(PFNWGLCREATECONTEXTATTRIBSARB, wglCreateContextAttribsARB);
			if (!r) ver = GL_32;

			// OpenGL EXT
			r2 = false;
			r2 |= (glExtSwapInterval = (PFNGLEXTSWAPINTERVAL)GETPROCADDRESS("wglSwapIntervalEXT")) == NULL;
			if (r2) r2 |= (glExtSwapInterval = (PFNGLEXTSWAPINTERVAL)GETPROCADDRESS("glXSwapIntervalEXT")) == NULL;
			r |= r2;
#endif

			return m_Version = ver;
		}
	}
}
