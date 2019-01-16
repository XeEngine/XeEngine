#include "pch.h"
#include "ContextGL.Buffer.h"
#include "ContextGL.Common.h"
#include <stdexcept>

using namespace Xe::Graphics;

const GLint CGLBuffer::GLTARGET[]
{
	GL_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER
};

CGLBuffer::CGLBuffer(
	Xe::Graphics::IContext* pContext,
	UsageType usage,
	int length,
	BufferType type,
	GLuint idBuffer) :
	IBuffer(pContext, usage, length, type),
	m_IdBuffer(idBuffer),
	m_Target(GLTARGET[type])
{ }

CGLBuffer::~CGLBuffer()
{
	glDeleteBuffers(1, &m_IdBuffer);
}

bool CGLBuffer::SubLock(DataDesc& map, LockType lockType)
{
	GLenum access;
	switch (lockType)
	{
	case Lock_Read:
		access = GL_READ_ONLY;
		break;
	case Lock_Write:
		access = GL_WRITE_ONLY;
		break;
	case Lock_ReadWrite:
		access = GL_READ_WRITE;
		break;
	default:
		throw std::invalid_argument(NAMEOF(access)" is invalid or not supported");
	}

	map.data = glMapBuffer(m_Target, access);
	map.pitch = GetLength();
	return true;
}

void CGLBuffer::SubUnlock()
{
	glUnmapBuffer(m_Target);
}

bool CContextGLCommon::CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData)
{
	GLint target;
	switch (desc.Type)
	{
	case BufferType_Vertex:
		target = GL_ARRAY_BUFFER;
		break;
	case BufferType_Index16:
	case BufferType_Index32:
		target = GL_ELEMENT_ARRAY_BUFFER;
		break;
	default:
		throw std::invalid_argument(NAMEOF(desc.Type)" is invalid or not supported");
	}

	GLint usage;
	switch (desc.Usage)
	{
	case Usage_Standard:
		usage = GL_STREAM_DRAW;
		break;
	case Usage_Static:
		usage = GL_STATIC_DRAW;
		break;
	case Usage_Dynamic:
	case Usage_Full:
		usage = GL_DYNAMIC_DRAW;
		break;
	default:
		throw std::invalid_argument(NAMEOF(desc.Usage)" is invalid or not supported");
	}

	GLuint idBuffer;
	glGenBuffers(1, &idBuffer);
	glBindBuffer(target, idBuffer);

	const GLvoid* data = pData ? pData->data : nullptr;
	glBufferData(target, desc.Length, data, usage);

	*ppBuffer = new CGLBuffer(this, desc.Usage, desc.Length, desc.Type, idBuffer);
	return true;
}

void CContextGLCommon::GetVertexBuffer(IBuffer** ppBuffer)
{
	*ppBuffer = m_VertexBuffer.Get();
}

void CContextGLCommon::SetVertexBuffer(IBuffer* pBuffer)
{
	m_VertexBuffer = dynamic_cast<CGLBuffer*>(pBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer ? m_VertexBuffer->m_IdBuffer : 0);
	glVertexAttribPointer(m_AttribPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(m_AttribTex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)12);
	glVertexAttribPointer(m_AttribCol, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)24);
}

void CContextGLCommon::GetIndexBuffer(IBuffer** ppBuffer)
{
	*ppBuffer = m_IndexBuffer.Get();
}

void CContextGLCommon::SetIndexBuffer(IBuffer* pBuffer)
{
	m_IndexBuffer = dynamic_cast<CGLBuffer*>(pBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer ? m_IndexBuffer->m_IdBuffer : 0);
}