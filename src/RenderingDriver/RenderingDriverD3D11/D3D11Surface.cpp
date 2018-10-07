#include "pch.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/XeMemory.h>

#ifdef _XBOX
#include <d3d11_x.h>
#else
#include <d3d11.h>
#endif

using namespace Xe::Graphics;
#include "D3D11Surface.h"
#include "ContextD3D11.h"
#include "D3D11Utilities.h"

namespace RenderingDriverD3D11
{
	CSurface::CSurface(
		Xe::Graphics::IContext *context,
		SurfaceType type,
		const Size &size,
		Color::Format format,
		ID3D11Resource *resource,
		ID3D11ShaderResourceView *resourceView,
		ID3D11RenderTargetView *targetView) :
		ISurface(context, type, size, format),
		m_pResource(resource),
		m_pResourceView(resourceView),
		m_pTargetView(targetView),
		m_LockBuffer(nullptr),
		m_DirectMode(false)
	{
		m_Length = size.x * size.y * Xe::Graphics::Color::GetBitsPerPixel(format) / 8;
	}

	CSurface::~CSurface()
	{
		if (m_pTargetView) m_pTargetView->Release();
		if (m_pResourceView) m_pResourceView->Release();
		if (m_pResource) m_pResource->Release();
	}

	bool CSurface::SubLock(DataDesc & map, LockType type)
	{
		map.data = nullptr;
		map.pitch = 0;

		if (m_Usage == Usage_Static)
		{
			LOGW("Unable to lock a static surface");
			return false;
		}

		ASSERT(m_LockBuffer == nullptr);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = ((CContextD3D11*)m_pContext)
			->m_d3dContext
			->Map(
				m_pResource, // pResource
				0, // Subresource
				GetMapType(type), // MapType
				0, // MapFlags
				&mappedSubresource // MappedResource
			);

		if (FAILED(hr)) {
			LOGW("Unable to lock the current buffer, trying to clone the resource (slower but it does work!).");

			map.data = m_LockBuffer = Xe::Memory::Alloc(m_Length);
			map.pitch = m_Length;
			m_DirectMode = false;
		}
		else {
			map.data = mappedSubresource.pData;
			map.pitch = mappedSubresource.DepthPitch;
			m_DirectMode = true;
		}

		return true;
	}

	void CSurface::SubUnlock()
	{
		if (!m_DirectMode)
		{
			DataDesc dataDesc;
			dataDesc.data = m_LockBuffer;
			dataDesc.pitch = m_size.x * Xe::Graphics::Color::GetBitsPerPixel(m_colorformat) / 8;

			Xe::Graphics::ISurface* tmpSurface;
			if (m_pContext->CreateSurface(&tmpSurface, m_type, m_size, m_colorformat, dataDesc))
			{
				CSurface* d3dSurface = (CSurface*)tmpSurface;

				((CContextD3D11*)m_pContext)->m_d3dContext->
					CopyResource(this->m_pResource, d3dSurface->m_pResource);
				tmpSurface->Release();
			}
			else
			{
				LOGE("Unable to create a temporary surface for undirect SubLock.");
			}

			Xe::Memory::Free(m_LockBuffer);
			m_LockBuffer = nullptr;
		}
		else
		{
			((CContextD3D11*)m_pContext)->m_d3dContext->Unmap(m_pResource, 0);
		}
	}
}