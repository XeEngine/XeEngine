#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/XeGraphicsTilemap2d.h>

namespace Xe {
	namespace Graphics {
		class CContextNull : public IContext {
			class CSurface : public ISurface {
			public:
				static const UID ID = 0x46a41097544f409fULL;
				IContext *m_context;

				bool Query(IObject **obj, UID id);
				CSurface(IContext *context, SurfaceType type, const Size &size, Color::Format format);
				~CSurface();
			};
			class CDepthStencilState : public IDepthStencilState {
			public:
				static const UID ID = 0xd2ea060ada7b4016ULL;
				IContext *m_context;

				bool Query(IObject **obj, UID id);
				CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc);
				~CDepthStencilState();
			};
			class CDrawing : public IDrawing2d {
				CContextNull* m_context;
			public:
				bool Query(IObject **obj, UID id);
				CDrawing(CContextNull *context);
				~CDrawing();

				bool GetDestinationSurface(ISurface **surface);
				void SetDestinationSurface(ISurface *surface);
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
			};
			class CTilemap2d : public Tilemap2d {
				IContext *m_Context;
			public:
				bool Query(IObject **obj, UID id);
				CTilemap2d(IContext *context);
				~CTilemap2d();

				void SetViewSize(const Math::Vector2f& size);
				void SetPosition(uvar layer, const Math::Vector2f& position);
				void OnDraw();
				void OnTilesetChanged(ISurface *pSurface);
				void OnColorLutChanged(ISurface *pSurface);
			};

			CDrawing *m_Drawing;
			CTilemap2d *m_Tilemap;
			Size m_Size;
			Color m_ClearColor;
			float m_ClearDepth;
			int m_ClearStencil;
			CSurface *m_Surface[0x100];
			CDepthStencilState *m_DepthStencilState;

		public:
			bool Query(IObject **obj, UID id);
			CContextNull();
			~CContextNull();

			bool Initialize(const ContextProperties& properties);
			void GetDrawing(IDrawing2d** drawing);
			void GetTilemap(Tilemap2d** tilemap);

			const Color &GetClearColor() const;
			void SetClearColor(const Color &color);
			float GetClearDepth() const;
			void SetClearDepthSub(float depth);
			int GetClearStencil() const;
			void SetClearStencil(int stencil);
			void Clear(svar clearmask);

			bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
			void SelectSurface(ISurface *surface, svar index);
			bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc);
			void SelectDepthStencilState(IDepthStencilState *depthStencilState);

			void SetClearDepth(float depth);
			void SwapBuffers(VBlankSync sync);
			void SetInternalResolution(const Size &size);
		};
	}
}