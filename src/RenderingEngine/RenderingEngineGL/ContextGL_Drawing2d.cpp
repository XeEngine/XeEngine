#include "pch.h"
#ifdef PLATFORM_GL
#include "ContextGL.h"
#include <XeSDK/XeMemory.h>

#define ENDL "\n"

namespace Xe {
	namespace Graphics {
		const char CContextGL::CDrawing::VertexShader[] =
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
		const char CContextGL::CDrawing::FragmentShader[] =
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

		///////////////////////////////////////////////////////////////////////
		// IDrawing2d implementation
		bool CContextGL::CDrawing::Query(IObject **obj, UID id) {
			switch (id) {
			case IDrawing2d::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			case IContext::ID:
				m_pContext->AddRef();
				*obj = m_pContext;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextGL::CDrawing::CDrawing(CContextGL *context) :
			m_pContext(context),
			m_curQuadsCount(0),
			m_pVertex(new Vertex[MaximumQuadsCount * 4]) {
			if (m_pContext->CompileShader(VertexShader, sizeof(VertexShader), 0, m_shader[0]) == false ||
				m_pContext->CompileShader(FragmentShader, sizeof(FragmentShader), 1, m_shader[1]) == false)
				return;
			if (m_pContext->BuildProgram(m_shader, 2, m_program) == false)
				return;
			glUseProgram(m_program);

			m_AttribPos = glGetAttribLocation(m_program, "a_position");
			m_AttribTex = glGetAttribLocation(m_program, "a_texture");
			m_AttribCol = glGetAttribLocation(m_program, "a_color");
			m_AttribTex0 = glGetUniformLocation(m_program, "tImage");
			m_AttribTex1 = glGetUniformLocation(m_program, "tClut");
			glGenBuffers(1, &m_bufferVertex);
		}
		CContextGL::CDrawing::~CDrawing() {
			Flush();

			glDeleteBuffers(1, &m_bufferVertex);
			glDeleteProgram(m_program);
			glDeleteShader(m_shader[0]);
			glDeleteShader(m_shader[1]);
			delete[] m_pVertex;
		}

		CContextGL::CDrawing::Vertex *CContextGL::CDrawing::Get() {
			if (m_curQuadsCount < MaximumQuadsCount)
				return m_pVertex + m_curQuadsCount++ * 4;
			Flush();
			m_curQuadsCount = 0;
			return m_pVertex;
		}
		void CContextGL::CDrawing::Flush() {
			if (m_curQuadsCount <= 0)
				return;

			// Set attributes
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferVertex);
			m_pContext->SelectQuadIndexBuffer();

			// Send updated buffer
			size_t buflen = sizeof(Vertex) * 4 * m_curQuadsCount;
			glBufferData(GL_ARRAY_BUFFER, buflen, 0, GL_STREAM_DRAW);
			void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (data) {
				Memory::Copy(data, m_pVertex, buflen);
				glUnmapBuffer(GL_ARRAY_BUFFER);
				// Set shader
				glUseProgram(m_program);
				glUniform1i(m_AttribTex0, 0);
				glUniform1i(m_AttribTex1, 1);
				// Draw data
				glVertexAttribPointer(m_AttribPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
				glVertexAttribPointer(m_AttribTex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)12);
				glVertexAttribPointer(m_AttribCol, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)24);

				glEnableVertexAttribArray(m_AttribPos);
				glEnableVertexAttribArray(m_AttribTex);
				glEnableVertexAttribArray(m_AttribCol);
				glDrawElements(GL_TRIANGLES, 6 * m_curQuadsCount, GL_UNSIGNED_SHORT, 0);
				glDisableVertexAttribArray(m_AttribCol);
				glDisableVertexAttribArray(m_AttribTex);
				glDisableVertexAttribArray(m_AttribPos);
			}
			// Validate content
			m_curQuadsCount = 0;
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector3f(&position)[4], const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextGL::CDrawing::DrawRectangle(const Math::Vector3f(&position)[4], const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextGL::CDrawing::DrawSurface(const Math::Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}

	}
}
#endif