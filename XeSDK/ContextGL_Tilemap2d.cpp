#include "pch.h"
#include "ContextGL.h"
#ifdef PLATFORM_GL
#include <XeSDK/XeMemory.h>

#define ENDL "\n"

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// Tilemap2d implementation
		const char CContextGL::CTilemap2d::VertexShaderClassic[] =
		{
			"#version 110\n"
			ENDL"attribute vec2 aPosition;"
			ENDL"attribute vec2 aTexture;"
			ENDL"uniform vec2 uPosition;"
			ENDL"uniform mat4 mCamera;"
			ENDL"varying vec2 vTexture;"
			ENDL
			ENDL"void main()"
			ENDL"{"
			ENDL"vTexture = aTexture;"
			ENDL"vec2 pos = aPosition - uPosition;"
			ENDL"gl_Position = mCamera * vec4(pos, 0.0, 1.0);"
			ENDL"}"
		};
		const char CContextGL::CTilemap2d::VertexShaderVerticesIndexed[] =
		{
			"#version 110\n"
			ENDL"attribute float aTileIndex;"
			ENDL"uniform float cTileSize;"
			ENDL"uniform ivec2 cTilemapSize;"
			ENDL"uniform vec2 cTilesetSize;"
			ENDL"uniform vec2 uPosition;"
			ENDL"uniform mat4 mCamera;"
			ENDL"varying vec2 vTexture;"
			ENDL
			ENDL"const vec2 verts[4] = vec2[4] ("
			ENDL"vec2(0.0, 0.0),"
			ENDL"vec2(16.0, 0.0),"
			ENDL"vec2(0.0, 16.0),"
			ENDL"vec2(16.0, 16.0)"
			ENDL");"
			ENDL
			ENDL"const vec2 tiluv[4] = vec2[4] ("
			ENDL"vec2(0.0, 0.0),"
			ENDL"vec2(1.0, 0.0),"
			ENDL"vec2(0.0, 1.0),"
			ENDL"vec2(1.0, 1.0)"
			ENDL");"
			ENDL
			ENDL"void main()"
			ENDL"{"
			ENDL"int vertId = int(mod(gl_VertexID, 4));"
			ENDL"float vertIdPos = mod(gl_VertexID / 4, cTilemapSize.x);"
			ENDL""
			ENDL"vec2 tileDiv = vec2(1.0 / cTilesetSize.x, 1.0 / cTilesetSize.y);"
			ENDL"vTexture = vec2(tileDiv.x * mod(aTileIndex, cTilesetSize.x), tileDiv.y * floor(aTileIndex / cTilesetSize.y));"
			ENDL"vTexture += tileDiv * tiluv[vertId];"
			ENDL"vec2 pos = verts[vertId];"
			ENDL"pos.x += vertIdPos * cTileSize;"
			ENDL"pos.y += gl_VertexID / 4 / cTilemapSize.x * cTileSize;"
			ENDL"pos -= uPosition;"
			ENDL"gl_Position = mCamera * vec4(pos.x, pos.y, 0.0f, 1.0);"
			ENDL"}"
		};
		const char CContextGL::CTilemap2d::FragmentShader[] =
		{
			"#version 110\n"
			ENDL"uniform sampler2D tImage;"
			ENDL"uniform sampler2D tClut;"
			ENDL"varying vec2 vTexture;"
			ENDL
			ENDL"void main()"
			ENDL"{"
			ENDL"gl_FragColor = texture2D(tImage, vTexture);"
			ENDL"}"
		};

		bool CContextGL::CTilemap2d::Query(IObject **obj, UID id) {
			switch (id) {
			case Tilemap2d::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextGL::CTilemap2d::CTilemap2d(CContextGL *context) :
			Tilemap2d(context),
			m_pContextGl(context),
			m_IsCameraInvalidated(true),
			m_mCamera(Math::Matrix4::Identity()),
			m_VboTiles(0),
			m_VboData(nullptr)
		{
			LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _S("Initializing CContextGL::CTilemap2d..."));
			m_LayerTilePos = new Math::Vector2<svar>[LayersCount];

			glGenBuffers(1, &m_VboTiles);

			/*if (glTexBuffer) {
			m_DrawType = Draw_VerticesIndexed;
			}
			else {
			m_DrawType = Draw_Classic;
			}*/
			m_DrawType = Draw_VerticesIndexed;
			switch (m_DrawType) {
			case Draw_Classic:
				m_VboDataLength = sizeof(VertexClassic) * 4 * TilemapWidth * TilemapHeight;
				m_VboData = Memory::Alloc(m_VboDataLength);
				if (m_pContextGl->CompileShader(VertexShaderClassic, sizeof(VertexShaderClassic), 0, m_shader[0]) == false ||
					m_pContextGl->CompileShader(FragmentShader, sizeof(FragmentShader), 1, m_shader[1]) == false)
					return;
				if (m_pContextGl->BuildProgram(m_shader, 2, m_program) == false)
					return;
				m_AttribPos = glGetAttribLocation(m_program, "aPosition");
				m_AttribTex = glGetAttribLocation(m_program, "aTexture");
				m_UniformPosition = glGetUniformLocation(m_program, "uPosition");
				m_UniformCamera = glGetUniformLocation(m_program, "mCamera");
				m_UniformTex0 = glGetUniformLocation(m_program, "tImage");
				m_UniformTex1 = glGetUniformLocation(m_program, "tClut");
				glUseProgram(m_program);
				glUniform1i(m_UniformTex0, 0);
				glUniform1i(m_UniformTex1, 1);
				break;
			case Draw_VerticesIndexed:
				m_VboDataLength = sizeof(VertexIndexed) * 4 * TilemapWidth * TilemapHeight;
				m_VboData = Memory::Alloc(m_VboDataLength);
				if (m_pContextGl->CompileShader(VertexShaderVerticesIndexed, sizeof(VertexShaderVerticesIndexed), 0, m_shader[0]) == false ||
					m_pContextGl->CompileShader(FragmentShader, sizeof(FragmentShader), 1, m_shader[1]) == false)
					return;
				if (m_pContextGl->BuildProgram(m_shader, 2, m_program) == false)
					return;
				m_AttribTileIndex = glGetAttribLocation(m_program, "aTileIndex");
				m_UniformTileSize = glGetUniformLocation(m_program, "cTileSize");
				m_UniformTilemapSize = glGetUniformLocation(m_program, "cTilemapSize");
				m_UniformTilesetSize = glGetUniformLocation(m_program, "cTilesetSize");
				m_UniformPosition = glGetUniformLocation(m_program, "uPosition");
				m_UniformCamera = glGetUniformLocation(m_program, "mCamera");
				m_UniformTex0 = glGetUniformLocation(m_program, "tImage");
				m_UniformTex1 = glGetUniformLocation(m_program, "tClut");
				glUseProgram(m_program);
				glUniform1f(m_UniformTileSize, (float)TileSize);
				glUniform2i(m_UniformTilemapSize, (GLuint)TilemapWidth, (GLuint)TilemapHeight);
				glUniform1i(m_UniformTex0, 0);
				glUniform1i(m_UniformTex1, 1);
				break;
			case Draw_TexInstanced:
				/*glGenBuffers(1, &m_Tbo);
				glBindBuffer(GL_TEXTURE_BUFFER, m_Tbo);
				glActiveTexture(GL_TEXTURE0 + 2);
				glGenTextures(1, &m_TboTexture);
				glBindTexture(GL_TEXTURE_BUFFER, m_TboTexture);
				glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8UI, m_Tbo);*/
				break;
			case Draw_VboInstanced:
				break;
			}
		}
		CContextGL::CTilemap2d::~CTilemap2d() {
			if (m_VboDataLength) Memory::Free(m_VboData);
			if (m_VboTiles) glDeleteBuffers(1, &m_VboTiles);
			switch (m_DrawType) {
			case Draw_TexInstanced:
				glDeleteTextures(1, &m_TboTexture);
				break;
			}
			glDeleteProgram(m_program);
			glDeleteShader(m_shader[0]);
			glDeleteShader(m_shader[1]);
			delete[] m_LayerTilePos;
		}
		void CContextGL::CTilemap2d::SetViewSize(const Math::Vector2f& size) {
			m_ViewSize = size;
			m_mCamera = Math::Matrix4::Ortho(0.0f, size.x, size.y, 0.0f, 0.0f, +1.0f);
			m_IsCameraInvalidated = true;
		}
		void CContextGL::CTilemap2d::SetPosition(uvar layer, const Math::Vector2f& position) {
			m_LayerPos[layer] = position;
			Math::Vector2<svar> tilepos((svar)position.x / TileSize, (svar)position.y / TileSize);
			if (m_LayerTilePos[layer] != tilepos) {
				m_LayerTilePos[layer] = tilepos;
				Invalidate();
			}
			glUniform2f(m_UniformPosition,
				Math::Fmod(position.x, (float)TileSize),
				Math::Fmod(position.y, (float)TileSize));
			//glUniform2f(m_UniformPosition, position.x, position.y);
		}
		void CContextGL::CTilemap2d::OnDraw() {
			if (!m_pTileset) return;
			switch (m_DrawType) {
			case Draw_Classic:
				OnDrawClassic();
				break;
			case Draw_VerticesIndexed:
				OnDrawIndexed();
				break;
			case Draw_TexInstanced:
				OnDrawTexInstanced();
				break;
			case Draw_VboInstanced:
				OnDrawVboInstanced();
				break;
			}
		}
		void CContextGL::CTilemap2d::OnDrawClassic() {
			static const float TileSizef = (float)TileSize;
			const Size& sizeTileset = m_pTileset->GetSize();
			const svar tilesPerRow = sizeTileset.x / TileSize;
			const float tilesPerRowf = (float)tilesPerRow;
			const float texW = 1.0f / (float)sizeTileset.x * TileSizef;
			const float texH = 1.0f / (float)sizeTileset.y * TileSizef;

			int w = TilemapWidth;
			int h = TilemapHeight;
			// Upload tilemap's vertices, if necessary
			m_pContextGl->SelectQuadIndexBuffer();
			glBindBuffer(GL_ARRAY_BUFFER, m_VboTiles);
			if (IsInvaldiated()) {
				Validate();

				VertexClassic *pVboData = (VertexClassic*)m_VboData;

				Math::Vector2f pos = m_LayerPos[0];
				int sj = (int)pos.x / TileSize;
				int si = (int)pos.y / TileSize;
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						VertexClassic *v = pVboData;
						pVboData += 4;
						// Get the tile to draw
						Tile t = GetTile((sj + j) % w, (si + i) % h);
						//if (t.tile != 0)
						{
							// Build vertex
							v[0].x = TileSizef * j;
							v[0].y = TileSizef * i;
							v[0].u = (float)(t.tile % tilesPerRow) * texW;
							v[0].v = (float)(t.tile / tilesPerRow) * texH;
							v[1].x = v[0].x + TileSizef;
							v[1].y = v[0].y;
							v[1].u = v[0].u + texW;
							v[1].v = v[0].v;
							v[2].x = v[0].x;
							v[2].y = v[0].y + TileSizef;
							v[2].u = v[0].u;
							v[2].v = v[0].v + texH;
							v[3].x = v[1].x;
							v[3].y = v[2].y;
							v[3].u = v[1].u;
							v[3].v = v[2].v;
						}
					}
				}

				glBufferData(GL_ARRAY_BUFFER, m_VboDataLength, 0, GL_STREAM_DRAW);
				void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				if (data) {
					Memory::Copy(data, m_VboData, m_VboDataLength);
					glUnmapBuffer(GL_ARRAY_BUFFER);
				}
			}
			// Set texture
			glActiveTexture(GL_TEXTURE0);
			CSurface *pTileset = (CSurface*)m_pTileset;
			glBindTexture(GL_TEXTURE_2D, pTileset->m_GlId);
			// Set shader and its properties
			glUseProgram(m_program);
			if (m_IsCameraInvalidated) {
				m_IsCameraInvalidated = false;
				glUniformMatrix4fv(m_UniformCamera, 1, GL_FALSE, m_mCamera.n);
			}

			glEnableVertexAttribArray(m_AttribPos);
			glEnableVertexAttribArray(m_AttribTex);
			glVertexAttribPointer(m_AttribPos, 2, GL_FLOAT, GL_FALSE, sizeof(VertexClassic), (GLvoid*)0);
			glVertexAttribPointer(m_AttribTex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexClassic), (GLvoid*)8);
			glDrawElements(GL_TRIANGLES, 6 * w * h, GL_UNSIGNED_SHORT, 0);
			glDisableVertexAttribArray(m_AttribTex);
			glDisableVertexAttribArray(m_AttribPos);
		}
		void CContextGL::CTilemap2d::OnDrawIndexed() {
			int w = TilemapWidth;
			int h = TilemapHeight;
			VertexIndexed *pVboData = (VertexIndexed*)m_VboData;

			const Math::Vector2f& pos = m_LayerPos[0];
			int sj = (int)pos.x / TileSize, si = (int)pos.y / TileSize;
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					VertexIndexed *v = pVboData;
					pVboData += 4;
					// Get the tile to draw
					const Tile t = GetTile((sj + j) % w, (si + i) % h);
					float TILE = (float)t.tile;
					// Build vertex
					v[0].index = TILE;
					v[1].index = TILE;
					v[2].index = TILE;
					v[3].index = TILE;
				}
			}
			// Upload tilemap's vertices
			m_pContextGl->SelectQuadIndexBuffer();
			glBindBuffer(GL_ARRAY_BUFFER, m_VboTiles);
			if (IsInvaldiated()) {
				Validate();
				glBufferData(GL_ARRAY_BUFFER, m_VboDataLength, 0, GL_STREAM_DRAW);
				void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				Memory::Copy(data, m_VboData, m_VboDataLength);
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
			// Set texture; check if it's null is done by OnDraw
			glActiveTexture(GL_TEXTURE0);
			CSurface *pTileset = (CSurface*)m_pTileset;
			glBindTexture(GL_TEXTURE_2D, pTileset->m_GlId);
			// Set shader and its properties
			glUseProgram(m_program);
			if (m_IsCameraInvalidated) {
				m_IsCameraInvalidated = false;
				glUniformMatrix4fv(m_UniformCamera, 1, GL_FALSE, m_mCamera.n);
			}

			glEnableVertexAttribArray(m_AttribTileIndex);
			glVertexAttribPointer(m_AttribTileIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexIndexed), (GLvoid*)0);
			glDrawElements(GL_TRIANGLES, 6 * w * h, GL_UNSIGNED_SHORT, 0);
			glDisableVertexAttribArray(m_AttribTileIndex);
		}
		void CContextGL::CTilemap2d::OnDrawTexInstanced() {
			/*static const GLsizeiptr BufferLength = sizeof(Tile) * TilemapWidth * TilemapHeight;
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_BUFFER, m_TboTexture);

			// Upload new tilemap data
			glBindBuffer(GL_TEXTURE_BUFFER, m_Tbo);
			glBufferData(GL_TEXTURE_BUFFER, BufferLength, 0, GL_STREAM_DRAW);
			void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(data, Tilemap, BufferLength);
			glUnmapBuffer(GL_ARRAY_BUFFER);*/
		}
		void CContextGL::CTilemap2d::OnDrawVboInstanced() {

		}
		void CContextGL::CTilemap2d::OnTilesetChanged(ISurface *pSurface) {
			if (m_UniformTilesetSize != (GLuint)-1 && pSurface != nullptr) {
				const Size& sizeTileset = m_pTileset->GetSize();
				glUniform2f(m_UniformTilesetSize, (float)(sizeTileset.x / TileSize), (float)(sizeTileset.y / TileSize));
			}
		}
		void CContextGL::CTilemap2d::OnColorLutChanged(ISurface *pSurface) {

		}
	}
}
#endif