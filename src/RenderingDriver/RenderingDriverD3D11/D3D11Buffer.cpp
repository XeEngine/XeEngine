#include "pch.h"
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMemory.h>
#include <d3d11.h>
#include "ContextD3D11.h"

using namespace Xe::Graphics;
#include "D3D11Buffer.h"
#include "D3D11Utilities.h"

namespace RenderingDriverD3D11
{
	CBuffer::CBuffer(
		Xe::Graphics::IContext *pContext,
		const BufferDesc& desc,
		ID3D11Buffer *pBuffer) :
		IBuffer(pContext, desc.Usage, desc.Length, desc.Type),
		m_pBuffer(pBuffer),
		m_LockBuffer(nullptr),
		m_IsLocked(false),
		m_DirectMode(false)
	{ }

	CBuffer::~CBuffer()
	{
		m_pBuffer->Release();
	}

	ID3D11Buffer* CBuffer::GetBuffer() const
	{
		return m_pBuffer;
	}

	bool CBuffer::SubLock(DataDesc& map, LockType type)
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

	void CBuffer::SubUnlock()
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
}