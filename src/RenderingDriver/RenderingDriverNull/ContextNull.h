#pragma once
#include <XeSDK/IGraphicsContext.h>
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

		ContextState m_State;
		Size m_Size;
		Color m_ClearColor;
		float m_ClearDepth;
		int m_ClearStencil;
		CSurface *m_Surface[0x100];
		CDepthStencilState *m_DepthStencilState;
		Xe::Math::Rectangle<int> m_Scissor;
		bool m_ScissorEnabled;

	public:
		CContextNull();
		~CContextNull();

		bool Initialize(const ContextInitDesc& properties);

		void GetCapabilities(Capabilities& capabilities);

		const Color &GetClearColor() const;
		void SetClearColor(const Color &color);
		float GetClearDepth() const;
		int GetClearStencil() const;
		void SetClearStencil(int stencil);

		bool IsScissorEnabled() const;
		void SetScissorEnabled(bool enabled);
		const Xe::Math::Rectangle<int> GetScissor() const;
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