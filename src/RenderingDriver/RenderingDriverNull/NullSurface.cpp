#include "pch.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/XeMemory.h>

using namespace Xe::Graphics;
#include "NullSurface.h"
#include "ContextNull.h"

namespace RenderingDriverNull
{
	CSurface::CSurface(
		Xe::Graphics::IContext *context,
		SurfaceType type,
		const Size &size,
		Color::Format format) :
		ISurface(context, type, size, format),
		m_Data(nullptr),
		m_BackData(nullptr),
		m_CopyBack(false)
	{
		m_Length = size.x * size.y * Xe::Graphics::Color::GetBitsPerPixel(format) / 8;
		m_Data = Xe::Memory::Alloc(m_Length);
	}

	CSurface::~CSurface()
	{
		Xe::Memory::Free(m_Data);
	}

	bool CSurface::SubLock(DataDesc & map, LockType type)
	{
		map.data = nullptr;
		map.pitch = 0;

		switch (type)
		{
		case Lock_Read:
			m_BackData = Xe::Memory::Alloc(m_Length);
			Xe::Memory::Copy(m_BackData, m_Data, m_Length);
			m_CopyBack = false;
			break;
		case Lock_Write:
			m_BackData = Xe::Memory::Alloc(m_Length);
			m_CopyBack = true;
			break;
		case Lock_ReadWrite:
			map.data = m_Data;
			map.pitch = m_Length;
			m_CopyBack = false;
			break;
		}
		
		return true;
	}

	void CSurface::SubUnlock()
	{
		if (m_CopyBack)
		{
			Xe::Memory::Copy(m_Data, m_BackData, m_Length);
		}

		if (m_BackData) Xe::Memory::Free(m_BackData);
		m_BackData = nullptr;
	}
}