#include "pch.h"
#include "ContextGL.Common.h"
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeLogger.h>
#include <XeSDK/ICoreView.h>

#define ENDL "\n"

using namespace Xe::Graphics;

CContextGLCommon::CContextGLCommon(Xe::Core::IFrameView *pView) :
	m_pView(pView),
	m_LibrariesLoaded(false),
	m_ClearColor(Color::Black),
	m_ClearDepth(0.0f),
	m_ClearStencil(0),
	m_DepthStencilState(nullptr),
	m_QuadIndexBuffer(0),
	m_IsSamplingEnabled(false),
	m_IsFboSupported(false)
{
	Memory::Fill(m_Surface, 0, sizeof(m_Surface));
}

CContextGLCommon::~CContextGLCommon() {
	DestroyDefaultShaders();
	if (m_QuadIndexBuffer) glDeleteBuffers(1, &m_QuadIndexBuffer);
	for (svar i = 0; i < lengthof(m_Surface); i++)
		if (m_Surface[i]) m_Surface[i]->Release();
	if (m_DepthStencilState) m_DepthStencilState->Release();
}

bool CContextGLCommon::Initialize(const ContextInitDesc& properties) {
	// Enumerate formats
	svar fCount = GetFormatsCount();
	Format *pFormat = (Format*)Memory::Alloc(sizeof(Format) * fCount);
	GetFormatsArray(pFormat, fCount);
	Memory::Free(pFormat);
	LOGI("%i GL configuration found", fCount);

	// Display internal graphics information
	LOGI("Vendor: %s", glGetString(GL_VENDOR));
	LOGI("Renderer: %s", glGetString(GL_RENDERER));
	LOGI("Version: %s", glGetString(GL_VERSION));
	LOGI("Shader version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Hey you, developer! Here's a warning message 4 ya:
	// Be sure to not pass this point if a valid GL context is not created yet.
	LOGD("Loading GL libraries...");
	m_Version = LoadLibraries();

	static const char *GLVER[] =
	{
		"1.1", "1.2", "1.3", "1.4", "1.5",
		"2.0", "2.1",
		"3.0", "3.1", "3.2", "3.3",
		"4.0", "4.1", "4.2", "4.3", "4.4", "4.5"
	};
			
	static const GLVersion MINGLVER = GL_31;

	if (m_Version >= MINGLVER)
	{
		LOGD("Using internal version %s.", GLVER[m_Version]);
		// Get capabilities
		//glGetIntegerv(GL_MAX_TEXTURE_UNITS, &m_capabilities.MaxTextureUnits);
		//glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_capabilities.MaxTextureSize);

		// Initialize common parameters
		glDisable(GL_DITHER);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		//glStencilMask(GL_TRUE);

		InitializeDefaultShaders();
		return true;
	}
	else
	{
		LOGE("Unable to initialize OpenGL. Version %s was found but %s is the minimum required.",
			GLVER[m_Version], GLVER[MINGLVER]);
	}
	return false;
}

GLVersion CContextGLCommon::GetVersion() const
{
	return m_Version;
}

void CContextGLCommon::InitializeDefaultShaders()
{
	static const char VertexShader[] =
	{
		"#version 110\n"
		ENDL"attribute vec4 a_position;"
		ENDL"attribute vec3 a_texture;"
		ENDL"attribute vec4 a_color;"
		ENDL"varying vec3 vTexture;"
		ENDL"varying vec4 vColor;"
		ENDL
		ENDL"void main()"
		ENDL"{"
		ENDL"gl_Position = a_position;"
		ENDL"vTexture = a_texture;"
		ENDL"vColor = a_color;"
		ENDL"}"
	};

	static const char FragmentShader[] =
	{
		"#version 110\n"
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

	if (CompileShader(VertexShader, sizeof(VertexShader), 0, m_ShaderVertex) == false ||
		CompileShader(FragmentShader, sizeof(FragmentShader), 1, m_ShaderFragment) == false)
	{
		LOGE("Unable to compile the default shaders");
		return;
	}

	GLint shaders[]{ m_ShaderVertex, m_ShaderFragment };
	if (BuildProgram(shaders, 2, m_Program))
	{
		LOGD("Default shader program compiled");
		glUseProgram(m_Program);
	}
	else
	{
		LOGE("Unable to build the default shader program");
	}

	m_AttribPos = glGetAttribLocation(m_Program, "a_position");
	m_AttribTex = glGetAttribLocation(m_Program, "a_texture");
	m_AttribCol = glGetAttribLocation(m_Program, "a_color");
	m_AttribTex0 = glGetUniformLocation(m_Program, "tImage");
	m_AttribTex1 = glGetUniformLocation(m_Program, "tClut");
	glUniform1i(m_AttribTex0, 0);
	glUniform1i(m_AttribTex1, 1);

	GLuint idVertexArrayObject;
	glGenVertexArrays(1, &idVertexArrayObject);
	glBindVertexArray(idVertexArrayObject);

	// Use the default Vertex format
	glVertexAttribPointer(m_AttribPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(m_AttribTex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)12);
	glVertexAttribPointer(m_AttribCol, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)24);

	glEnableVertexAttribArray(m_AttribPos);
	glEnableVertexAttribArray(m_AttribTex);
	glEnableVertexAttribArray(m_AttribCol);
}

void CContextGLCommon::DestroyDefaultShaders()
{
	glDisableVertexAttribArray(m_AttribCol);
	glDisableVertexAttribArray(m_AttribTex);
	glDisableVertexAttribArray(m_AttribPos);

	glDeleteProgram(m_Program);
	glDeleteShader(m_ShaderVertex);
	glDeleteShader(m_ShaderFragment);
}

///////////////////////////////////////////////////////////////////
// Common stuff
void CContextGLCommon::SetInternalResolution(const Size& resolution) {
	// if specified resolution is different from window size, then use
	// down-sampling or super-sampling.
	if (m_pView->GetSize() != resolution) {
		if (m_IsFboSupported) {

		}
		else {
			// This works between OpenGL 1.1 and 2.1 and it's useful for
			// sampling and works without FBO but it's slower.
			if (m_IsSamplingEnabled == false) {
				// Sampling was disabled, so we need to create resources
				/*GLuint idPrevSurface;
				if (m_Surface[0]) idPrevSurface = m_Surface[0]->m_GlId;
				else idPrevSurface = 0;
				glGenTextures(1, &m_idFboSampler);
				glBindTexture(GL_TEXTURE_2D, m_idFboSampler);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, glFormat.internalFormat, (GLsizei)size.x, (GLsizei)size.y,
					0, glFormat.glFormat, glFormat.type, dataDesc.data);

				glGenTextures(1, &idPrevSurface);*/
			}
		}
		m_IsSamplingEnabled = true;
	}
	else {
		if (m_IsSamplingEnabled) {
			// Sampling was enabled, so we need to delete resources
		}
		m_IsSamplingEnabled = false;
	}
	glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);

}
const Color &CContextGLCommon::GetClearColor() const {
	return m_ClearColor;
}
void CContextGLCommon::SetClearColor(const Color &color) {
	m_ClearColor = color;
	glClearColor(color.r, color.g, color.b, color.a);
}
float CContextGLCommon::GetClearDepth() const {
	return m_ClearDepth;
}
void CContextGLCommon::SetClearDepthSub(float depth) {
	m_ClearDepth = depth;
}
int CContextGLCommon::GetClearStencil() const {
	return m_ClearStencil;
}
void CContextGLCommon::SetClearStencil(int stencil) {
	m_ClearStencil = stencil;
	glClearStencil((GLint)stencil);
}


bool CContextGLCommon::IsScissorEnabled() const
{
	return glIsEnabled(GL_SCISSOR_TEST);
}

void CContextGLCommon::SetScissorEnabled(bool enabled)
{
	enabled ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}

const Xe::Math::Rectangle<int> CContextGLCommon::GetScissor() const
{
	return m_Scissor;
}

void CContextGLCommon::SetScissor(const Xe::Math::Rectangle<int>& scissor)
{
	glScissor(scissor.left, scissor.top, scissor.GetWidth(), scissor.GetHeight());
}

void CContextGLCommon::Clear(svar clearmask) {
	GLbitfield mask = 0;
	if (clearmask & Clear_Color)
		mask |= GL_COLOR_BUFFER_BIT;
	if (clearmask & Clear_Depth)
		mask |= GL_DEPTH_BUFFER_BIT;
	if (clearmask & Clear_Stencil)
		mask |= GL_STENCIL_BUFFER_BIT;
	glClear(mask);
}

void CContextGLCommon::Draw(Primitive primitive, u32 count, u32 start)
{
	glDrawArrays(GetPrimitive(primitive), (GLint)start, (GLsizei)count);
}

void CContextGLCommon::DrawIndexed(Primitive primitive, u32 count, u32 start)
{
	if (m_IndexBuffer)
	{
		GLenum type;
		size_t elementSize;
		switch (m_IndexBuffer->GetType())
		{
		case BufferType_Index16:
			type = GL_UNSIGNED_SHORT;
			elementSize = sizeof(u16);
			break;
		case BufferType_Index32:
			type = GL_UNSIGNED_INT;
			elementSize = sizeof(u32);
			break;
		default:
			LOGE("The selected index buffer is an unsupported type");
			return;
		}

		glDrawElements(GetPrimitive(primitive), (GLsizei)count, type, (const GLvoid*)(start * elementSize));
	}
}

///////////////////////////////////////////////////////////////////
// Context specifics
void CContextGLCommon::SelectQuadIndexBuffer() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIndexBuffer);
}
void CContextGLCommon::SelectSurfaceId(GLuint id, GLenum index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, id);
}
bool CContextGLCommon::CompileShader(const char* source, const int length, const unsigned int type, GLint& id) {
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
	if (!CHECKERROR || id == 0)
	{
		return false;
	}

	GLint shader_ok = GL_FALSE;
	glShaderSource(id, 1, (const GLchar**)&source, &length);
	CHECKERROR;

	glCompileShader(id);
	CHECKERROR;

	glGetShaderiv(id, GL_COMPILE_STATUS, &shader_ok);
	CHECKERROR;

	if (shader_ok == GL_FALSE)
	{
		LOGE("Shader error sourcecode %s\n", source);
		ShaderError(id, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(id);
		return false;
	}
	return true;
}
void CContextGLCommon::DeleteShader(GLint id) {
	glDeleteShader(id);
}
bool CContextGLCommon::BuildProgram(GLint* shaders, unsigned int count, GLint& id) {
	GLint program_ok;

	id = glCreateProgram();
	for (unsigned int i = 0; i < count; i++)
	{
		glAttachShader(id, shaders[(ptrdiff_t)i]);
	}
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &program_ok);
	if (!CHECKERROR || program_ok == GL_FALSE)
	{
		ShaderError((GLuint)id, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(id);
		return false;
	}

	return true;
}
void CContextGLCommon::DeleteProgram(GLint id) {
	glDeleteProgram(id);
}
void CContextGLCommon::ShaderError(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog) {
	GLint log_length;
	GLchar* outlog;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	outlog = new char[log_length];
	glGet__InfoLog(object, (GLsizei)log_length, NULL, outlog);
#if defined(PLATFORM_WIN32)
	OutputDebugStringA(outlog);
#endif
	delete[] outlog;
}

///////////////////////////////////////////////////////////////////
// Internal stuff
bool CContextGLCommon::CheckError(ctstring function, ctstring filename, svar line) {
	// glGetError is slow and it should be removed from Release builds
#ifdef DEVELOPMENT
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		Xe::Logger::Error(__COUNTER__, filename, line, function, "GL error %s", GetMessageError(err));
	}
	return err == GL_NO_ERROR;
#else
	return true;
#endif
}
ctstring CContextGLCommon::GetMessageError(GLenum error) {
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
	return _T("unknown error");
}

const CContextGLCommon::ColorFormatFallback CContextGLCommon::COLORFORMAT_FALLBACK[] =
{
	{ Color::Format_Unknown, 0, 0, 0 },
	{ Color::Format_BGRA8888, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE },
	{ Color::Format_RGBA8888, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
	{ Color::Format_BGRX8888, GL_RGB, GL_BGRA, GL_UNSIGNED_BYTE },
	{ Color::Format_RGBX8888, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE },
	{ Color::Format_BGR888, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE },
	{ Color::Format_RGB888, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },
	{ Color::Format_BGR565, GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV },
	{ Color::Format_RGB565, GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5 },
	{ Color::Format_BGRA5551, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV },
	{ Color::Format_ARGB1555, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1 },
	{ Color::Format_BGRX5551, GL_RGB5, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV },
	{ Color::Format_XRGB1555, GL_RGB5, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1 },
	{ Color::Format_BGRA4444, GL_RGBA4, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV },
	{ Color::Format_ARGB4444, GL_RGBA4, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4 },
	{ Color::Format_BGRX4444, GL_RGB4, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV },
	{ Color::Format_XRGB4444, GL_RGB4, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4 },
	{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
	{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
	{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
	{ Color::Format_Indexed8, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE },
	{ Color::Format_Indexed16, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_SHORT },
	{ Color::Format_Indexed32, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_INT },
};