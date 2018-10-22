#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsTilemap.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsTilemap.h>

namespace Xe { namespace Graphics {
	struct ContextState
	{
		Xe::Graphics::IBuffer* VertexBuffer;
		Xe::Graphics::IBuffer* IndexBuffer;
	};

	class CContextNull : public IContext {
		class CSurface : public ISurface {
		public:
			IContext *m_context;

			CSurface(IContext *context, SurfaceType type, const Size &size, Color::Format format);
			~CSurface();

			// Inherited via ISurface
			virtual bool SubLock(DataDesc& map, LockType type);
			virtual void SubUnlock();
		};

		class CDepthStencilState : public IDepthStencilState {
		public:
			IContext *m_context;

			CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc);
			~CDepthStencilState();
		};
		class CDrawing : public IDrawing2d {
			CContextNull* m_context;
		public:
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
		class CTilemap : public ITilemap {
			IContext *m_pContext;
			Size m_Size;

			void SetTileset(const TilesetProperties& tileset);

			const Size& GetMapSize() const;
			void SetMapSize(const Size& size);

			void Lock(TilemapData& data);
			void Unlock();

			const Math::Rectanglef& GetCamera() const;
			void SetCamera(const Math::Rectanglef& camera);

			void Draw(int flags);
		public:
			CTilemap(IContext *context);
			~CTilemap();
		};

		CDrawing *m_Drawing;
		ContextState m_State;
		Size m_Size;
		Color m_ClearColor;
		float m_ClearDepth;
		int m_ClearStencil;
		CSurface *m_Surface[0x100];
		CDepthStencilState *m_DepthStencilState;

	public:
		CContextNull();
		~CContextNull();

		bool Initialize(const ContextInitDesc& properties);
		void GetDrawing(IDrawing2d** drawing);
		void CreateTilemap(ITilemap** tilemap);

		void GetCapabilities(Capabilities& capabilities);

		const Color &GetClearColor() const;
		void SetClearColor(const Color &color);
		float GetClearDepth() const;
		void SetClearDepthSub(float depth);
		int GetClearStencil() const;
		void SetClearStencil(int stencil);

		void Clear(svar clearmask);
		void Draw(Primitive primitive, u32 count, u32 start);
		void DrawIndexed(Primitive primitive, u32 count, u32 start);

		bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
		void SelectSurface(ISurface *surface, svar index);

		bool CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData);
		void SelectBuffer(IBuffer *pBuffer);
		void GetVertexBuffer(IBuffer** ppBuffer);
		void SetVertexBuffer(IBuffer* pBuffer);
		void GetIndexBuffer(IBuffer** ppBuffer);
		void SetIndexBuffer(IBuffer* pBuffer);

		bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc);
		void SelectDepthStencilState(IDepthStencilState *depthStencilState);

		void SetClearDepth(float depth);
		void SwapBuffers(VBlankSync sync);
		void SetInternalResolution(const Size &size);
	};
} }