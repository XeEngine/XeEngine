#include "pch.h"
#include "ContextNull.h"
#include "NullBuffer.h"

using namespace RenderingDriverNull;

namespace Xe { namespace Graphics {
	bool CContextNull::CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData)
	{
		*ppBuffer = new CBuffer(this, desc.Usage, desc.Length, desc.Type);
		return true;
	}

	void CContextNull::GetVertexBuffer(IBuffer** ppBuffer)
	{
		if (m_State.VertexBuffer) m_State.VertexBuffer->AddRef();
		*ppBuffer = m_State.VertexBuffer;
	}

	void CContextNull::SetVertexBuffer(IBuffer* pBuffer)
	{
		if (m_State.VertexBuffer) m_State.VertexBuffer->Release();
		m_State.VertexBuffer = pBuffer;
		if (m_State.VertexBuffer) m_State.VertexBuffer->AddRef();
	}

	void CContextNull::GetIndexBuffer(IBuffer** ppBuffer)
	{
		if (m_State.IndexBuffer) m_State.IndexBuffer->AddRef();
		*ppBuffer = m_State.IndexBuffer;
	}

	void CContextNull::SetIndexBuffer(IBuffer* pBuffer)
	{
		if (m_State.IndexBuffer) m_State.IndexBuffer->Release();
		m_State.IndexBuffer = pBuffer;
		if (m_State.IndexBuffer) m_State.IndexBuffer->AddRef();
	}
} }