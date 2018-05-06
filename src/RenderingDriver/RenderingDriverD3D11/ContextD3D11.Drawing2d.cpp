#include "pch.h"
#include "ContextD3D11.h"
#include <XeSDK/XeMemory.h>

//#include "ShaderD3D11_VSDefault.h"
//#include "ShaderD3D11_FSDefault.h"

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// IDrawing2d implementation
		bool CContextD3D11::CDrawing::Query(IObject **obj, UID id) {
			switch (id) {
			case IDrawing2d::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			case IContext::ID:
				m_context->AddRef();
				*obj = m_context;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CContextD3D11::CDrawing::CDrawing(CContextD3D11 *context) :
			m_context(context),
			m_pInputLayout(nullptr),
			m_pVertexShader(nullptr),
			m_pPixelShader(nullptr),
			m_pVertexBuffer(nullptr),
			m_pVertex(new Vertex[MaximumQuadsCount * 4]),
			m_curQuadsCount(0),
			m_IsInitialized(false)
		{
			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXTURE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			HRESULT hr = context->p_d3dDevice->CreateInputLayout(vertexDesc, 3, d3d11vsDefault, sizeof(d3d11vsDefault), &m_pInputLayout);
			if (FAILED(hr)) {
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateInputLayout failed (%08X)."), hr);
				return;
			}

			hr = context->p_d3dDevice->CreateVertexShader(d3d11vsDefault, sizeof(d3d11vsDefault), nullptr, &m_pVertexShader);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create default vertex shader (%08X)."), hr);
				return;
			}
			hr = context->p_d3dDevice->CreatePixelShader(d3d11fsDefault, sizeof(d3d11fsDefault), nullptr, &m_pPixelShader);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create default pixel shader (%08X)."), hr);
				return;
			}

			D3D11_SAMPLER_DESC samplerDesc;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			samplerDesc.BorderColor[0] = 1.0f;
			samplerDesc.BorderColor[1] = 1.0f;
			samplerDesc.BorderColor[2] = 1.0f;
			samplerDesc.BorderColor[3] = 1.0f;
			samplerDesc.MinLOD = -3.402823466e+38F;
			samplerDesc.MaxLOD = +3.402823466e+38F;
			hr = context->p_d3dDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create default sampler state."));
				return;
			}

			// VERTICES BUFFER INITIALIZATION
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = (UINT)(MaximumQuadsCount * 4 * sizeof(Vertex));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			Memory::Fill(m_pVertex, 0, desc.ByteWidth);
			hr = context->p_d3dDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create vertex buffer (%08X)."), hr);
				return;
			}

			// INDICES BUFFER INITIALIZATION
			u16* indices = new u16[MaximumQuadsCount * 6];
			u32* pIndices = (u32*)indices;
			for (int i = 0, nx = 0x00000000; i < MaximumQuadsCount * 3; i += 3)
			{
				pIndices[i + 0] = nx + 0x00000001;
				pIndices[i + 1] = nx + 0x00010002;
				pIndices[i + 2] = nx + 0x00030002;

				nx += 0x00040004;
			}

			BufferDesc indexBufferDesc;
			indexBufferDesc.Usage = Usage_Static;
			indexBufferDesc.Length = MaximumQuadsCount * 6 * sizeof(u16);
			indexBufferDesc.Type = BufferType_Index;

			DataDesc indexDataDesc;
			indexDataDesc.data = pIndices;
			indexDataDesc.pitch = indexBufferDesc.Length;

			if (!m_context->CreateBuffer(&m_pIndexBuffer, indexBufferDesc, &indexDataDesc))
			{
				Logger::DebugError("Unable to create default index buffer.");
			}

			m_IsInitialized = true;
		}
		CContextD3D11::CDrawing::~CDrawing() {
			if (m_pIndexBuffer) m_pIndexBuffer->Release();
			if (m_pVertexBuffer) m_pVertexBuffer->Release();
			if (m_pSamplerState) m_pSamplerState->Release();
			if (m_pPixelShader) m_pPixelShader->Release();
			if (m_pVertexShader) m_pVertexShader->Release();
			if (m_pInputLayout) m_pInputLayout->Release();
			delete[] m_pVertex;
		}

		bool CContextD3D11::CDrawing::GetDestinationSurface(ISurface **surface) {
			return false;
		}
		void CContextD3D11::CDrawing::SetDestinationSurface(ISurface *surface) {
		}
		void CContextD3D11::CDrawing::Flush() {
			if (m_IsInitialized) {
				static const UINT start = 0;
				UINT stride = sizeof(Vertex), offsets = 0;

				// Upload new vertex buffer's content
				D3D11_MAPPED_SUBRESOURCE map;
				HRESULT hr = m_context->m_d3dContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
				if (SUCCEEDED(hr)) {
					Memory::Copy(map.pData, m_pVertex, sizeof(Vertex) * m_curQuadsCount * 4);
					m_context->m_d3dContext->Unmap(m_pVertexBuffer, 0);

					m_context->m_d3dContext->VSSetShader(m_pVertexShader, nullptr, 0);
					m_context->m_d3dContext->PSSetShader(m_pPixelShader, nullptr, 0);
					m_context->m_d3dContext->PSSetSamplers(0, 1, &m_pSamplerState);
					m_context->m_d3dContext->PSSetSamplers(1, 1, &m_pSamplerState);
					m_context->m_d3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offsets);
					m_context->SelectBuffer(m_pIndexBuffer);
					m_context->m_d3dContext->IASetInputLayout(m_pInputLayout);
					m_context->DrawIndexed(Primitive_TriangleList, m_curQuadsCount * 6, start);
				}
				else
					LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to upload new buffer's content."));
			}
			m_curQuadsCount = 0;
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, z, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector3f(&position)[4], const Color &color) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextD3D11::CDrawing::DrawRectangle(const Math::Vector3f(&position)[4], const Color(&color)[4]) {
			Common::DrawRectangle(Get(), GetMatrix(), position, color);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, z, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CContextD3D11::CDrawing::DrawSurface(const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface(Get(), GetMatrix(), position, uvCoord, color, mode);
		}

	}
}