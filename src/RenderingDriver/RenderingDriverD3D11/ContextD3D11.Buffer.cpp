#include "pch.h"
#include "ContextD3D11.h"
#include "D3D11Buffer.h"
#include <XeSDK/XeMemory.h>

using namespace RenderingDriverD3D11;

namespace Xe { namespace Graphics {
	bool CContextD3D11::CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData)
	{
		D3D11_BUFFER_DESC d3d11Desc = { 0 };
		d3d11Desc.ByteWidth = (UINT)desc.Length;
		d3d11Desc.Usage = GetUsageType(desc.Usage);
		d3d11Desc.BindFlags = GetBufferType(desc.Type);
		d3d11Desc.CPUAccessFlags = GetCpuAccess(desc.Usage);
		d3d11Desc.MiscFlags = 0;
		d3d11Desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA d3d11Data;
		if (pData)
		{
			d3d11Data.pSysMem = pData->data;
			d3d11Data.SysMemPitch = 0;
			d3d11Data.SysMemSlicePitch = 0;
		}

		ID3D11Buffer *pBuffer;
		HRESULT hr = p_d3dDevice->CreateBuffer(
			&d3d11Desc,
			pData ? &d3d11Data : NULL,
			&pBuffer);

		if (FAILED(hr))
		{
			*ppBuffer = nullptr;
			LOGE("D3D11::CreateBuffer: Unable to create index buffer (%08X).", hr);
			return false;
		}

		*ppBuffer = new CBuffer(this, desc, pBuffer);
		return true;
	}

	void CContextD3D11::SelectBuffer(IBuffer *pBuffer)
	{
		switch (pBuffer->GetType())
		{
		case BufferType_Vertex:
			SetVertexBuffer(pBuffer);
			break;
		case BufferType_Index16:
		case BufferType_Index32:
			SetIndexBuffer(pBuffer);
			break;
		}
	}

	void CContextD3D11::GetVertexBuffer(IBuffer** ppBuffer)
	{
		if (m_State.VertexBuffer) m_State.VertexBuffer->AddRef();
		*ppBuffer = m_State.VertexBuffer;
	}

	void CContextD3D11::SetVertexBuffer(IBuffer *pBuffer)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0U;
		if (pBuffer != nullptr)
		{
			ID3D11Buffer* d3d11Buffer = ((CBuffer*)pBuffer)->GetBuffer();
			switch (pBuffer->GetType())
			{
			case BufferType_Vertex:
				m_d3dContext->IASetVertexBuffers(0, 1, &d3d11Buffer, &stride, &offset);
				break;
			default:
				LOGE("Buffer type %i is not valid.", pBuffer->GetType());
				return;
			}
		}
		else
		{
			m_d3dContext->IASetVertexBuffers(0, 1, NULL, &stride, &offset);
		}

		if (m_State.VertexBuffer) m_State.VertexBuffer->Release();
		m_State.VertexBuffer = pBuffer;
		if (m_State.VertexBuffer) m_State.VertexBuffer->AddRef();
	}

	void CContextD3D11::GetIndexBuffer(IBuffer** ppBuffer)
	{
		if (m_State.IndexBuffer) m_State.IndexBuffer->AddRef();
		*ppBuffer = m_State.IndexBuffer;
	}

	void CContextD3D11::SetIndexBuffer(IBuffer *pBuffer)
	{
		if (pBuffer != nullptr)
		{
			ID3D11Buffer* d3d11Buffer = ((CBuffer*)pBuffer)->GetBuffer();
			switch (pBuffer->GetType())
			{
			case BufferType_Index16:
				m_d3dContext->IASetIndexBuffer(d3d11Buffer, DXGI_FORMAT_R16_UINT, 0);
				break;
			case BufferType_Index32:
				m_d3dContext->IASetIndexBuffer(d3d11Buffer, DXGI_FORMAT_R32_UINT, 0);
				break;
			default:
				LOGE("Buffer type %i is not valid.", pBuffer->GetType());
				return;
			}
		}
		else
		{
			m_d3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
		}

		if (m_State.IndexBuffer) m_State.IndexBuffer->Release();
		m_State.IndexBuffer = pBuffer;
		if (m_State.IndexBuffer) m_State.IndexBuffer->AddRef();
	}
} }