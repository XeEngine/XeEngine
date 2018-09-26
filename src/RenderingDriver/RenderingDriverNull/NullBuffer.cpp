#include "pch.h"
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMemory.h>
#include "NullBuffer.h"

using namespace Xe::Graphics;

namespace RenderingDriverNull
{
	CBuffer::CBuffer(
		Xe::Graphics::IContext* pContext,
		Xe::Graphics::UsageType usage,
		svar length,
		Xe::Graphics::BufferType type) :
		Base(pContext, usage, length, type),
		m_Data(nullptr),
		m_BackData(nullptr),
		m_CopyBack(false)
	{
		m_Data = Xe::Memory::Alloc(length);
	}
	
	CBuffer::~CBuffer()
	{
		Xe::Memory::Free(m_Data);
	}

	bool CBuffer::SubLock(
		Xe::Graphics::DataDesc& map,
		Xe::Graphics::LockType type)
	{
		switch (type)
		{
		case Lock_Read:
			m_BackData = Xe::Memory::Alloc(m_length);
			Xe::Memory::Copy(m_BackData, m_Data, m_length);
			m_CopyBack = false;
			break;
		case Lock_Write:
			m_BackData = Xe::Memory::Alloc(m_length);
			m_CopyBack = true;
			break;
		case Lock_ReadWrite:
			map.data = m_Data;
			map.pitch = m_length;
			m_CopyBack = false;
			break;
		}

		return true;
	}

	void CBuffer::SubUnlock()
	{
		if (m_CopyBack)
		{
			Xe::Memory::Copy(m_Data, m_BackData, m_length);
		}

		if (m_BackData) Xe::Memory::Free(m_BackData);
		m_BackData = nullptr;
	}
}