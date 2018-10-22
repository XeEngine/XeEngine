#include "pch.h"
#include "ContextD3D11.h"
#include <XeSDK/XeString.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeLogger.h>

#if !_XBOX_ONE
#include <dxgi1_2.h>
#define MY_IID_PPV_ARGS IID_PPV_ARGS

#else
#define MY_IID_PPV_ARGS(ppType)  __uuidof(**(ppType)), (void**)(ppType)

#endif

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
		bool CContextD3D11::CreateFactory()
		{
#if !_XBOX_ONE
			if (!m_pFactory)
			{
				HRESULT hr;
				hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_pFactory2));
				if (FAILED(hr))
				{
					LOG(Log::Priority_Info, Log::Type_Graphics, _T("DXGI 1.2 not supported."));
					hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_pFactory1));

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
					// Windows Store apps does not support CreateDXGIFactory
					if (FAILED(hr)) {
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("DXGI 1.1 not supported."));
						hr = CreateDXGIFactory(IID_PPV_ARGS(&m_pFactory));
					}
					else
						m_pFactory1->QueryInterface(&m_pFactory);
#endif

					if (FAILED(hr)) {
						LOG(Log::Priority_Critical, Log::Type_Graphics, _T("Unable to create DXGI Factory."));
						return false;
					}

#if WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP
					m_pFactory1->QueryInterface(&m_pFactory);
#endif
				}
				else {
					m_pFactory2->QueryInterface(&m_pFactory1);
					m_pFactory2->QueryInterface(&m_pFactory);
				}
			}
			return true;
#else
			return false;
#endif
		}

		bool CContextD3D11::CreateDevice(const ContextInitDesc& properties) {
			/*HRESULT result;
			IDXGIFactory *factory;
			IDXGIOutput *adapterOutput;

			UINT numModes;
			DXGI_MODE_DESC *displayModeList;
			

			result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(result))
				return false;

			// Select a monitor
			result = adapter->EnumOutputs(properties.DisplayIndex, &adapterOutput);
			if (FAILED(result))
			{
				LOG(Log::Priority_Info, Log::Type_Graphics, _T("Specified screen %i not found."), properties.DisplayIndex);
				result = adapter->EnumOutputs(0, &adapterOutput);
				if (FAILED(result)) {
					LOG(Log::Priority_Warning, Log::Type_Graphics, _T("Default screen not found. It's not connected?"));
					return false;
				}
			}

			DXGI_OUTPUT_DESC displayDesc;
			adapterOutput->GetDesc(&displayDesc);
			LOG(Log::Priority_Info, Log::Type_Graphics,
				_T("Display %i %s (%i, %i, %i, %i) attached? %c\n"),
				properties.DisplayIndex,
				String(displayDesc.DeviceName).GetData(),
				displayDesc.DesktopCoordinates.left,
				displayDesc.DesktopCoordinates.top,
				displayDesc.DesktopCoordinates.right,
				displayDesc.DesktopCoordinates.bottom,
				displayDesc.AttachedToDesktop ? 'Y' : 'N');

			// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
			result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
			if (FAILED(result))
			{
				return false;
			}

			// Create a list to hold all the possible display modes for this monitor/video card combination.
			displayModeList = new DXGI_MODE_DESC[numModes];
			if (!displayModeList)
			{
				return false;
			}

			// Now fill the display mode list structures.
			result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
			if (FAILED(result))
			{
				return false;
			}

			int screenWidth = 1920;
			int screenHeight = 1080;
			int numerator, denominator;

			// Now go through all the display modes and find the one that matches the screen width and height.
			// When a match is found store the numerator and denominator of the refresh rate for that monitor.
			for (int i = 0; i<numModes; i++)
			{
				if (displayModeList[i].Width == (unsigned int)screenWidth)
				{
					if (displayModeList[i].Height == (unsigned int)screenHeight)
					{
						numerator = displayModeList[i].RefreshRate.Numerator;
						denominator = displayModeList[i].RefreshRate.Denominator;
					}
				}
			}*/

			static const D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};
			UINT creationFlags =
				D3D11_CREATE_DEVICE_SINGLETHREADED |
				D3D11_CREATE_DEVICE_BGRA_SUPPORT;
			HRESULT hr;

			IDXGIAdapter *pAdapter;
			if (!CreateFactory())
				return false;

			// Select a videocard (default one if fails)
			hr = m_pFactory->EnumAdapters((UINT)properties.VideoCardIndex, &pAdapter);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Warning, Log::Type_Graphics, _T("Specified videocard %i not found."), properties.VideoCardIndex);
				hr = m_pFactory->EnumAdapters(0, &pAdapter);
				if (FAILED(hr))
					LOG(Log::Priority_Critical, Log::Type_Graphics, _T("Unable to open a videocard."));
			}
			else
				LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Getting info from videocard %i..."), properties.VideoCardIndex);
			if (FAILED(hr))
				return false;

			// Log some info about videocard
#ifdef DEBUGLOG
			DXGI_ADAPTER_DESC adapterDesc;
			pAdapter->GetDesc(&adapterDesc);
			LOG(Log::Priority_Info, Log::Type_Graphics,
				_T("Videocard %i VEN=%04X DEV=%04X SUB=%08X REV=%02X\n%s\nVideo Memory: %iMB\nSystem Memory: %iMB\nShared Memory: %iMB\n"),
				properties.VideoCardIndex,
				adapterDesc.VendorId, adapterDesc.DeviceId, adapterDesc.SubSysId, adapterDesc.Revision,
				String(adapterDesc.Description).GetData(),
				adapterDesc.DedicatedVideoMemory / 1048576,
				adapterDesc.DedicatedSystemMemory / 1048576,
				adapterDesc.SharedSystemMemory / 1048576);
#endif
#ifdef DEBUGLOG
			LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Checking for D3D11 SDK layer..."));
			if (IsSdkLayersAvailable())
			{
				// Se il progetto si trova in una compilazione di debug, abilita il debug tramite i livelli SDK utilizzando questo flag.
				//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
				LOG(Log::Priority_Info, Log::Type_Graphics, _T("D3D11_CREATE_DEVICE_DEBUG set"));
			}
			else
				LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("No debugging features found."));
#endif
			ID3D11Device* device;
			ID3D11DeviceContext* context;
			LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Creating D3D11 device..."));

			hr = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr,
				creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
				&device, &m_FeatureLevel, &context);
			if (FAILED(hr))
			{
				// Se l'inizializzazione non riesce, eseguire il fallback al dispositivo WARP.
				// Per ulteriori informazioni su WARP, vedere:
				// http://go.microsoft.com/fwlink/?LinkId=286690
				LOG(Log::Priority_Warning, Log::Type_Graphics, _T("D3D11 Hardware not available. Creating WRAP with default device..."));

				hr = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_WARP, nullptr,
					creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
					&device, &m_FeatureLevel, &context);

				if (FAILED(hr))
					LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create a D3D11 device."));
			}

			if (pAdapter)
				pAdapter->Release();

			if (FAILED(hr))
				return false;
			LOG(Log::Priority_Info, Log::Type_Graphics, _T("D3D11 device created with success. Feature level: %04X"), m_FeatureLevel);
			p_d3dDevice = device;
			m_d3dContext = context;
			device->QueryInterface(MY_IID_PPV_ARGS(&p_d3dDevice1));
			context->QueryInterface(MY_IID_PPV_ARGS(&m_d3dContext1));

			IDXGIDevice1* dxgiDevice;
			if (SUCCEEDED(p_d3dDevice->QueryInterface(MY_IID_PPV_ARGS(&dxgiDevice))))
			{
				// Accertarsi che DXGI non metta in coda più di un frame alla volta. Ciò consente sia di ridurre la latenza sia
				// di accertarsi che l'applicazione esegua il rendering dopo ogni VSync, in modo da ridurre al minimo il consumo di energia.
				dxgiDevice->SetMaximumFrameLatency(1);
				dxgiDevice->Release();
				LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("SetMaximumFrameLatency set to 1."));
			}
			else
				LOG(Log::Priority_Warning, Log::Type_Graphics, _T("Unable to set SetMaximumFrameLatency: current device does not support DXGI 1.1."));
			return true;
		}

		bool CContextD3D11::CreateResources() {
			HRESULT hr;
			
			D3D11_DEPTH_STENCIL_DESC d3d11desc;
			ZeroMemory(&d3d11desc, sizeof(d3d11desc));
			d3d11desc.DepthEnable = TRUE;
			d3d11desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			d3d11desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			d3d11desc.StencilEnable = FALSE;
			d3d11desc.StencilReadMask = 0;
			d3d11desc.StencilWriteMask = 0;
			d3d11desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
			d3d11desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

			ID3D11DepthStencilState* pDepthStencilState = nullptr;
			hr = p_d3dDevice->CreateDepthStencilState(&d3d11desc, &pDepthStencilState);
			if (SUCCEEDED(hr)) {
				m_d3dContext->OMSetDepthStencilState(pDepthStencilState, 0);
				pDepthStencilState->Release();
			}
			else
				LOG(Xe::Debug::Log::Priority_Error,
					Xe::Debug::Log::Type_Generic,
					_T("Unable to create depth/stencil state."));

			D3D11_RASTERIZER_DESC rasterizerDesc;
			Memory::Fill(&rasterizerDesc, 0, sizeof(rasterizerDesc));
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.ScissorEnable = FALSE;

			ID3D11RasterizerState* rasterizerState;
			hr = p_d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
			if (SUCCEEDED(hr)) {
				m_d3dContext->RSSetState(rasterizerState);
				rasterizerState->Release();
			}
			else
				LOG(Xe::Debug::Log::Priority_Error,
					Xe::Debug::Log::Type_Generic,
					_T("Unable to create rasterizer state."));

			D3D11_BLEND_DESC blendStateDesc;
			Memory::Fill(&blendStateDesc, 0, sizeof(blendStateDesc));
			blendStateDesc.AlphaToCoverageEnable = FALSE;
			blendStateDesc.IndependentBlendEnable = FALSE; // Use only RenderTarget[0]
			blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			ID3D11BlendState *pBlendState;
			hr = p_d3dDevice->CreateBlendState(&blendStateDesc, &pBlendState);
			if (SUCCEEDED(hr)) {
				m_d3dContext->OMSetBlendState(pBlendState, NULL, 0xFFFFFF);
				pBlendState->Release();
			}
			else
				LOG(Xe::Debug::Log::Priority_Error,
					Xe::Debug::Log::Type_Generic,
					_T("Unable to create blend state."));
			return true;
		}
		bool CContextD3D11::CreateWindowSizeDependentResources() {
			static const DXGI_FORMAT SWAPCHAINFORMAT[] = {
				DXGI_FORMAT_R10G10B10A2_UNORM, // DX10
				DXGI_FORMAT_B8G8R8A8_UNORM, // DX9.1
				DXGI_FORMAT_R8G8B8A8_UNORM, // most compatible
				DXGI_FORMAT_UNKNOWN, // Desperate solution
			};

			const auto& frameViewSize = m_pFrameView->GetSize();

			HRESULT hr = S_FALSE;
			UINT Width = (UINT)frameViewSize.x;
			UINT Height = (UINT)frameViewSize.y;
			UINT SampleDescCount = 1;
			UINT SampleDescQuality = 0;
#if !_XBOX_ONE
			DXGI_USAGE BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;
#else
			DXGI_USAGE BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
#endif
			
			UINT BufferCount = 2;
			UINT Flags = 0; // DXGIX_SWAP_CHAIN_FLAG_QUANTIZATION_RGB_FULL
			UINT RefreshRateNumerator = 0;
			UINT RefreshRateDenominator = 0;
			DXGI_MODE_SCANLINE_ORDER ScalineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			DXGI_MODE_SCALING Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			BOOL Windowed = TRUE;
			DXGI_SWAP_EFFECT SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			/*IDXGIFactory4* pFactory4;
			if (SUCCEEDED(m_pFactory->QueryInterface(&pFactory4))) {
				SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
				pFactory4->Release();
			}
			else*/
				SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#else
			SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
#endif

			auto systemWindow = m_pFrameView->GetSystemWindow();

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
			fullScreenDesc.RefreshRate.Numerator = RefreshRateNumerator;
			fullScreenDesc.RefreshRate.Denominator = RefreshRateDenominator;
			fullScreenDesc.ScanlineOrdering = ScalineOrdering;
			fullScreenDesc.Scaling = Scaling;
			fullScreenDesc.Windowed = Windowed;

			m_swapChainDesc1.Width = Width;
			m_swapChainDesc1.Height = Height;
			m_swapChainDesc1.Format = DXGI_FORMAT_UNKNOWN;
			m_swapChainDesc1.Stereo = FALSE;
			m_swapChainDesc1.SampleDesc.Count = SampleDescCount;
			m_swapChainDesc1.SampleDesc.Quality = SampleDescQuality;
			m_swapChainDesc1.BufferUsage = BufferUsage;
			m_swapChainDesc1.BufferCount = BufferCount;
			m_swapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
			m_swapChainDesc1.SwapEffect = SwapEffect;
			m_swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
			m_swapChainDesc1.Flags = Flags;

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			if (m_pFactory2 == nullptr)
			{
				// Specifying 0, the system takes the size form window
				m_swapChainDesc.BufferDesc.Width = Width;
				m_swapChainDesc.BufferDesc.Height = Height;

				// Refresh rate is need to be specified only in full-screen
				m_swapChainDesc.BufferDesc.RefreshRate.Numerator = RefreshRateNumerator;
				m_swapChainDesc.BufferDesc.RefreshRate.Denominator = RefreshRateDenominator;
				m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_UNKNOWN;
				m_swapChainDesc.BufferDesc.ScanlineOrdering = ScalineOrdering;
				m_swapChainDesc.BufferDesc.Scaling = Scaling;

				// Antialiasing
				m_swapChainDesc.SampleDesc.Count = SampleDescCount;
				m_swapChainDesc.SampleDesc.Quality = SampleDescQuality;
				m_swapChainDesc.BufferUsage = BufferUsage;
				m_swapChainDesc.BufferCount = BufferCount;
				m_swapChainDesc.OutputWindow = (HWND)systemWindow;
				m_swapChainDesc.Windowed = Windowed;
				m_swapChainDesc.SwapEffect = SwapEffect; // Only value supported from multi-sampling
				m_swapChainDesc.Flags = Flags; // Only valid on full-screen
			}
#endif
			if (m_swapChain)
				m_swapChain->Release();

			for (uvar i = 0; i < lengthof(SWAPCHAINFORMAT); i++)
			{
				DXGI_FORMAT Format = SWAPCHAINFORMAT[i];

				// If on DXGI 1.2, try to create a IDXGISwapChain1
				if (m_pFactory2) {
					m_swapChainDesc1.Format = Format;
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
					hr = m_pFactory2->CreateSwapChainForHwnd(p_d3dDevice, (HWND)systemWindow,
						&m_swapChainDesc1, &fullScreenDesc, nullptr, &m_swapChain1);
#else
					//m_swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
					hr = m_pFactory2->CreateSwapChainForCoreWindow(p_d3dDevice, (IUnknown*)systemWindow,
						&m_swapChainDesc1, nullptr, &m_swapChain1);
#endif

					if (FAILED(hr))
					{
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("Unable to create swapchain with format %i."), Format);
					}
					else
					{
						m_swapChain1->QueryInterface(MY_IID_PPV_ARGS(&m_swapChain));
						break;
					}
				}
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
				// Windows Store apps does not support IDXGISwapChain creation
				else
				{
					m_swapChainDesc.BufferDesc.Format = Format;
					hr = m_pFactory->CreateSwapChain(p_d3dDevice, &m_swapChainDesc, &m_swapChain);
					if (FAILED(hr)) {
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("Unable to create swapchain with format %i."), Format);
					}
					else
						break;
				}
#endif
			}

			if (FAILED(hr))
			{
				LOG(Log::Priority_Critical, Log::Type_Graphics, _T("Unable to create swapchain in any specified format."));
				return false;
			}

			return CreateResourcesForSwapchain();
		}
		bool CContextD3D11::CreateResourcesForSwapchain()
		{
			if (GetResourcesFromSwapchain())
			{
				DXGI_SWAP_CHAIN_DESC desc;
				m_swapChain->GetDesc(&desc);

				if (CreateDepthBuffer(desc.BufferDesc.Width, desc.BufferDesc.Height))
				{
					m_d3dContext->OMSetRenderTargets(1,
						&m_pBackbufferRenderTargetView, m_pDepthStencilView);
					return true;
				}
			}

			return false;
		}
		bool CContextD3D11::GetResourcesFromSwapchain() {
			HRESULT hr;
			if (m_pBackbufferTexture) m_pBackbufferTexture->Release();
			hr = m_swapChain->GetBuffer(0, MY_IID_PPV_ARGS(&m_pBackbufferTexture));
			if (FAILED(hr)) {
				LOG(Log::Priority_Critical, Log::Type_Graphics, _T("Unable to get texture from swapchain."));
				if (m_swapChain1) m_swapChain1->Release();
				m_swapChain->Release();
				return false;
			}
			if (m_pBackbufferRenderTargetView) m_pBackbufferRenderTargetView->Release();
			hr = p_d3dDevice->CreateRenderTargetView(m_pBackbufferTexture, nullptr, &m_pBackbufferRenderTargetView);
			if (FAILED(hr)) {
				LOG(Log::Priority_Critical, Log::Type_Graphics, _T("Unable to create a render target view from swapchain's texture."));
				m_pBackbufferRenderTargetView->Release();
				if (m_swapChain1) m_swapChain1->Release();
				m_swapChain->Release();
				return false;
			}
			return true;
		}
		bool CContextD3D11::CreateDepthBuffer(UINT width, UINT height) {
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			if (m_pBackbufferDepthStencil) {
				m_pBackbufferDepthStencil->Release();
				m_pDepthStencilView->Release();
			}
			HRESULT hr = p_d3dDevice->CreateTexture2D(&desc, nullptr, &m_pBackbufferDepthStencil);
			if (FAILED(hr)) {
				LOG(Xe::Debug::Log::Priority_Error,
					Xe::Debug::Log::Type_Generic,
					_T("Unable to create depth/stencil buffer."));
				return false;
			}
			hr = p_d3dDevice->CreateDepthStencilView(m_pBackbufferDepthStencil, nullptr, &m_pDepthStencilView);
			if (FAILED(hr)) {
				m_pBackbufferDepthStencil->Release();
				m_pBackbufferDepthStencil = nullptr;
				LOG(Xe::Debug::Log::Priority_Error,
					Xe::Debug::Log::Type_Generic,
					_T("Unable to create depth/stencil buffer."));
				return false;
			}
			return true;
		}
		void CContextD3D11::SetViewport(const Size& size) {
			m_viewport.TopLeftX = 0;
			m_viewport.TopLeftY = 0;
			m_viewport.Width = (FLOAT)size.x;
			m_viewport.Height = (FLOAT)size.y;
			m_viewport.MinDepth = D3D11_MIN_DEPTH;
			m_viewport.MaxDepth = D3D11_MAX_DEPTH;
			m_d3dContext->RSSetViewports(1, &m_viewport);
		}
		// Dism /online /add-capability /capabilityname:Tools.Graphics.DirectX~~~~0.0.1.0
		bool CContextD3D11::IsSdkLayersAvailable() {
			HRESULT hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_NULL,       // Non è necessario creare un dispositivo hardware reale.
				0,
				D3D11_CREATE_DEVICE_DEBUG,  // Verificare i livelli SDK.
				nullptr,                    // Qualsiasi livello di funzionalità è valido.
				0,
				D3D11_SDK_VERSION,          // Impostare sempre questo su D3D11_SDK_VERSION per le applicazioni Windows Store.
				nullptr,                    // Non è necessario mantenere il riferimento dispositivo D3D.
				nullptr,                    // Non è necessario conoscere il livello di funzionalità.
				nullptr                     // Non è necessario mantenere il riferimento contesto di dispositivo D3D.
			);
			return SUCCEEDED(hr);
		}
	}
}