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

			Size m_Size;

			virtual void SwapBuffersSub(VBlankSync sync) = 0;

		public:
			CContextGL(Core::IView* pView);
			~CContextGL();

			virtual bool Initialize(const ContextInitDesc& properties);
			void GetDrawing(IDrawing2d** drawing);

			void GetCapabilities(Capabilities& capabilities);

			void SetClearDepth(float depth);
			void SwapBuffers(VBlankSync sync);
		};
	}
}
#endif