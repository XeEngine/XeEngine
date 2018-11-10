#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include "NullSurface.h"

using namespace RenderingDriverNull;

namespace Xe { namespace Graphics {
	struct ContextState
	{
		Xe::Graphics::IBuffer* VertexBuffer;
		Xe::Graphics::IBuffer* IndexBuffer;
		Xe::Graphics::ISurface* RenderSurface;
	};

	class CContextNull : public IContext {
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

		CDrawing *m_Drawing;
		ContextState m_State;
		Size m_Size;
		Color m_ClearColor;
		float m_ClearDepth;
		int m_ClearStencil;
		CSurface *m_Surface[0x100];
		CDepthStencilState *m_DepthStencilState;
		Xe::Math::Rectangle<int> m_Scissor;

	public:
		CContextNull();
		~CContextNull();

		bool Initialize(const ContextInitDesc& properties);
		void GetDrawing(IDrawing2d** drawing);

		void GetCapabilities(Capabilities& capabilities);

		const Color &GetClearColor() const;
		void SetClearColor(const Color &color);
		float GetClearDepth() const;
		int GetClearStencil() const;
		void SetClearStencil(int stencil);

		const Xe::Math::Rectangle<int> GetScissor();
		void SetScissor(const Xe::Math::Rectangle<int>& scissor);

		void Clear(svar clearmask);
		void Draw(Primitive primitive, u32 count, u32 start);
		void DrawIndexed(Primitive primitive, u32 count, u32 start);

		bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
		void SelectSurface(ISurface *surface, svar index);
		void GetRenderingSurface(ISurface** ppSurface);
		void SetRenderingSurface(ISurface* surface);

		bool CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData);
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