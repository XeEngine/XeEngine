#include "pch.h"
#include "ContextD3D11.h"
#include <XeSDK/XeMemory.h>

namespace Xe { namespace Graphics {
	CContextD3D11::CBuffer::CBuffer(
		IContext *pContext,
		const BufferDesc& desc,
		ID3D11Buffer *pBuffer) :
		IBuffer(pContext, desc.Usage, desc.Length, desc.Type),
		m_pBuffer(pBuffer),
		m_IsLocked(false)
	{ }

	CContextD3D11::CBuffer::~CBuffer()
	{
		m_pBuffer->Release();
	}

	ID3D11Buffer* CContextD3D11::CBuffer::GetBuffer() const
	{
		return m_pBuffer;
	}

	bool CContextD3D11::CBuffer::SubLock(DataDesc& map, LockType type)
	{
		map.data = nullptr;
		map.pitch = 0;

		if (m_IsLocked)
		{
			LOGE("Unable to lock the buffer: it was already locked");
			return false;
		}

		if (m_Usage == Usage_Static)
		{
			LOGW("Unable to lock a static buffer");
			return false;
		}

		ASSERT(m_LockBuffer == nullptr);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = ((CContextD3D11*)m_pContext)
			->m_d3dContext
			->Map(
				m_pBuffer, // pResource
				0, // Subresource
				GetMapType(type), // MapType
				0, // MapFlags
				&mappedSubresource // MappedResource
			);

		if (FAILED(hr)) {
			LOGW("Unable to lock the current buffer, trying to clone the resource (slower but it does work!).");

			map.data = m_LockBuffer = Xe::Memory::Alloc(m_length);
			map.pitch = m_length;
			m_DirectMode = false;
		}
		else {
			map.data = mappedSubresource.pData;
			map.pitch = mappedSubresource.DepthPitch;
			m_DirectMode = true;
		}

		m_IsLocked = true;
		return true;
	}

	void CContextD3D11::CBuffer::SubUnlock()
	{
		if (!m_IsLocked)
		{
			LOGE("Unable to unlock the buffer: it was not locked");
		}

		if (!m_DirectMode)
		{
			BufferDesc bufferDesc;
			bufferDesc.Usage = Usage_Standard;
			bufferDesc.Length = m_length;
			bufferDesc.Type = m_type;
			
			DataDesc dataDesc;
			dataDesc.data = m_LockBuffer;
			dataDesc.pitch = 0;

			IBuffer* pBackBuffer;
			if (m_pContext->CreateBuffer(&pBackBuffer, bufferDesc, &dataDesc))
			{
				((CContextD3D11*)m_pContext)->m_d3dContext->
					CopyResource(m_pBuffer, ((CBuffer*)pBackBuffer)->m_pBuffer);
				pBackBuffer->Release();
			}
			else
			{
				LOGE("Unable to create a temporary buffer for undirect SubLock.");
			}

			Xe::Memory::Free(m_pBuffer);
			m_pBuffer = nullptr;
		}
		else
		{
			((CContextD3D11*)m_pContext)->m_d3dContext->Unmap(m_pBuffer, 0);
		}

		m_IsLocked = false;
	}

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
		ID3D11Buffer* d3d11Buffer = ((CBuffer*)pBuffer)->GetBuffer();
		UINT stride = sizeof(Vertex);
		UINT offset = 0U;

		switch (pBuffer->GetType())
		{
		case BufferType_Vertex:
			m_d3dContext->IASetVertexBuffers(0, 1, &d3d11Buffer, &stride, &offset);
			break;
		case BufferType_Index:
			m_d3dContext->IASetIndexBuffer(d3d11Buffer, DXGI_FORMAT_R16_UINT, 0);
			break;
		}
	}
} }