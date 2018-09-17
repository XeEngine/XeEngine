#include "pch.h"
#include "ContextD3D11.h"
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeGraphicsColor.h>

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// ISurface implementation
		bool CContextD3D11::CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc) {
			static const D3D11_USAGE TEXTURE_TYPE_USAGE[] =
			{
				D3D11_USAGE_DEFAULT,
				D3D11_USAGE_DYNAMIC,
				D3D11_USAGE_DEFAULT,
			};
			static const UINT TEXTURE_TYPE_BIND[] =
			{
				0,
				D3D11_BIND_SHADER_RESOURCE,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			};
			static const UINT TEXTURE_TYPE_CPU[] =
			{
				0,
				D3D11_CPU_ACCESS_WRITE,
				0,
			};

			/*if (!IsFormatSupported(format, type))
				return false;
			DXGI_FORMAT d3dFormat = COLORFORMAT[format];*/

			DXGI_FORMAT d3dFormat;
			switch (format) {
			case Color::Format_Indexed8:
				d3dFormat = DXGI_FORMAT_R8_UNORM;
				break;
			case Color::Format_BGRA8888:
				d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				break;
			case Color::Format_BGRX8888:
				d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				break;
			case Color::Format_RGBA8888:
				d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				break;
			case Color::Format_RGBX8888:
				d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				break;
			default:
				return false;
			}

			D3D11_SUBRESOURCE_DATA resData;
			D3D11_SUBRESOURCE_DATA* pResData;
			if (dataDesc.data != nullptr)
			{
				resData.pSysMem = dataDesc.data;
				resData.SysMemPitch = (UINT)dataDesc.pitch;
				resData.SysMemSlicePitch = dataDesc.pitch * size.y;
				pResData = &resData;
			}
			else
				pResData = nullptr;

			HRESULT hr;
			ID3D11Resource* pTexture;
			D3D11_SRV_DIMENSION srcDimension;
			D3D11_RTV_DIMENSION dstDimension;

			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = (UINT)size.x;
			texDesc.Height = (UINT)size.y;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = d3dFormat;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DYNAMIC; //TEXTURE_TYPE_USAGE[desc.usage];
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //TEXTURE_TYPE_BIND[desc.usage];
			texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //TEXTURE_TYPE_CPU[desc.usage];
			texDesc.MiscFlags = 0;

			srcDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			dstDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			ID3D11Texture2D* _pTexture = nullptr;
			hr = p_d3dDevice->CreateTexture2D(&texDesc, pResData, &_pTexture);
			if (FAILED(hr))
			{
				LOGE("Unable to create 2D texture.");
				return false;
			}
			pTexture = _pTexture;

			ID3D11ShaderResourceView* pResView;
			CD3D11_SHADER_RESOURCE_VIEW_DESC resDesc(srcDimension, d3dFormat, 0, 1);
			hr = p_d3dDevice->CreateShaderResourceView(pTexture, &resDesc, &pResView);
			if (FAILED(hr))
			{
				LOGE("Unable to create shader resource view.");
				return false;
			}

			ID3D11RenderTargetView* renderTarget = nullptr;
			if (type == SurfaceType_Target)
			{
				// Setup the description of the render target view.
				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
				renderTargetViewDesc.Format = d3dFormat;
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;

				hr = p_d3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &renderTarget);
				if (FAILED(hr))
				{
					pResView->Release();
					pTexture->Release();
					LOGE("Unable to create render target view.");
					return false;
				}
			}
			*surface = new CSurface(this, type, size, format, pTexture, pResView, renderTarget);
			return *surface != nullptr;
		}
		void CContextD3D11::SelectSurface(ISurface *surface, svar index) {
			if (index >= 0 && index < lengthof(m_Surface) && m_Surface[index] != surface) {
				if (surface != nullptr) {
					CSurface *p;
					if (p = static_cast<CSurface*>(surface))
					{
						m_Drawing->Flush();
						if (m_Surface[index]) m_Surface[index]->Release();
						m_Surface[index] = p;
						m_Surface[index]->AddRef();
						m_d3dContext->PSSetShaderResources((UINT)index, 1, &p->m_pResourceView);
					}
					// If the object does not belong to current context, then ignore it.
				}
				else {
					if (m_Surface[index]) m_Surface[index]->Release();
					m_Surface[index] = nullptr;
					m_d3dContext->PSSetShaderResources((UINT)index, 0, NULL);
				}
			}
		}

		CContextD3D11::CSurface::CSurface(IContext *context, SurfaceType type, const Size &size, Color::Format format,
			ID3D11Resource *resource, ID3D11ShaderResourceView *resourceView, ID3D11RenderTargetView *targetView) :
			ISurface(context, type, size, format),
			m_pResource(resource),
			m_pResourceView(resourceView),
			m_pTargetView(targetView),
			m_LockBuffer(nullptr),
			m_IsLocked(false),
			m_DirectMode(false)
		{
			m_length = size.x * size.y * Xe::Graphics::Color::GetBitsPerPixel(format) / 8;

		}
		CContextD3D11::CSurface::~CSurface() {
			if (m_pTargetView) m_pTargetView->Release();
			if (m_pResourceView) m_pResourceView->Release();
			if (m_pResource) m_pResource->Release();
		}

		bool CContextD3D11::CSurface::SubLock(DataDesc & map, LockType type)
		{
			map.data = nullptr;
			map.pitch = 0;

			if (m_IsLocked)
			{
				LOGE("Unable to lock the surface: it was already locked");
				return false;
			}

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

		void CContextD3D11::CSurface::SubUnlock()
		{
			if (!m_IsLocked)
			{
				LOGE("Unable to unlock the buffer: it was not locked");
			}

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

			m_IsLocked = false;
		}
	}
}