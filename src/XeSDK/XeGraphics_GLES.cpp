#include "pch.h"
#include <XeSDK/IGraphicsContext.h>
#include "XeGraphicsCommon.h"
#if defined(PLATFORM_GLES2)
#include "ContextGL.Library.h"
#include <cstdlib>
#include "ContextGL.Common.h"
#include "IGraphicsWindowsWin32.h"
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/XeMemory.h>

#define ENDL "\n"
#define BUFFER_OFFSET(i) ((void*)(i))

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
		static const char ContextGLES_Drawing_ShaderVertex[] =
		{
			"#version 120\n"
			/*ENDL"uniform mat4 mCamera;"
			ENDL"uniform mat4 mPosition;"
			ENDL"varying vec3 vTexture;"
			ENDL"varying vec4 vColor;"
			ENDL
			ENDL"void main()"
			ENDL"{"
			ENDL"gl_Position = gl_Vertex * mPosition * mCamera;"
			ENDL"gl_TexCoord[0]  = gl_MultiTexCoord0;"
			ENDL"vTexture = gl_TexCoord[0].xyz;"
			ENDL"vColor = gl_Color;"
			ENDL"}"*/

			ENDL"attribute vec4 a_position;"
			ENDL"attribute vec3 a_texture;"
			ENDL"attribute vec4 a_color;"
			ENDL"varying vec3 vTexture;"
			ENDL"varying vec4 vColor;"
			ENDL
			ENDL"void main()"
			ENDL"{"
			ENDL"gl_Position = a_position;"
			ENDL"gl_Position.z = 0.5f;"
			ENDL"vTexture = a_texture;"
			ENDL"vColor = a_color;"
			ENDL"}"
		};
		static const char ContextGLES_Drawing_ShaderFragment[] =
		{
			"#version 120\n"
			ENDL"uniform sampler2D tImage;"
			ENDL"uniform sampler2D tClut;"
			ENDL"varying vec3 vTexture;"
			ENDL"varying vec4 vColor;"
			ENDL
			ENDL"void main()"
			ENDL"{"
			ENDL"vec4 color = vColor;"
			ENDL"if (vTexture.z < 0.5)"
			ENDL"{"
			ENDL"float colorIndex =  texture2D(tImage, vTexture.xy).a;"
			ENDL"color *= texture2D(tClut, vec2(colorIndex, vTexture.z * 2.0));"
			ENDL"}"
			ENDL"else if (vTexture.z < 1.0)"
			ENDL"{"
			ENDL"color *= texture2D(tImage, vTexture.xy);"
			ENDL"}"
			ENDL"gl_FragColor = color;"
			ENDL"}"
		};
		bool GetNativeWindowType(EGLNativeWindowType *window, const Size& size, s32 format);

		class ContextGLES : public Gl {
			struct FormatFallback
			{
				Color::Format format;
				GLint internalFormat;
				GLint glFormat;
				GLint type;
			};
			static const FormatFallback FORMAT_FALLBACK[];

			class Drawing : public IDrawing2d
			{
				// Default vertex model used from GL21 drawing engine
				struct Vertex {
					float x, y;
					float u, v, p;
					Color color;
				};

				static const svar MaximumQuadsCount = 16383;
				ContextGLES *m_pContext;
				svar m_curQuadsCount;
				Vertex *m_pVertex;

				GLint m_shader[2];
				GLint m_program;
				GLuint m_bufferObj[2];

				ISurface *m_pSrcSurface;
				ISurface *m_pDstSurface;

				inline Vertex *Get() {
					if (m_curQuadsCount < MaximumQuadsCount)
						return m_pVertex + m_curQuadsCount++ * 4;
					Flush();
					m_curQuadsCount = 0;
					return m_pVertex;
				}

				bool GetSourceSurface(ISurface **surface);
				void SetSourceSurface(ISurface *surface);
				bool GetDestinationSurface(ISurface **surface);
				void SetDestinationSurface(ISurface *surface);
				void Flush();
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);

			public:
				Drawing(ContextGLES *pContext);
				~Drawing();
			};
			class GLSurface : public ISurface {
				GLuint m_id;
			public:
				GLSurface(GLuint id, Type type, const Size &size, Color::Format format) :
					ISurface(type, size, format), m_id(id) { }
				~GLSurface() {
					glDeleteTextures(1, &m_id);
				}
				GLuint GetId() const { return m_id; }
			};

			static const svar MaximumSelectedSurfaces = 0x100;

			Drawing *m_pDrawing;

			EGLDisplay m_pDisplay;
			EGLContext m_pContext;
			EGLSurface m_pSurface;

			Capabilities m_capabilities;
			GLuint m_selectedSurfaces[MaximumSelectedSurfaces];

			VBlankSync m_sync;
			Size m_size;

			bool LoadWindow(Size& size);
			void Uninitialize();
			static bool CheckError(svar line);
			static ctstring GetMessageError(GLenum error);
		public:
			ContextGLES();
			~ContextGLES();
			bool Initialize();
			void GetDrawing(IDrawing2d **drawing);
			void GetInput(IO::IInput **input);
			void SetCallback(ICallback *callback);

			void SetTitle(ctstring title);
			const Size &GetSize() const;
			void SetSize(const Size &size);

			bool CreateSurface(ISurface **surface, ISurface::Type type, const Size &size, Color::Format format, const DataDesc& dataDesc);
			void SelectSurface(ISurface *surface, svar index);

			void SetClearDepth(float depth);
			void SwapBuffers(VBlankSync sync);
			bool Process();

			bool CompileShader(const char* source, const int length, const unsigned int type, GLint& id);
			bool BuildProgram(GLint* shaders, unsigned int count, GLint& id);
			void ShaderError(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
		};

		bool ContextGLES::CheckError(svar line)
		{
			// glGetError is slow and it should be removed from Release builds
#ifdef _DEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
			{
				LOGE("GLerr %s", GetMessageError(err));
			}
#endif
			return true;
		}
		ctstring ContextGLES::GetMessageError(GLenum error)
		{
			switch (error)
			{
			case GL_NO_ERROR:			return _T("GL_NO_ERROR");
			case GL_INVALID_ENUM:		return _T("GL_INVALID_ENUM");
			case GL_INVALID_VALUE:		return _T("GL_INVALID_VALUE");
			case GL_INVALID_OPERATION:	return _T("GL_INVALID_OPERATION");
			case GL_STACK_OVERFLOW:		return _T("GL_STACK_OVERFLOW");
			case GL_STACK_UNDERFLOW:	return _T("GL_STACK_UNDERFLOW");
			case GL_OUT_OF_MEMORY:		return _T("GL_OUT_OF_MEMORY");
			}
			return _T("UNKNOWN ERROR");
		}

		bool ContextGLES::LoadWindow(Size& size) {
			static const EGLint attribs[] = {
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 4,
				EGL_GREEN_SIZE, 4,
				EGL_RED_SIZE, 4,
				EGL_NONE
			};

			if (size.x < 0 || size.y < 0) {
				LOGE("Invalid size (%i, %i)", size.x, size.y);
				return false;
			}

			EGLint maj, min;
			EGLint numConfigs;
			EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (display == EGL_NO_DISPLAY) {
				LOGF("EGL_NO_DISPLAY");
				return false;
			}
			if (eglInitialize(display, &maj, &min) == EGL_FALSE) {
				LOGE("eglInitialize failed");
				return false;
			}

			EGLConfig *pConfigs;
			eglChooseConfig(display, attribs, NULL, 0, &numConfigs);
			pConfigs = new EGLConfig[numConfigs];
			eglChooseConfig(display, attribs, pConfigs, numConfigs, &numConfigs);

			LOGD("egl found %i different configurations", numConfigs);

			struct EGLConfiguration {
				EGLint id;
				EGLint size;
				EGLint level;
				EGLint version;
				EGLint conformant;
				EGLint renderable;
				EGLint format;
				EGLint surfaces;
				EGLint r, g, b, a;
				EGLint depth, stencil;
				EGLint samples, sampleBuffers;

				static int Compare(const void *x, const void *y) {
					EGLint diff;
					EGLConfiguration *b = (EGLConfiguration*)x;
					EGLConfiguration *a = (EGLConfiguration*)y;
					if ((diff = a->conformant - b->conformant) != 0) return diff;
					if ((diff = a->renderable - b->renderable) != 0) return diff;
					if ((diff = a->g - b->g) != 0) return diff;
					if ((diff = a->a - b->a) != 0) return diff;
					if ((diff = a->depth - b->depth) != 0) return diff;
					if ((diff = a->stencil - b->stencil) != 0) return diff;
					if ((diff = a->samples - b->samples) != 0) return diff;
					if ((diff = a->sampleBuffers - b->sampleBuffers) != 0) return diff;
					if ((diff = a->r - b->r) != 0) return diff;
					if ((diff = a->b - b->b) != 0) return diff;
					if ((diff = a->size - b->size) != 0) return diff;
					return 0;
				}
			};

			EGLConfiguration *pConfigs2 = new EGLConfiguration[numConfigs];
			for (EGLint i = 0; i < numConfigs; i++) {
				/* EGL_NATIVE_VISUAL_ID è un attributo dell'oggetto EGLConfig che viene
				* sicuramente accettato da ANativeWindow_setBuffersGeometry().
				* Non appena si seleziona un oggetto EGLConfig, si può riconfigurare in tutta sicurezza
				* i buffer ANativeWindow da abbinare usando EGL_NATIVE_VISUAL_ID. */
				EGLConfig curConfig = pConfigs[i];
				EGLConfiguration *curConfig2 = &pConfigs2[i];
				eglGetConfigAttrib(display, curConfig, EGL_CONFIG_ID, &curConfig2->id);
				eglGetConfigAttrib(display, curConfig, EGL_BUFFER_SIZE, &curConfig2->size);
				eglGetConfigAttrib(display, curConfig, EGL_LEVEL, &curConfig2->level);
				eglGetConfigAttrib(display, curConfig, EGL_CONTEXT_CLIENT_VERSION, &curConfig2->version);
				eglGetConfigAttrib(display, curConfig, EGL_CONFORMANT, &curConfig2->conformant);
				eglGetConfigAttrib(display, curConfig, EGL_RENDERABLE_TYPE, &curConfig2->renderable);
				eglGetConfigAttrib(display, curConfig, EGL_NATIVE_VISUAL_ID, &curConfig2->format);
				eglGetConfigAttrib(display, curConfig, EGL_SURFACE_TYPE, &curConfig2->surfaces);
				eglGetConfigAttrib(display, curConfig, EGL_RED_SIZE, &curConfig2->r);
				eglGetConfigAttrib(display, curConfig, EGL_GREEN_SIZE, &curConfig2->g);
				eglGetConfigAttrib(display, curConfig, EGL_BLUE_SIZE, &curConfig2->b);
				eglGetConfigAttrib(display, curConfig, EGL_ALPHA_SIZE, &curConfig2->a);
				eglGetConfigAttrib(display, curConfig, EGL_DEPTH_SIZE, &curConfig2->depth);
				eglGetConfigAttrib(display, curConfig, EGL_STENCIL_SIZE, &curConfig2->stencil);
				eglGetConfigAttrib(display, curConfig, EGL_SAMPLES, &curConfig2->samples);
				eglGetConfigAttrib(display, curConfig, EGL_SAMPLE_BUFFERS, &curConfig2->sampleBuffers);
			}
			// CONFIGURATION SORT
			qsort(pConfigs2, numConfigs, sizeof(EGLConfiguration), EGLConfiguration::Compare);

			bool success = false;
			for (EGLint i = 0; i < numConfigs; i++) {
				EGLNativeWindowType pWindow = 0;
				if (GetNativeWindowType(&pWindow, size, pConfigs2[i].format)) {
					m_pSurface = eglCreateWindowSurface(display, pConfigs[i], pWindow, NULL);
					if (m_pSurface == NULL) {
						LOGE("eglInitialize failed");
						return false;
					}
					static const EGLint contextAttribs[] = {
						// request a context using Open GL ES 2.0
						EGL_CONTEXT_CLIENT_VERSION, 2,
						EGL_NONE
					};
					m_pContext = eglCreateContext(display, pConfigs[i], NULL, contextAttribs);
					if (m_pContext == NULL) {
						LOGE("eglCreateContext failed");
						return false;
					}
					if (eglMakeCurrent(display, m_pSurface, m_pSurface, m_pContext) != EGL_FALSE) {
						LOGD("GL Version: %s", (const char*)glGetString(GL_VERSION));
						LOGD("GL Vendor: %s", (const char*)glGetString(GL_VENDOR));
						LOGD("GL Renderer: %s", (const char*)glGetString(GL_RENDERER));
						LOGD("GL Extensions: %s", (const char*)glGetString(GL_EXTENSIONS));
						EGLint w, h;
						eglQuerySurface(display, m_pSurface, EGL_WIDTH, &w);
						eglQuerySurface(display, m_pSurface, EGL_HEIGHT, &h);
						LOGD("Detected GL size %ix%i", w, h);
						size.x = w;
						size.y = h;
						m_pDisplay = display;
						success = true;
						break;
					}
					else
						LOGE("eglMakeCurrent failed"));
				}

			}
			delete[] pConfigs2;
			delete[] pConfigs;
			return success;
		}
		void ContextGLES::Uninitialize() {
			if (m_pDisplay != EGL_NO_DISPLAY) {
				eglMakeCurrent(m_pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
				if (m_pContext != EGL_NO_CONTEXT) {
					eglDestroyContext(m_pDisplay, m_pContext);
				}
				if (m_pSurface != EGL_NO_SURFACE) {
					eglDestroySurface(m_pDisplay, m_pSurface);
				}
				eglTerminate(m_pDisplay);
			}
		}
		bool ContextGLES::Initialize() {
			Size size;
			bool r = LoadLibraries();
			r &= LoadWindow(size);
			if (r) {
				glGetIntegerv(GL_MAX_TEXTURE_UNITS, &m_capabilities.MaxTextureUnits);
				glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_capabilities.MaxTextureSize);
				m_pDrawing = new Drawing(this);
				return true;
			}
			return false;
		}

		ContextGLES::ContextGLES() :
			m_pDrawing(nullptr),
			m_pDisplay(EGL_NO_DISPLAY),
			m_pContext(EGL_NO_CONTEXT),
			m_pSurface(EGL_NO_SURFACE) {
			Xe::Memory::Fill(m_selectedSurfaces, 0, sizeof(m_selectedSurfaces));
		}
		ContextGLES::~ContextGLES() {
			if (m_pDrawing)
				m_pDrawing->Release();
			Uninitialize();
		}
		void ContextGLES::GetDrawing(IDrawing2d **drawing) {
			m_pDrawing->AddRef();
			*drawing = m_pDrawing;
		}
		void ContextGLES::GetInput(IO::IInput **input) {
		}
		void ContextGLES::SetCallback(ICallback *callback) {

		}

		void ContextGLES::SetTitle(ctstring title) {
		}
		const Size &ContextGLES::GetSize() const {
			return m_size;
		}
		void ContextGLES::SetSize(const Size &size) {
		}

		bool ContextGLES::CreateSurface(ISurface **surface, ISurface::Type type, const Size &size, Color::Format format, const DataDesc& dataDesc) {
			static const GLenum texType = GL_TEXTURE_2D;
			GLuint glId;
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &glId);
			glBindTexture(texType, glId);
			glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

			const FormatFallback& glFormat = FORMAT_FALLBACK[format];
			glTexImage2D(texType, 0, glFormat.internalFormat, (GLsizei)size.x, (GLsizei)size.y,
				0, glFormat.glFormat, glFormat.type, dataDesc.data);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(texType, m_selectedSurfaces[0]);
			if (!CheckError(__LINE__)) {
				*surface = nullptr;
				return false;
			}
			*surface = new GLSurface(glId, type, size, format);
			return true;
		}
		void ContextGLES::SelectSurface(ISurface *surface, svar index) {
			if (index < 0 || index >= MaximumSelectedSurfaces) {
				LOGE("SelectSurface(,%i) index not valid", index);
				return;
			}
			if (index >= m_capabilities.MaxTextureUnits) {
				LOGE("SelectSurface(,%i) index exceed GetCapabilities().MaxTextureUnits", index);
				return;
			}
			// XTODO check if it's a ContextGLES surface
			GLuint id = ((GLSurface*)surface)->GetId();
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, m_selectedSurfaces[index] = id);
		}

		void ContextGLES::SetClearDepth(float depth) {
			SetClearDepthSub(depth);
			glDepthMask(GL_TRUE);
			glClearDepthf((GLclampf)depth);
		}

		void ContextGLES::SwapBuffers(VBlankSync sync) {
			if (m_sync != sync) {
				m_sync = sync;
				eglSwapInterval(m_pDisplay, sync);
			}
			eglSwapBuffers(m_pDisplay, m_pSurface);
		}
		bool ContextGLES::Process() {
			return true;
		}

		bool ContextGLES::CompileShader(const char* source, const int length, const unsigned int type, GLint& id) {
			GLenum shaderType;
			switch (type)
			{
			case 0:
				shaderType = GL_VERTEX_SHADER;
				break;
			case 1:
				shaderType = GL_FRAGMENT_SHADER;
				break;
			default:
				return false;
			}
			id = glCreateShader(shaderType);
			if (id == 0) return false;

			GLint shader_ok = GL_FALSE;
			glShaderSource(id, 1, (const GLchar**)&source, &length);
			glCompileShader(id);
			glGetShaderiv(id, GL_COMPILE_STATUS, &shader_ok);
			if (shader_ok == GL_FALSE)
			{
				ShaderError(id, (PFNGLGETSHADERIVPROC)glGetShaderiv, (PFNGLGETSHADERINFOLOGPROC)glGetShaderInfoLog);
				glDeleteShader(id);
				return false;
			}
			return true;
		}
		bool ContextGLES::BuildProgram(GLint* shaders, unsigned int count, GLint& id) {
			GLint program_ok;

			id = glCreateProgram();
			for (unsigned int i = 0; i < count; i++)
			{
				glAttachShader(id, shaders[(ptrdiff_t)i]);
			}
			glLinkProgram(id);

			glGetProgramiv(id, GL_LINK_STATUS, &program_ok);
			if (program_ok == GL_FALSE)
			{
				//ShaderError((GLuint)id, glGetProgramiv, glGetProgramInfoLog);
				glDeleteProgram(id);
				return false;
			}
			return true;
		}
		void ContextGLES::ShaderError(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog) {
			GLint log_length;
			GLchar* outlog;

			glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
			outlog = new char[log_length];
			glGet__InfoLog(object, (GLsizei)log_length, NULL, outlog);
			// insert debug log here
#if defined(PLATFORM_WIN32)
			OutputDebugStringA(outlog);
#endif
		}


		ContextGLES::Drawing::Drawing(ContextGLES *pContext) :
			m_pContext(pContext),
			m_curQuadsCount(0),
			m_pVertex(new Vertex[MaximumQuadsCount * 4]),
			m_pSrcSurface(nullptr),
			m_pDstSurface(nullptr)
		{
			if (m_pContext->CompileShader(ContextGLES_Drawing_ShaderVertex, sizeof(ContextGLES_Drawing_ShaderVertex), 0, m_shader[0]) == false ||
				m_pContext->CompileShader(ContextGLES_Drawing_ShaderFragment, sizeof(ContextGLES_Drawing_ShaderFragment), 1, m_shader[1]) == false)
				return;
			if (m_pContext->BuildProgram(m_shader, 2, m_program) == false)
				return;
			glUseProgram(m_program);
			m_pContext->GetAttribsFromProgram(m_program);

			unsigned short *indices = new unsigned short[MaximumQuadsCount * 6];
			unsigned int *pIndices = (unsigned int*)indices;
			for (int i = 0, nx = 0x00000000; i < MaximumQuadsCount * 3; i += 3)
			{
				pIndices[i + 0] = nx + 0x00010000;
				pIndices[i + 1] = nx + 0x00010002;
				pIndices[i + 2] = nx + 0x00030002;
				nx += 0x00040004;
			}
			glGenBuffers(2, m_bufferObj);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObj[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaximumQuadsCount * 6 * sizeof(u16), indices, GL_STATIC_DRAW);
			delete[] indices;
		}
		ContextGLES::Drawing::~Drawing() {
			Flush();

			if (m_pSrcSurface)
				m_pSrcSurface->Release();
			if (m_pDstSurface)
				m_pDstSurface->Release();
			glDeleteBuffers(2, m_bufferObj);
			glDeleteProgram(m_program);
			glDeleteShader(m_shader[0]);
			glDeleteShader(m_shader[1]);
			delete[] m_pVertex;
		}

		bool ContextGLES::Drawing::GetSourceSurface(ISurface **surface) {
			*surface = m_pSrcSurface;
			if (m_pSrcSurface) {
				m_pSrcSurface->AddRef();
				return true;
			}
			return false;
		}
		void ContextGLES::Drawing::SetSourceSurface(ISurface *surface) {
			if (m_pSrcSurface != surface) {
				if (m_pSrcSurface)
					m_pSrcSurface->Release();
				if (surface)
					surface->AddRef();
				m_pSrcSurface = surface;
			}
		}
		bool ContextGLES::Drawing::GetDestinationSurface(ISurface **surface) {
			*surface = m_pDstSurface;
			if (m_pDstSurface) {
				m_pDstSurface->AddRef();
				return true;
			}
			return false;
		}
		void ContextGLES::Drawing::SetDestinationSurface(ISurface *surface) {
			if (m_pDstSurface != surface) {
				if (m_pDstSurface)
					m_pDstSurface->Release();
				if (surface)
					surface->AddRef();
				m_pDstSurface = surface;
			}
		}
		void ContextGLES::Drawing::Flush() {
			if (m_curQuadsCount < 0)
				return;
			glUseProgram(m_program);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObj[1]);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferObj[0]);

			size_t buflen = sizeof(Vertex) * 4 * m_curQuadsCount;
			if (glMapBufferOES != NULL) {
				glBufferData(GL_ARRAY_BUFFER, buflen, 0, GL_STREAM_DRAW);
				void *data = glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
				memcpy(data, m_pVertex, buflen);
				glUnmapBufferOES(GL_ARRAY_BUFFER);
			}
			else {
				void *data = malloc(buflen);
				memcpy(data, m_pVertex, buflen);
				glBufferData(GL_ARRAY_BUFFER, buflen, data, GL_STATIC_DRAW);
			}

			m_pContext->FlushDrawElements(m_curQuadsCount);
			m_curQuadsCount = 0;
		}

		void ContextGLES::Drawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color &color) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}
		void ContextGLES::Drawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}
		void ContextGLES::Drawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color &color) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}
		void ContextGLES::Drawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}

		void ContextGLES::Drawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void ContextGLES::Drawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void ContextGLES::Drawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void ContextGLES::Drawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}

		const ContextGLES::FormatFallback ContextGLES::FORMAT_FALLBACK[] =
		{
			{ Color::Format_Unknown, 0, 0, 0 },
			{ Color::Format_BGRA8888, GL_RGBA, GL_EXT_texture_format_BGRA8888, GL_UNSIGNED_BYTE },
			{ Color::Format_RGBA8888, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
			{ Color::Format_BGRX8888, GL_RGB, GL_EXT_texture_format_BGRA8888, GL_UNSIGNED_BYTE },
			{ Color::Format_RGBX8888, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE },
			{ Color::Format_BGR888, 0, 0, 0 },
			{ Color::Format_RGB888, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },
			{ Color::Format_BGR565, 0, 0, 0 },
			{ Color::Format_RGB565, GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5 },
			{ Color::Format_BGRA5551, 0, 0, 0 },
			{ Color::Format_ARGB1555, 0, 0, 0 },
			{ Color::Format_BGRX5551, 0, 0, 0 },
			{ Color::Format_XRGB1555, 0, 0, 0 },
			{ Color::Format_BGRA4444, 0, GL_EXT_texture_format_BGRA8888, 0 },
			{ Color::Format_ARGB4444, 0, GL_EXT_texture_format_BGRA8888, 0 },
			{ Color::Format_BGRX4444, 0, 0, 0 },
			{ Color::Format_XRGB4444, 0, 0, 0 },
			{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
			{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
			{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
			{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
			{ Color::Format_Indexed16, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_SHORT },
			{ Color::Format_Indexed32, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_INT },
		};
		bool CreateContextGLES(IContext **context, const ContextInitDesc& properties) {
			ContextGLES *pContext = new ContextGLES;
			if (pContext->Initialize()) {
				*context = pContext;
				return true;
			}
			*context = nullptr;
			return false;
		}
	}
}
#else
namespace Xe {
	namespace Graphics {
		bool CreateContextGLES(IContext **context, const ContextInitDesc& properties) {
			*context = nullptr;
			return false;
		}
	}
}
#endif