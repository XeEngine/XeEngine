#pragma once
#ifdef PLATFORM_GL
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsTilemap.h>
#include "XeGraphicsCommon.h"
#include "ContextGL.Common.h"

namespace Xe {
	namespace Graphics {
		class CContextGL : public CContextGLCommon {
			class CDrawing : public IDrawing2d {
			public:
				bool Query(IObject **obj, UID id);
				CDrawing(CContextGL *context);
				~CDrawing();

				void Flush();
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]);
				void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color(&color)[4]);
				void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color(&color)[4]);
				void DrawRectangle(const Math::Vector3f(&position)[4], const Color &color);
				void DrawRectangle(const Math::Vector3f(&position)[4], const Color(&color)[4]);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
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
			class CTilemap : public ITilemap {
				IContext* m_pContext;
				IDrawing2d* m_pDrawing;

				Size m_TileSize;
				Vector2f m_TileSizef;

				int m_TilesPerRow;
				Math::Rectangle<float> m_TilesetRectf;
				Vector2f m_TilesetPos;
				Vector2f m_TilesetSize;
				Vector2f m_TilesetMul;
				Vector2f m_TilesetPadding;

				Rectanglef m_Camera;

				Size m_MapSize;
				int m_ParallaxSize;
				TileData* m_Tilemap;
				float* m_Parallax;

				void SetTileset(const TilesetProperties& tileset);

				const Size& GetMapSize() const;
				void SetMapSize(const Size& size);

				void Lock(TilemapData& data);
				void Unlock();

				const Rectanglef& GetCamera() const;
				void SetCamera(const Rectanglef& camera);

				void Draw(int flags);
				void DrawStandard() const;
				void DrawFlip() const;

			public:
				bool Query(IObject **obj, UID id);
				CTilemap(IContext* context);
				~CTilemap();
			};

			Size m_Size;

			virtual void SwapBuffersSub(VBlankSync sync) = 0;

		public:
			bool Query(IObject **obj, UID id);
			CContextGL(Core::IView* pView);
			~CContextGL();

			virtual bool Initialize(const ContextProperties& properties);
			void GetDrawing(IDrawing2d** drawing);
			void CreateTilemap(ITilemap** pTilemap);

			void SetClearDepth(float depth);
			void SwapBuffers(VBlankSync sync);
		};
	}
}
#endif