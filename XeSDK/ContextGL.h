#pragma once
#ifdef PLATFORM_GL
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/XeGraphicsTilemap2d.h>
#include "XeGraphicsCommon.h"
#include "ContextGL_Common.h"

namespace Xe {
	namespace Graphics {
		class CContextGL : public CContextGLCommon {
			class CDrawing : public IDrawing2d {
			public:
				typedef Common::Vertex Vertex;

				bool Query(IObject **obj, UID id);
				CDrawing(CContextGL *context);
				~CDrawing();

				void Flush();
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
			private:
				static const svar MaximumQuadsCount = CContextGLCommon::MaximumQuadsCount;
				static const char VertexShader[];
				static const char FragmentShader[];

				CContextGL* m_pContext;
				svar m_curQuadsCount;
				Vertex *m_pVertex;

				GLint m_shader[2];
				GLint m_program;
				GLuint m_bufferVertex;
				GLuint m_AttribPos;
				GLuint m_AttribTex;
				GLuint m_AttribCol;
				GLuint m_AttribTex0;
				GLuint m_AttribTex1;

				inline Vertex *Get();
			};
			class CTilemap2d : public Tilemap2d {
				CContextGL *m_pContextGl;
			public:
				bool Query(IObject **obj, UID id);
				CTilemap2d(CContextGL *context);
				~CTilemap2d();

				void SetViewSize(const Math::Vector2f& size);
				void SetPosition(uvar layer, const Math::Vector2f& position);
			private:
				enum DrawType {
					// Classic drawing defining each vertex.
					// This should be used in GLES where gl_VertexID is not available.
					Draw_Classic,
					// Enhanced version with the use of gl_VertexID.
					// Supported from OpenGL 2.0 or greater
					Draw_VerticesIndexed,
					// Enhanced version with the use of Texture Buffer Object.
					// Supported from OpenGL 3.1 or greater
					Draw_TexInstanced,
					// Fastest version with gl_InstanceID support.
					// Supported from OpenGL 3.3 or greater
					Draw_VboInstanced,
				};
				struct VertexClassic {
					GLfloat x, y;
					GLfloat u, v;
				};
				struct VertexIndexed {
					GLfloat index;
				};

				static const char VertexShaderClassic[];
				static const char VertexShaderVerticesIndexed[];
				static const char FragmentShader[];

				DrawType m_DrawType;
				GLint m_shader[2];
				GLint m_program;
				GLuint m_AttribTileIndex;
				GLuint m_AttribPos;
				GLuint m_AttribTex;
				GLuint m_UniformPosition;
				GLuint m_UniformCamera;
				GLuint m_UniformTex0;
				GLuint m_UniformTex1;

				// VBO that will store all vertces to send to the GPU
				GLuint m_VboTiles;
				void *m_VboData;
				uvar m_VboDataLength;
				// Uniforms to control the size of tiles and map
				GLuint m_UniformTileSize;
				GLuint m_UniformTilesetSize;
				GLuint m_UniformTilemapSize;

				// Draw with Texture Buffer Object instanced
				GLuint m_Tbo;
				GLuint m_TboTexture;
				GLuint m_UniformTboTilemap;

				bool m_IsCameraInvalidated;
				Math::Vector2<svar> *m_LayerTilePos;
				Math::Matrix4 m_mCamera;

				void OnDraw();
				void OnDrawClassic();
				void OnDrawIndexed();
				void OnDrawTexInstanced();
				void OnDrawVboInstanced();

				void OnTilesetChanged(ISurface *pSurface);
				void OnColorLutChanged(ISurface *pSurface);
			};

			CTilemap2d *m_Tilemap;
			Size m_Size;

			virtual void SwapBuffersSub(VBlankSync sync) = 0;

		public:
			bool Query(IObject **obj, UID id);
			CContextGL(Core::IView* pView);
			~CContextGL();

			virtual bool Initialize(const ContextProperties& properties);
			void GetDrawing(IDrawing2d** drawing);
			void GetTilemap(Tilemap2d** tilemap);

			void SetClearDepth(float depth);
			void SwapBuffers(VBlankSync sync);
		};
	}
}
#endif