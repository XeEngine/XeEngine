#include "pch.h"
#include "ContextD3D11.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMemory.h>

#if !_DURANGO
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#else
#pragma comment(lib, "d3d11_x.lib")
#endif

namespace Xe { namespace Graphics {
	CContextD3D11::CContextD3D11() :
		m_pFrameView(nullptr),
		m_Size(800, 480),
		m_ClearColor(Color::Black),
		m_ClearDepth(0.0f),
		m_ClearStencil(0),
		m_DepthStencilState(nullptr),

		p_d3dDevice(nullptr),
		m_d3dContext(nullptr),
		p_d3dDevice1(nullptr),
		m_d3dContext1(nullptr),

		m_dxgiDevice1(nullptr),
		m_dxgiAdapter(nullptr),
		m_pFactory(nullptr),
		m_pFactory1(nullptr),
		m_pFactory2(nullptr),

		m_swapChain(nullptr),
		m_swapChain1(nullptr),
		m_pBackbufferTexture(nullptr),
		m_pBackbufferDepthStencil(nullptr),
		m_pBackbufferRenderTargetView(nullptr),
		m_pDepthStencilView(nullptr)
	{
		Memory::Fill(&m_State, 0, sizeof(m_State));
		Memory::Fill(m_Surface, 0, sizeof(m_Surface));
	}
	CContextD3D11::~CContextD3D11() {
		ULONG r;
		for (svar i = 0; i < lengthof(m_Surface); i++)
			if (m_Surface[i]) m_Surface[i]->Release();
		if (m_DepthStencilState) m_DepthStencilState->Release();
		if (m_d3dContext1) r = m_d3dContext1->Release();
		if (p_d3dDevice1) r = p_d3dDevice1->Release();
		if (m_d3dContext) r = m_d3dContext->Release();
		if (p_d3dDevice) r = p_d3dDevice->Release();

		if (m_dxgiDevice1) r = m_dxgiDevice1->Release();
		if (m_dxgiAdapter) r = m_dxgiAdapter->Release();

		if (m_pFactory) {
			m_pFactory->Release();
			if (m_pFactory1) {
				m_pFactory1->Release();
				if (m_pFactory2) m_pFactory2->Release();
			}
		}

		if (m_swapChain1) m_swapChain1->Release();
		if (m_swapChain) {
			m_swapChain->SetFullscreenState(false, NULL);
			if (m_pBackbufferTexture) m_pBackbufferTexture->Release();
			if (m_pBackbufferRenderTargetView) m_pBackbufferRenderTargetView->Release();
			if (m_pBackbufferDepthStencil) m_pBackbufferDepthStencil->Release();
			if (m_pDepthStencilView) m_pDepthStencilView->Release();
			m_swapChain->Release();
		}

		m_pFrameView->Release();
	}

	bool CContextD3D11::Initialize(const ContextInitDesc& properties)
	{
		ASSERT(properties.FrameView != nullptr);

		m_pFrameView = properties.FrameView;
		m_pFrameView->AddRef();

		bool success = CreateDevice(properties) && CreateResources() &&
			CreateWindowSizeDependentResources();
		if (success == true)
		{
			SetSwapChainViewport(m_pFrameView->GetSize());
			for (svar i = 0; i < lengthof(m_Surface); i++)
				SelectSurface(nullptr, i);
		}

		return success;
	}

	void CContextD3D11::GetCapabilities(Capabilities& capabilities)
	{
		capabilities.MaxTexture1DSize = D3D11_REQ_TEXTURE1D_U_DIMENSION;
		capabilities.MaxTexture1DArrayLength = D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION;
		capabilities.MaxTexture2DSize = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
		capabilities.MaxTexture2DArrayLength = D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
		capabilities.MaxTexture3DSize = D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
		capabilities.MaxTextureCubeSize = D3D11_REQ_TEXTURECUBE_DIMENSION;
		capabilities.MaxTextureUnits = D3D11_VS_INPUT_REGISTER_COUNT;
		capabilities.MaxAntisotropy = D3D11_REQ_MAXANISOTROPY;
	}

	void CContextD3D11::SetInternalResolution(const Size& size) {
		if (m_pBackbufferTexture)
		{
			auto refs = m_pBackbufferTexture->Release();
			if (refs > 0) LOGW("m_pBackbufferTexture has still %i refs", refs);
			m_pBackbufferTexture = nullptr;
		}

		if (m_pBackbufferRenderTargetView)
		{
			m_pBackbufferRenderTargetView->Release();
			m_pBackbufferRenderTargetView = nullptr;
		}

		HRESULT hr = m_swapChain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
		if (SUCCEEDED(hr))
		{
			CreateResourcesForSwapchain();
			SetSwapChainViewport(size);
		}
		else
		{
			LOGF("Unable to resize the swapchain (hr=%08X)", hr);
		}
	}

	const Color &CContextD3D11::GetClearColor() const {
		return m_ClearColor;
	}
	void CContextD3D11::SetClearColor(const Color &color) {
		m_ClearColor = color;
		m_d3dClearColor[0] = color.r;
		m_d3dClearColor[1] = color.g;
		m_d3dClearColor[2] = color.b;
		m_d3dClearColor[3] = color.a;
	}
	float CContextD3D11::GetClearDepth() const {
		return m_ClearDepth;
	}
	void CContextD3D11::SetClearDepth(float depth) {
		m_ClearDepth = depth;
	}
	int CContextD3D11::GetClearStencil() const {
		return m_ClearStencil;
	}
	void CContextD3D11::SetClearStencil(int stencil) {
		m_ClearStencil = stencil;
	}

	bool CContextD3D11::IsScissorEnabled() const
	{
		return m_RasterizerDesc.ScissorEnable;
	}

	void CContextD3D11::SetScissorEnabled(bool enabled)
	{
		if (m_RasterizerDesc.ScissorEnable != enabled)
		{
			m_RasterizerDesc.ScissorEnable = enabled;
			CommitRasterizerDesc();
		}
	}

	const Xe::Math::Rectangle<int> CContextD3D11::GetScissor() const
	{
		UINT numRects = 1;
		D3D11_RECT rect;
		m_d3dContext->RSGetScissorRects(&numRects, &rect);

		return Xe::Math::Rectangle<int>((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom);
	}

	void CContextD3D11::SetScissor(const Xe::Math::Rectangle<int>& scissor)
	{
		D3D11_RECT rect{ (LONG)scissor.left, (LONG)scissor.top, (LONG)scissor.right, (LONG)scissor.bottom };
		m_d3dContext->RSSetScissorRects(1, &rect);
	}

	void CContextD3D11::Clear(svar clearmask) {
		if (clearmask == 0)
			return;
		if (clearmask & Clear_Color)
		{
			m_d3dContext->ClearRenderTargetView(m_pBackbufferRenderTargetView, m_d3dClearColor);
		}
		if (clearmask != Clear_Color)
		{
			UINT flags = 0;
			if (clearmask & Clear_Depth)
				flags |= D3D11_CLEAR_DEPTH;
			if (clearmask & Clear_Stencil)
				flags |= D3D11_CLEAR_STENCIL;
			m_d3dContext->ClearDepthStencilView(m_pDepthStencilView,
				(UINT)flags,
				(FLOAT)m_ClearDepth,
				(UINT8)m_ClearStencil);
		}
	}

	void CContextD3D11::Draw(Primitive primitive, u32 count, u32 start)
	{
		m_d3dContext->Draw(count, start);
	}

	void CContextD3D11::DrawIndexed(Primitive primitive, u32 count, u32 start)
	{
		m_d3dContext->IASetPrimitiveTopology(GetPrimitive(primitive));
		m_d3dContext->DrawIndexed(count, start, 0);
	}


	void CContextD3D11::SwapBuffers(VBlankSync sync) {
		HRESULT hr = m_swapChain->Present(sync, 0);
		if (m_d3dContext1)
		{
			m_d3dContext1->DiscardView(m_pBackbufferRenderTargetView);
			m_d3dContext1->DiscardView(m_pDepthStencilView);
		}
		m_d3dContext->OMSetRenderTargets(1,
			&m_pBackbufferRenderTargetView, m_pDepthStencilView);
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			// Handle device lost
		}
	}

	bool CreateContextD3D11(IContext **context, const ContextInitDesc& properties) {
		CContextD3D11 *tmp = new CContextD3D11();
		if (tmp->Initialize(properties)) {
			*context = tmp;
			return true;
		}
		tmp->Release();
		return false;
	}
} }