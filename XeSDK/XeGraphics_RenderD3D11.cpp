#pragma once
#include "pch.h"/*
#include "XeGraphics_RenderD3D11.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeDebugLog.h>
#include "IGraphicsWindow.h"
#include "IGraphicsWindowsWin32.h"
#include "IGraphicsWindowsWinRT.h"
#include "XeGraphicsCommon.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Xe::Debug;

namespace Xe {
	namespace Graphics {
#if defined(_WIN32)
		class CD3D11 : public IContext {
		public:
			class CDrawing : public IDrawing2d {
			public:
				struct Vertex {
					Math::Vector2f pos;
					float u, v, p;
					Color color;
				};
			private:
				CD3D11* m_context;
				ID3D11Buffer* m_pVertexBuffer;
				ID3D11Buffer* m_pIndicesBuffer;
				Vertex *m_pVertex;
				uvar m_curQuadsCount;
			public:
				static const svar MaximumQuadsCount = 16383;

				bool Query(IObject **obj, UID id);
				CDrawing(CD3D11 *context);
				~CDrawing();

				bool GetDestinationSurface(ISurface **surface);
				void SetDestinationSurface(ISurface *surface);
				void Flush();
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color &color);
				void DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
				void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);

				inline Vertex *Get() {
					if (m_curQuadsCount < MaximumQuadsCount)
						return m_pVertex + m_curQuadsCount++ * 4;
					Flush();
					m_curQuadsCount = 0;
					return m_pVertex;
				}
			};
			class CSurface : public ISurface {
			public:
				static const UID ID = 0x38417d1a10274f69ULL;
				IContext *m_context;
				ID3D11Resource *m_pResource;
				ID3D11ShaderResourceView *m_pResourceView;
				ID3D11RenderTargetView *m_pTargetView;

				bool Query(IObject **obj, UID id);
				CSurface(IContext *context, SurfaceType type, const Size &size, Color::Format format,
					ID3D11Resource *resource, ID3D11ShaderResourceView *resourceView, ID3D11RenderTargetView *targetView);
				~CSurface();
			};
			class CDepthStencilState : public IDepthStencilState {
			public:
				static const UID ID = 0x9feb8a5bfb0e4c6aULL;
				CD3D11 *m_context;
				ID3D11DepthStencilState *m_pDepthStencilState;

				bool Query(IObject **obj, UID id);
				CDepthStencilState(CD3D11 *context, const DepthStencilStateDesc& desc);
				~CDepthStencilState();
			};
			class CRenderTarget : public IObject
			{
				static const DXGI_FORMAT FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM; // Questo è il formato più comune di catena di scambio.
				static const UINT BUFFER_COUNT = 2; // Usare il doppio buffer per ridurre al minimo la latenza.
				static const UINT FLAGS = 0;

				CD3D11 *m_pContext;
				ID3D11Device *m_pDevice;
				IDXGIDevice *m_pDXGIDevice;
				ID3D11RenderTargetView* m_pRenderTargetView;
				ID3D11DepthStencilView* m_pDepthStencilView;
			public:
				IDXGISwapChain *m_pSwapChain;
				IDXGISwapChain1 *m_pSwapChain1;

				bool Query(IObject **obj, UID id) {
					switch (id) {
					case IObject::ID:
						AddRef();
						*obj = this;
						return true;
					case IContext::ID:
						m_pContext->AddRef();
						*obj = m_pContext;
						return true;
					}
					*obj = nullptr;
					return false;
				}

				CRenderTarget(CD3D11 *pContext, ID3D11Device *device) :
					m_pContext(pContext), m_pDevice(device),
					m_pDXGIDevice(nullptr), m_pSwapChain(nullptr),
					m_pSwapChain1(nullptr),
					m_pRenderTargetView(nullptr), m_pDepthStencilView(nullptr)
				{
					m_pDevice->AddRef();
					device->QueryInterface(__uuidof(IDXGIDevice), (void **)&m_pDXGIDevice);
				}
				~CRenderTarget()
				{
					if (m_pRenderTargetView)
						m_pRenderTargetView->Release();
					if (m_pDepthStencilView)
						m_pDepthStencilView->Release();
					if (m_pSwapChain)
						m_pSwapChain->Release();
					m_pDXGIDevice->Release();
					m_pDevice->Release();
				}

				bool Create()
				{
					return Create(m_pContext->GetSize());
				}
				bool Create(const Size& size)
				{
					static const UINT sampleDescCount = 1; // Non usare il campionamento multiplo.
					static const UINT sampleDescQuality = 0;
					static const DXGI_USAGE usage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

					if (m_pSwapChain)
					{
						m_pSwapChain->Release();
						m_pSwapChain = nullptr;
					}

					HRESULT hr;
					IDXGIDevice1* pDXGIDevice1 = nullptr;
					IDXGIAdapter* pDXGIAdapter = nullptr;
					IDXGIFactory2* dxgiFactory = nullptr;
					hr = m_pDXGIDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDevice1);
					if (SUCCEEDED(hr)) {
						hr = pDXGIDevice1->GetAdapter(&pDXGIAdapter);
						pDXGIDevice1->Release();
						if (SUCCEEDED(hr)) {
							hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
							pDXGIAdapter->Release();
						}
					}
					if (SUCCEEDED(hr))
					{
						DXGI_SWAP_CHAIN_DESC1 desc;
						desc.Width = size.x;
						desc.Height = size.y;
						desc.Format = FORMAT;
						desc.Stereo = false;
						desc.SampleDesc.Count = sampleDescCount;
						desc.SampleDesc.Quality = sampleDescQuality;
						desc.BufferUsage = usage;
						desc.BufferCount = BUFFER_COUNT;
						desc.Flags = FLAGS;
						desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
						desc.Scaling = DXGI_SCALING_NONE;
						desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
#if defined(PLATFORM_WINRT)
						//IUnknown* pWindow = reinterpret_cast<IUnknown*>(((Xe::Device::Window::IWinRT*)m_pWindow)->GetCoreWindow());
						IUnknown* pWindow = m_pContext->m_Window->GetCoreWindowAsIUnknown();
						hr = dxgiFactory->CreateSwapChainForCoreWindow(m_pDXGIDevice, pWindow, &desc, nullptr, &m_pSwapChain1);
						dxgiFactory->Release();
						if (FAILED(hr))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create SwapChain. %s:%i"), __TFILE__, __LINE__);
							return false;
						}
#elif defined(PLATFORM_WIN32)
						HWND hWnd = m_pContext->m_Window->GetHwnd();
						if ((hr = dxgiFactory->CreateSwapChainForHwnd(m_pDXGIDevice, hWnd,
							&desc, nullptr, nullptr, &m_pSwapChain1)) != S_OK)
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create SwapChain. %s:%i"), __TFILE__, __LINE__);
							return false;
						}
#else
						LOG(Log::Priority_Critical, Log::Type_Generic, _T("Direct3D 11.1 not fully supported on current platform"));
						return false;
#endif
						m_pSwapChain = m_pSwapChain1;
					}
					else {
						HWND hWnd;
						m_pSwapChain1 = nullptr;
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("Unable to retrive IDXGIFactory2."));
#ifdef PLATFORM_WIN32
						// DirectX 11.0 is not used by Windows Runtime, so it's safe to get a HWND because we are in Win32 mode
						hWnd = m_pContext->m_Window->GetHwnd();
#else
						hWnd = nullptr;
#endif
						DXGI_SWAP_CHAIN_DESC desc;
						desc.BufferDesc.Width = (UINT)size.x;
						desc.BufferDesc.Height = (UINT)size.y;
						desc.BufferDesc.Format = FORMAT;
						desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
						desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
						desc.SampleDesc.Count = sampleDescCount;
						desc.SampleDesc.Quality = sampleDescQuality;
						desc.BufferUsage = usage;
						desc.BufferCount = BUFFER_COUNT;
						desc.OutputWindow = hWnd;
						desc.Windowed = true;//!m_pWindow->IsFullscreen();
						desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
						desc.Flags = FLAGS;

						IDXGIAdapter *pDXGIAdapter;
						hr = m_pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
						if (FAILED(hr))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to retrieve IDXGIAdapter."));
							return false;
						}

						IDXGIFactory* dxgiFactory;
						hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
						if (FAILED(hr))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to retrieve IDXGIFactory."));
							return false;
						}
						if ((hr = dxgiFactory->CreateSwapChain(m_pDXGIDevice, &desc, &m_pSwapChain) != S_OK))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create SwapChain."));
							return false;
						}
					}
					return CreateRenderTargetView(size) && CreateDepthStencilView(size);
				}
				bool Resize()
				{
					return Resize(m_pContext->GetSize());
				}
				bool Resize(const Size& size)
				{
#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
					LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Windows Phone does not support swapchain resize, this will be recreated."));
					if (m_pSwapChain) {
						m_pSwapChain->Release();
						m_pSwapChain = nullptr;
					}
#endif
					if (m_pSwapChain)
					{
						if (m_pRenderTargetView) {
							m_pRenderTargetView->Release();
							m_pRenderTargetView = nullptr;
						}
						if (m_pDepthStencilView) {
							m_pDepthStencilView->Release();
							m_pDepthStencilView = nullptr;
						}
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("Resizing SwapChain..."));
						HRESULT hr = m_pSwapChain->ResizeBuffers(BUFFER_COUNT, (UINT)size.x, (UINT)size.y, FORMAT, FLAGS);
						if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
						{
							LOG(Log::Priority_Warning, Log::Type_Graphics, _T("SwapChain resize failed, device needs to be reconfigured."));
							return false;
						}
						else if (hr != S_OK)
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to resize SwapChain. %s:%i"), __TFILE__, __LINE__);
							return false;
						}
						LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("SwapChain resized with success!"));
						return CreateRenderTargetView(size) && CreateDepthStencilView(size);
					}
					else
					{
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("SwapChain was null, it will be created from scratch."));
						return Create(size);
					}
				}
				void Select(ID3D11DeviceContext *pContext)
				{
					ID3D11DeviceContext1 *pContext1;
					if (SUCCEEDED(pContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&pContext1)))
					{
						// Ignorare i contenuti della destinazione di rendering.
						// Questa operazione è valida solo se i contenuti esistenti verranno interamente
						// sovrascritti. Se si usano rettangoli dirty o scroll, questa chiamata deve essere rimossa.
						pContext1->DiscardView(m_pRenderTargetView);
						// Ignorare i contenuti del depth stencil.
						pContext1->DiscardView(m_pDepthStencilView);
						pContext1->Release();
					}
					pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
				}
				void Clear(ID3D11DeviceContext *pContext, svar clearmask, float color[4], float depth, svar stencil)
				{
					if (clearmask == 0)
						return;
					if (clearmask & Clear_Color)
					{
						pContext->ClearRenderTargetView(m_pRenderTargetView, color);
					}
					if (clearmask != Clear_Color)
					{
						UINT flags = 0;
						if (clearmask & Clear_Depth)
							flags |= D3D11_CLEAR_DEPTH;
						if (clearmask & Clear_Stencil)
							flags |= D3D11_CLEAR_STENCIL;
						pContext->ClearDepthStencilView(m_pDepthStencilView, (UINT)flags, (FLOAT)depth, (UINT8)stencil);
					}
				}
			private:
				bool CreateRenderTargetView(const Size& size)
				{
					HRESULT hr;
					ID3D11Texture2D* pTexture;
					if (m_pSwapChain != nullptr)
					{
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("CreateRenderTargetView: swapchain is not null, getting its texture."));
						hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture);
						if (FAILED(hr))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateRenderTargetView: Unable to get backbuffer's texture."));
							return false;
						}
					}
					else
					{
						LOG(Log::Priority_Info, Log::Type_Graphics, _T("CreateRenderTargetView: swapchain is null, a new texture will be created."));
						CD3D11_TEXTURE2D_DESC backBufferDesc(DXGI_FORMAT_B8G8R8A8_UNORM,
							(UINT)size.x, (UINT)size.y, 1, 1, D3D11_BIND_RENDER_TARGET);
						hr = m_pDevice->CreateTexture2D(&backBufferDesc, nullptr, &pTexture);
						if (FAILED(hr))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateRenderTargetView: Unable to create backbuffer's texture."));
							return false;
						}
					}
					if (m_pRenderTargetView) m_pRenderTargetView->Release();
					hr = m_pDevice->CreateRenderTargetView(pTexture, nullptr, &m_pRenderTargetView);
					pTexture->Release();
					if (FAILED(hr))
					{
						LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateRenderTargetView: Unable to create target view."));
						return false;
					}
					return true;
				}
				bool CreateDepthStencilView(const Size& size)
				{
					HRESULT hr;
					ID3D11Texture2D* pTexture;

					LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("CreateDepthStencilView called."));
					CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT,
						(UINT)size.x, (UINT)size.y, 1, 1, D3D11_BIND_DEPTH_STENCIL);

					hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pTexture);
					if (FAILED(hr))
					{
						LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateDepthStencilView: Unable to create backbuffer's texture."));
						return false;
					}

					if (m_pDepthStencilView) m_pDepthStencilView->Release();
					CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
					hr = m_pDevice->CreateDepthStencilView(pTexture, &depthStencilViewDesc, &m_pDepthStencilView);
					pTexture->Release();
					if (FAILED(hr))
					{
						LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateRenderTargetView: Unable to create depth stencil view."));
						return false;
					}
					return true;
				}
			};
			class CBuffer : public IBuffer
			{
				svar m_stride;
				// temporarely vars that are needed from lock/unlock
				bool m_bufferMapped;
				void* m_pDataTmp;
				ID3D11Buffer* m_d3d11tmpbuffer;

				void _ReleaseTmpBuffer(ID3D11DeviceContext* context)
				{
					if (m_d3d11tmpbuffer != nullptr)
					{
						context->Unmap(m_d3d11tmpbuffer, 0);
						m_d3d11tmpbuffer->Release();
						m_d3d11tmpbuffer = nullptr;
					}
				}

			protected:
				ID3D11Buffer* m_buffer;
			public:
				bool Query(IObject **obj, UID id)
				{
					switch (id)
					{
					case IBuffer::ID:
					case IObject::ID:
						AddRef();
						*obj = this;
						return true;
					}
					*obj = nullptr;
					return false;
				}
				CBuffer(IContext* pContext, UsageType usage, svar length, BufferType type,
					ID3D11Buffer* buffer, svar stride) : IBuffer(pContext, usage, length, type),
					m_buffer(buffer),
					m_stride(stride) {
				}
				~CBuffer() {
					m_buffer->Release();
				}
				ID3D11Buffer* GetBuffer() const {
					return m_buffer;
				}
				svar GetStride() const {
					return m_stride;
				}
				bool SubLock(DataDesc& map, LockType lockType) {
					bool useDirectMode;
					bool needsDownload;

					if (m_usage == Usage_Static)
					{
						switch (lockType)
						{
						case Lock_Write:
						case Lock_ReadWrite:
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Cannot lock a static resource for writing."));
							return false;
						}
					}
					if (lockType == Lock_Read || lockType == Lock_ReadWrite)
						// buffers in D3D11 cannot be accessed directly
						// to read data; every time that locking requires
						// reading, the buffer must be downloaded.
						needsDownload = true;
					else
						needsDownload = false;
					switch (m_usage)
					{
					case Usage_Standard:
					case Usage_Static:
						useDirectMode = false;
						break;
					case Usage_Dynamic:
						useDirectMode = !needsDownload;
						break;
					case Usage_Full:
						useDirectMode = true;
						break;
					default:
						return false;
					}

					HRESULT hr;

					ID3D11DeviceContext* context = ((CD3D11*)m_pContext)->m_d3dContext;
					m_d3d11tmpbuffer = nullptr;

					// D3D11 buffers cannot be locked for reading; instead
					// we need to create a temporarely staging buffer where
					// the GPU buffer will be copied on, then mapped as
					// readable data.
					D3D11_MAPPED_SUBRESOURCE tmpMap{ nullptr };
					if (needsDownload == true)
					{
						D3D11_BUFFER_DESC desc;
						desc.ByteWidth = (UINT)m_length;
						desc.Usage = D3D11_USAGE_STAGING;
						desc.BindFlags = 0;
						desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
						desc.MiscFlags = 0;
						desc.StructureByteStride = 0;

						ID3D11Device* device;
						context->GetDevice(&device);
						if (FAILED(hr = device->CreateBuffer(&desc, nullptr, &m_d3d11tmpbuffer)))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create buffer."));
							device->Release();
							return false;
						}
						device->Release();
						context->CopyResource(m_d3d11tmpbuffer, GetBuffer());
						if (FAILED(hr = context->Map(m_d3d11tmpbuffer, 0, D3D11_MAP_READ, 0, &tmpMap)))
						{
							LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to mapping buffer for reading."));
							m_d3d11tmpbuffer->Release();
							return false;
						}
					}
					if (lockType == Lock_Read)
					{
						map.data = tmpMap.pData;
						map.pitch = m_stride;
						m_bufferMapped = false;
					}
					else
					{
						if (useDirectMode == true)
						{
							D3D11_MAPPED_SUBRESOURCE d3d11Map;
							if (FAILED(hr = context->Map(GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &d3d11Map)))
							{
								LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to mapping buffer for writing."));
								if (m_d3d11tmpbuffer != nullptr)
									m_d3d11tmpbuffer->Release();
								return false;
							}
							map.data = d3d11Map.pData;
							map.pitch = d3d11Map.RowPitch;
							//map.depthPitch = d3d11Map.DepthPitch;
							m_bufferMapped = true;
						}
						else
						{
							m_pDataTmp = Memory::Alloc(m_length);
							if (needsDownload == true)
							{
								Memory::Copy(m_pDataTmp, tmpMap.pData, m_length);
								_ReleaseTmpBuffer(context);
							}
							map.data = m_pDataTmp;
							map.pitch = m_stride;
							//map.depthPitch = m_length;
						}
					}
					return true;
				}
				void SubUnlock()
				{
					CD3D11* pContext = (CD3D11*)m_pContext;
					ID3D11DeviceContext* pD3DContext = pContext->m_d3dContext;

					_ReleaseTmpBuffer(pD3DContext);
					if (m_pDataTmp != nullptr)
					{
						IBuffer *tmpBuffer = nullptr;
						bool result = pContext->CreateBuffer(&tmpBuffer, m_type, Usage_Static,
							m_pDataTmp, m_stride, m_length / m_stride);
						if (result)
						{
							CBuffer* pBuffer = (CBuffer*)tmpBuffer;
							pD3DContext->CopyResource(GetBuffer(), pBuffer->GetBuffer());
							tmpBuffer->Release();
						}
						m_pDataTmp = Memory::Free(m_pDataTmp);
					}
					if (m_bufferMapped)
					{
						pD3DContext->Unmap(m_buffer, 0);
						m_bufferMapped = false;
					}
				}
			};

			CDrawing *m_Drawing;
			ICallback* m_pCallback;
			Size m_Size;
			Color m_ClearColor;
			float m_ClearDepth;
			int m_ClearStencil;
			CRenderTarget *m_pRenderTarget;
			CSurface *m_Surface[0x100];
			CDepthStencilState *m_DepthStencilState;
			ID3D11RasterizerState *m_pRasterizerState;

			D3D11_VIEWPORT m_viewport;
			D3D_FEATURE_LEVEL m_FeatureLevel;
			// D3D11
			ID3D11Device* p_d3dDevice;
			ID3D11DeviceContext* m_d3dContext;
			DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
			ID3D11InputLayout *m_inputLayout;
			ID3D11Buffer *m_matrixBuffer;
			ID3D11VertexShader* p_VShader;
			ID3D11PixelShader* p_FShader;
			// D3D11.1
			ID3D11Device1* p_d3dDevice1;
			ID3D11DeviceContext1* m_d3dContext1;
			DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc1;
			DXGI_MODE_ROTATION m_dxgiRotation;

#if defined(PLATFORM_WIN32)
			WindowWin32 *m_Window;
#elif defined(PLATFORM_WINRT)
			WindowWinRT *m_Window;
			Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
			Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;
#endif

			inline bool IsSdkLayersAvailable();
			bool CheckFeatureLevel(D3D_FEATURE_LEVEL featureLevel);
			bool CreateDevice();
			bool CreateResources();
			bool CreateWindowSizeDependentResources();
			bool CreateBackbuffer(const Size& size);
			bool _UpdateRasterizerState(D3D11_RASTERIZER_DESC* desc);
			void SetBackbufferResolution(const Size& size);
			void HandleDeviceLost();
			void SendMatrixToShader(const Math::Matrix4& matrix);
		public:
			bool Query(IObject **obj, UID id);
			CD3D11();
			~CD3D11();

			bool Initialize(const ContextProperties& properties);
			void GetDrawing(IDrawing2d** drawing);
			void CreateTilemap(Tilemap2d **ppTilemap) {
				*ppTilemap = nullptr;
			}
			void SetCallback(ICallback* callback);

			void SetTitle(ctstring title);
			const Size &GetSize() const;
			void SetSize(const Size &size);

			const Color &GetClearColor() const;
			void SetClearColor(const Color &color);
			float GetClearDepth() const;
			void SetClearDepth(float depth);
			int GetClearStencil() const;
			void SetClearStencil(int stencil);
			void Clear(svar clearmask);

			bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
			void SelectSurface(ISurface *surface, svar index);
			bool SubCreateBuffer(ID3D11Buffer** ppBuffer, BufferType type, UsageType usage, const void* data, svar length, svar count);
			bool SubUploadBuffer(ID3D11Buffer* ppBuffer, svar offset, svar length, svar count, const void* data);
			bool CreateBuffer(IBuffer** ppBuffer, BufferType type, UsageType usage, const void* data, svar length, svar count);
			bool UploadBuffer(IBuffer *pBuffer, svar offset, svar length, svar count, const void* data);
			void SelectBuffer(IBuffer* buffer);
			bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc);
			void SelectDepthStencilState(IDepthStencilState *depthStencilState);

			void SwapBuffers(VBlankSync sync);
			bool Process();
		};

		bool CD3D11::Query(IObject **obj, UID id) {
			switch (id) {
			case IContext::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			case ICallback::ID:
				m_pCallback->AddRef();
				*obj = this;
				return true;
			case IDrawing2d::ID:
				m_Drawing->AddRef();
				*obj = m_Drawing;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CD3D11::CD3D11() :
			m_Drawing(nullptr),
			m_pCallback(nullptr),
			m_Size(800, 480),
			m_ClearColor(Color::Black),
			m_ClearDepth(0.0f),
			m_ClearStencil(0),
			m_pRenderTarget(nullptr),
			m_DepthStencilState(nullptr),
			m_pRasterizerState(nullptr),

			m_FeatureLevel(D3D_FEATURE_LEVEL_9_1),
			p_d3dDevice(nullptr),
			m_d3dContext(nullptr),
			m_inputLayout(nullptr),
			m_matrixBuffer(nullptr),
			p_VShader(nullptr),
			p_FShader(nullptr),
			p_d3dDevice1(nullptr),
			m_d3dContext1(nullptr),
			m_Window(nullptr)
		{
			Memory::Fill(m_Surface, 0, sizeof(m_Surface));
		}
		CD3D11::~CD3D11() {
			for (svar i = 0; i < lengthof(m_Surface); i++)
				if (m_Surface[i]) m_Surface[i]->Release();
			if (m_pRenderTarget) m_pRenderTarget->Release();
			if (m_pRasterizerState) m_pRasterizerState->Release();
			if (m_DepthStencilState) m_DepthStencilState->Release();
			if (m_pCallback) m_pCallback->Release();
			if (p_FShader) p_FShader->Release();
			if (p_VShader) p_VShader->Release();
			if (m_matrixBuffer) m_matrixBuffer->Release();
			if (m_inputLayout) m_inputLayout->Release();
			if (m_Drawing) m_Drawing->Release();
			if (m_Window) m_Window->Release();
		}

		bool CD3D11::Initialize(const ContextProperties& properties) {
#if defined(PLATFORM_WIN32)
			if (!WindowWin32::Create(&m_Window, properties))
				return false;
#elif defined(PLATFORM_WINRT)
			if (!WindowWinRT::Create(&m_Window, properties))
				return false;
#else
			LOG(Log::Priority_Error, Log::Type_Graphics, _T("LOLWUT?"));
#endif
			if (!CreateDevice())
				return false;
			if (!CreateResources())
				return false;
			m_pRenderTarget = new CRenderTarget(this, p_d3dDevice);
			m_Drawing = new CDrawing(this);
			if (!CreateWindowSizeDependentResources())
				return false;
			{
				DepthStencilStateDesc desc = DepthStencilStateDesc::Default();
				IDepthStencilState *state;
				if (CreateDepthStencilState(&state, desc)) {
					SelectDepthStencilState(state);
					state->Release();
				}
			}
			SendMatrixToShader(Math::Matrix4::Identity());
			return true;
		}
		void CD3D11::GetDrawing(IDrawing2d** drawing) {
			m_Drawing->AddRef();
			*drawing = m_Drawing;
		}
		void CD3D11::SetCallback(ICallback* callback) {
			if (m_pCallback != callback) {
				if (m_pCallback != nullptr)
					m_pCallback->Release();
				m_pCallback = callback;
				m_pCallback->AddRef();
			}
		}

		void CD3D11::SetTitle(ctstring title) {
			m_Window->SetTitle(title);
		}
		const Size &CD3D11::GetSize() const {
			return m_Window->GetSize();
		}
		void CD3D11::SetSize(const Size &size) {
			if (m_Size != size) {
				if (m_Window->SetSize(size)) {
					m_Size = size;
					SetBackbufferResolution(size);
				}
			}
		}

		const Color &CD3D11::GetClearColor() const {
			return m_ClearColor;
		}
		void CD3D11::SetClearColor(const Color &color) {
			m_ClearColor = color;
		}
		float CD3D11::GetClearDepth() const {
			return m_ClearDepth;
		}
		void CD3D11::SetClearDepth(float depth) {
			m_ClearDepth = depth;
		}
		int CD3D11::GetClearStencil() const {
			return m_ClearStencil;
		}
		void CD3D11::SetClearStencil(int stencil) {
			m_ClearStencil = stencil;
		}
		void CD3D11::Clear(svar clearmask) {
			float color[4] = { m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a };
			m_pRenderTarget->Clear(m_d3dContext, clearmask, color, m_ClearDepth, m_ClearStencil);
		}

		bool CD3D11::CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc) {
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

			if (!IsFormatSupported(format, desc.type, desc.usage))
				return false;
			DXGI_FORMAT d3dFormat = COLORFORMAT[desc.format];
#ifdef DEBUGLOG
			if (d3dFormat == DXGI_FORMAT_UNKNOWN)
			{
				System::Platform::DebugPrint(_T("%s:%i ColorFormat %i not supported (BUG?)\n"),
					__TFILE__, __LINE__, desc.format);
				return false;
			}
#endif
			DXGI_FORMAT d3dFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

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
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create 2D texture."));
				return false;
			}
			pTexture = _pTexture;

			ID3D11ShaderResourceView* pResView;
			CD3D11_SHADER_RESOURCE_VIEW_DESC resDesc(srcDimension, d3dFormat, 0, 1);
			hr = p_d3dDevice->CreateShaderResourceView(pTexture, &resDesc, &pResView);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create shader resource view."));
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
					LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create render target view."));
					return false;
				}
			}

			*surface = new CSurface(this, type, size, format, pTexture, pResView, renderTarget);
			return *surface != nullptr;
		}
		void CD3D11::SelectSurface(ISurface *surface, svar index) {
			CSurface *p;
			if (index >= 0 && index < lengthof(m_Surface) && m_Surface[index] != surface) {
				if (surface != nullptr) {
					if (surface->Query((IObject**)&p, CSurface::ID)) {
						if (p->m_context == this) {
							m_Drawing->Flush();
							if (m_Surface[index] != nullptr)
								m_Surface[index]->Release();
							m_Surface[index] = p;
							m_Surface[index]->AddRef();
							m_d3dContext->PSSetShaderResources((UINT)index, 1, &p->m_pResourceView);
						}
						p->Release();
					}
				}
				else {
					// TODO clear state
				}
			}
		}

		bool CD3D11::SubCreateBuffer(ID3D11Buffer** ppBuffer, BufferType type, UsageType usage,
			const void* data, svar length, svar count) {
			if (length <= 0 || count <= 0)
				return false;
			if (type == BufferType_Index && (length != 2 && length != 4))
				return false;
			const D3D11_BIND_FLAG SPECIFIC_TYPE[] =
			{
				D3D11_BIND_VERTEX_BUFFER,
				D3D11_BIND_INDEX_BUFFER,
			};
			const D3D11_USAGE SPECIFIC_PERMISSION[] =
			{
				D3D11_USAGE_DEFAULT,
				D3D11_USAGE_IMMUTABLE,
				D3D11_USAGE_DYNAMIC,
				// I'm using DYNAMIC because STAGING cannot be bound
				D3D11_USAGE_DYNAMIC,
			};
			const UINT SPECIFIC_ACCESSFLAGS[] =
			{
				0,
				0,
				D3D11_CPU_ACCESS_WRITE,
				// DYNAMIC cannot have D3D11_CPU_ACCESS_READ flag
				D3D11_CPU_ACCESS_WRITE,
			};

			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = (UINT)(length * count);
			desc.Usage = SPECIFIC_PERMISSION[usage];
			desc.BindFlags = SPECIFIC_TYPE[type];
			desc.CPUAccessFlags = SPECIFIC_ACCESSFLAGS[usage];
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr;
			D3D11_SUBRESOURCE_DATA subresourceData;
			D3D11_SUBRESOURCE_DATA* pSubresourceData;
			if (data != nullptr)
			{
				subresourceData.pSysMem = data;
				subresourceData.SysMemPitch = 0;
				subresourceData.SysMemSlicePitch = 0;
				pSubresourceData = &subresourceData;
			}
			else
				pSubresourceData = nullptr;

			if ((hr = p_d3dDevice->CreateBuffer(&desc, pSubresourceData, ppBuffer)) != S_OK)
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create buffer."));
				return false;
			}
			return true;
		}
		bool CD3D11::SubUploadBuffer(ID3D11Buffer *pBuffer, svar offset,
			svar length, svar count, const void* data) {
			D3D11_MAPPED_SUBRESOURCE map;
			HRESULT hr = m_d3dContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to upload new buffer's content."));
				return false;
			}
			else
			{
				Memory::Copy(map.pData, data, length * count);
				m_d3dContext->Unmap(pBuffer, 0);
			}
			return true;
		}
		bool CD3D11::CreateBuffer(IBuffer** ppBuffer, BufferType type, UsageType usage,
			const void* data, svar length, svar count) {
			ID3D11Buffer* pBuffer;
			SubCreateBuffer(&pBuffer, type, usage, data, length, count);
			*ppBuffer = new CBuffer(this, usage, length * count, type, pBuffer, length);
			return true;
		}
		bool CD3D11::UploadBuffer(IBuffer *pBuffer, svar offset, svar length, svar count, const void* data)
		{
			CBuffer* pCBuffer = (CBuffer*)pBuffer;
			switch (pBuffer->GetUsage())
			{
			case Usage_Standard:
				return false;
			case Usage_Static:
				return false;
			case Usage_Dynamic:
				break;
			case Usage_Full:
				break;
			default:
				return false;
			}
			switch (pBuffer->GetType())
			{
			case BufferType_Vertex:
				break;
			case BufferType_Index:
				break;
			default:
				return false;
			}
			return SubUploadBuffer(pCBuffer->GetBuffer(), offset, length, count, data);
		}
		void CD3D11::SelectBuffer(IBuffer* buffer)
		{
			if (buffer != nullptr)
			{
				UINT offsets = 0;
				CBuffer* pBuffer = (CBuffer*)buffer;
				ID3D11Buffer* pBuffer11 = pBuffer->GetBuffer();
				UINT stride = pBuffer->GetStride();
				switch (buffer->GetType())
				{
				case BufferType_Vertex:
					m_d3dContext->IASetVertexBuffers(0, 1, &pBuffer11, &stride, &offsets);
					break;
				case BufferType_Index:
					m_d3dContext->IASetIndexBuffer(pBuffer11, stride == 2 ?
						DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
					break;
				}
			}
		}
		bool CD3D11::CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc) {
			*depthStencilState = new CDepthStencilState(this, desc);
			return *depthStencilState != nullptr;
		}
		void CD3D11::SelectDepthStencilState(IDepthStencilState *depthStencilState) {
			CDepthStencilState *p;
			if (m_DepthStencilState != depthStencilState) {
				if (depthStencilState != nullptr) {
					if (depthStencilState->Query((IObject**)&p, CDepthStencilState::ID)) {
						if (p->m_context == this) {
							m_Drawing->Flush();
							if (m_DepthStencilState != nullptr)
								m_DepthStencilState->Release();
							m_DepthStencilState = p;
							m_DepthStencilState->AddRef();
							m_d3dContext->OMSetDepthStencilState(p->m_pDepthStencilState, 0);
						}
						p->Release();
					}
				}
				else {
					m_d3dContext->OMSetDepthStencilState(nullptr, 0);
				}
			}
		}

		void CD3D11::SwapBuffers(VBlankSync sync) {

		}
		bool CD3D11::Process() {
			HRESULT hr = S_OK;
			if (m_pRenderTarget != nullptr && m_pRenderTarget->m_pSwapChain != nullptr)
				hr = m_pRenderTarget->m_pSwapChain->Present(1, 0);

			m_d3dContext->RSSetViewports(1, &m_viewport);
			m_pRenderTarget->Select(m_d3dContext);

			if (hr != S_OK)
			{
				// Se il dispositivo è stato rimosso in seguito a un aggiornamento del driver o a una disconnessione,
				// deve ricreare tutte le risorse del dispositivo.
				if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
					HandleDeviceLost();
			}
			return m_Window->Update();
		}

		bool CD3D11::IsSdkLayersAvailable() {
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
		bool CD3D11::CheckFeatureLevel(D3D_FEATURE_LEVEL featureLevel) {
			return m_FeatureLevel >= featureLevel;
		}
		bool CD3D11::CreateDevice() {
			static const UINT Adapter = 0;
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

			IDXGIFactory1 *pFactory;
			CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));

			IDXGIAdapter *pAdapter = nullptr;
			hr = pFactory->EnumAdapters(Adapter, &pAdapter);
			pFactory->Release();
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to get graphics adapter ID %i"), Adapter);
				return false;
			}
			else
				LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Got graphics adapter ID %i"), Adapter);

			LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("Checking for D3D11 SDK layer..."));
			if (IsSdkLayersAvailable())
			{
				// Se il progetto si trova in una compilazione di debug, abilita il debug tramite i livelli SDK utilizzando questo flag.
				creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
				LOG(Log::Priority_Info, Log::Type_Graphics, _T("D3D11_CREATE_DEVICE_DEBUG set"));
			}
			else
				LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("No debugging features found."));


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
			pAdapter->Release();
			if (FAILED(hr))
				return false;
			LOG(Log::Priority_Info, Log::Type_Graphics, _T("D3D11 device created with success. Feature level: %04X"), m_FeatureLevel);
			p_d3dDevice = device;
			m_d3dContext = context;
			if (m_FeatureLevel >= D3D_FEATURE_LEVEL_11_1) {
				device->QueryInterface(__uuidof(ID3D11Device1), (void**)&p_d3dDevice1);
				context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_d3dContext1);
			}
			return true;
		}
		bool CD3D11::CreateResources() {
			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXTURE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 2,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 5, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			HRESULT hr;

			hr = p_d3dDevice->CreateInputLayout(vertexDesc, 3, d3d11vsDefault, sizeof(d3d11vsDefault), &m_inputLayout);
			if (FAILED(hr)) {
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("CreateInputLayout failed (%08X)."), hr);
				return false;
			}
			m_d3dContext->IASetInputLayout(m_inputLayout);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(Xe::Math::Matrix4) * 2, D3D11_BIND_CONSTANT_BUFFER);
			hr = p_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_matrixBuffer);
			if (FAILED(hr))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create constant buffer for matrix (%08X."), hr);
				return false;
			}
			m_d3dContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

			if (FAILED(hr = p_d3dDevice->CreateVertexShader(d3d11vsDefault, sizeof(d3d11vsDefault), nullptr, &p_VShader)))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create default vertex shader (%08X)."), hr);
				return false;
			}
			if (FAILED(hr = p_d3dDevice->CreatePixelShader(d3d11fsDefault, sizeof(d3d11fsDefault), nullptr, &p_FShader)))
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create default pixel shader (%08X)."), hr);
				return false;
			}
			m_d3dContext->VSSetShader(p_VShader, nullptr, 0);
			m_d3dContext->PSSetShader(p_FShader, nullptr, 0);

			D3D11_RASTERIZER_DESC rasterizerDesc;
			Xe::Memory::Fill(&rasterizerDesc, 0, sizeof(rasterizerDesc));
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.ScissorEnable = FALSE;
			_UpdateRasterizerState(&rasterizerDesc);

			D3D11_SAMPLER_DESC samplerDesc;
			Memory::Fill(&samplerDesc, 0, sizeof(samplerDesc));
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			samplerDesc.BorderColor[0] = 1.0f;
			samplerDesc.BorderColor[1] = 1.0f;
			samplerDesc.BorderColor[2] = 1.0f;
			samplerDesc.BorderColor[3] = 1.0f;
			samplerDesc.MinLOD = -3.402823466e+38F;
			samplerDesc.MaxLOD = +3.402823466e+38F;
			ID3D11SamplerState* p_samplerDesc;
			if ((hr = p_d3dDevice->CreateSamplerState(&samplerDesc, &p_samplerDesc)) != S_OK)
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create default sampler state."));
				return false;
			}
			m_d3dContext->PSSetSamplers(0, 1, &p_samplerDesc);
			m_d3dContext->PSSetSamplers(1, 1, &p_samplerDesc);
			p_samplerDesc->Release();

			ID3D11RenderTargetView* nullViews[] = { nullptr };
			m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);

			LOG(Log::Priority_Diagnostics, Log::Type_Graphics, _T("D3D11 CreateResources ended with success!"));
			return true;
		}
		bool CD3D11::CreateWindowSizeDependentResources() {
			if (!m_pRenderTarget->Resize())
				HandleDeviceLost();
			else
			{
#if defined(PLATFORM_WINRT)
				// Impostare l'orientamento corretto per la catena di scambio e generare
				// trasformazioni di matrice 2D e 3D per il rendering nella catena di scambio ruotata.
				// Si noti che gli angoli di rotazione per le trasformazioni 2D e 3D sono diversi tra loro.
				// Ciò è dovuto alla differenza negli spazi di coordinate. Inoltre,
				// la matrice 3D viene specificata in modo esplicito per evitare errori di arrotondamento.
				//m_pRenderTarget->m_pSwapChain->SetRotation(m_dxgiRotation);
#endif
				return CreateBackbuffer(GetSize());
			}
			return true;
		}
		bool CD3D11::CreateBackbuffer(const Size& size) {
			m_viewport.TopLeftX = 0;
			m_viewport.TopLeftY = 0;
			m_viewport.Width = (FLOAT)size.x;
			m_viewport.Height = (FLOAT)size.y;
			m_viewport.MinDepth = D3D11_MIN_DEPTH;
			m_viewport.MaxDepth = D3D11_MAX_DEPTH;
			m_d3dContext->RSSetViewports(1, &m_viewport);

			m_pRenderTarget->Select(m_d3dContext); //SetRenderTarget2D(nullptr);
			return true;
		}
		bool CD3D11::_UpdateRasterizerState(D3D11_RASTERIZER_DESC* desc) {
			ID3D11RasterizerState* rasterizerState;
			HRESULT hr = p_d3dDevice->CreateRasterizerState(desc, &rasterizerState);
			if (SUCCEEDED(hr))
			{
				m_d3dContext->RSSetState(rasterizerState);
				if (m_pRasterizerState != nullptr)
					m_pRasterizerState->Release();
				m_pRasterizerState = rasterizerState;
				return true;
			}
			else
			{
				LOG(Log::Priority_Error, Log::Type_Graphics, _T("Unable to create the rasterizer state."));
				return false;
			}
		}
		void CD3D11::SetBackbufferResolution(const Size& size) {
			UINT width, height;
			const Size& windowSize = GetSize();
			width = (UINT)(size.x > 0 ? size.x : windowSize.x);
			height = (UINT)(size.y > 0 ? size.y : windowSize.y);

			ID3D11RenderTargetView* nullViews[] = { nullptr };
			m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			m_d3dContext->Flush();
			if (m_pRenderTarget->m_pSwapChain != nullptr)
			{
				if (!m_pRenderTarget->Resize(Size(width, height)))
					return;
				CreateBackbuffer(Size(width, height));
			}
		}
		void CD3D11::HandleDeviceLost() {
			CreateResources();
			CreateWindowSizeDependentResources();
		}
		void CD3D11::SendMatrixToShader(const Math::Matrix4& matrix) {
			struct Math::Matrix4 m[] = { matrix, Math::Matrix4::Identity() };
			m_d3dContext->UpdateSubresource(m_matrixBuffer, 0, nullptr, m, 0, 0);
		}

		bool CD3D11::CDrawing::Query(IObject **obj, UID id) {
			switch (id) {
			case IDrawing2d::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			case IContext::ID:
				m_context->AddRef();
				*obj = m_context;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CD3D11::CDrawing::CDrawing(CD3D11 *context) :
			m_context(context),
			m_pVertexBuffer(nullptr),
			m_pIndicesBuffer(nullptr),
			m_pVertex(new Vertex[MaximumQuadsCount * 4]),
			m_curQuadsCount(0) {
			m_context->AddRef();

			// VERTICES BUFFER INITIALIZATION
			Memory::Fill(m_pVertex, 0, MaximumQuadsCount * 4 * sizeof(Vertex));
			m_context->SubCreateBuffer(&m_pVertexBuffer, BufferType_Vertex, Usage_Dynamic, m_pVertex, sizeof(Vertex), MaximumQuadsCount * 4);
			// INDICES BUFFER INITIALIZATION
			u16* indices = new unsigned short[MaximumQuadsCount * 6];
			u32* pIndices = (unsigned int*)indices;
			for (int i = 0, nx = 0x00000000; i < MaximumQuadsCount * 3; i += 3)
			{
				pIndices[i + 0] = nx + 0x00000001;
				pIndices[i + 1] = nx + 0x00010002;
				pIndices[i + 2] = nx + 0x00030002;

				nx += 0x00040004;
			}
			m_context->SubCreateBuffer(&m_pIndicesBuffer, BufferType_Index, Usage_Dynamic, m_pVertex, sizeof(u16), MaximumQuadsCount * 6);
			m_context->SubUploadBuffer(m_pIndicesBuffer, 0, sizeof(u16), MaximumQuadsCount * 6, indices);
			delete[] indices;
		}
		CD3D11::CDrawing::~CDrawing() {
			Flush();
			delete[] m_pVertex;
			if (m_pIndicesBuffer) m_pIndicesBuffer->Release();
			if (m_pVertexBuffer) m_pVertexBuffer->Release();
			m_context->Release();
		}

		bool CD3D11::CDrawing::GetDestinationSurface(ISurface **surface) {
			return false;
		}
		void CD3D11::CDrawing::SetDestinationSurface(ISurface *surface) {}
		void CD3D11::CDrawing::Flush() {
			static const UINT start = 0;
			if (m_curQuadsCount > 0) {
				UINT stride = sizeof(Vertex), offsets = 0;
				m_context->SubUploadBuffer(m_pVertexBuffer, 0, sizeof(Vertex), m_curQuadsCount * 4, m_pVertex);
				m_context->m_d3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offsets);
				m_context->m_d3dContext->IASetIndexBuffer(m_pIndicesBuffer, DXGI_FORMAT_R16_UINT, 0);
				m_context->m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				m_context->m_d3dContext->DrawIndexed((UINT)(m_curQuadsCount * 6), start, 0);
				m_curQuadsCount = 0;
			}
		}
		void CD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color &color) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}
		void CD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}
		void CD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color &color) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}
		void CD3D11::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]) {
			Common::DrawRectangle((Common::Vertex*)Get(), GetMatrix(), position, color);
		}

		void CD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}
		void CD3D11::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
			Common::DrawSurface((Common::Vertex*)Get(), GetMatrix(), position, uvCoord, color, mode);
		}

		bool CD3D11::CSurface::Query(IObject **obj, UID id) {
			switch (id) {
			case CSurface::ID:
			case ISurface::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CD3D11::CSurface::CSurface(IContext *context, SurfaceType type, const Size &size, Color::Format format,
			ID3D11Resource *resource, ID3D11ShaderResourceView *resourceView, ID3D11RenderTargetView *targetView) :
			ISurface(type, size, format),
			m_context(context),
			m_pResource(resource),
			m_pResourceView(resourceView),
			m_pTargetView(targetView)
		{
			m_context->AddRef();
		}
		CD3D11::CSurface::~CSurface() {
			if (m_pTargetView) m_pTargetView->Release();
			if (m_pResourceView) m_pResourceView->Release();
			if (m_pResource) m_pResource->Release();
			m_context->Release();
		}

		bool CD3D11::CDepthStencilState::Query(IObject **obj, UID id) {
			switch (id) {
			case CDepthStencilState::ID:
			case IDepthStencilState::ID:
			case IObject::ID:
				AddRef();
				*obj = this;
				return true;
			}
			*obj = nullptr;
			return false;
		}
		CD3D11::CDepthStencilState::CDepthStencilState(CD3D11 *context, const DepthStencilStateDesc& desc) :
			m_context(context),
			m_pDepthStencilState(nullptr) {

			static const D3D11_COMPARISON_FUNC COMPARISON[] =
			{
				D3D11_COMPARISON_NEVER,
				D3D11_COMPARISON_LESS,
				D3D11_COMPARISON_EQUAL,
				D3D11_COMPARISON_LESS_EQUAL,
				D3D11_COMPARISON_GREATER,
				D3D11_COMPARISON_NOT_EQUAL,
				D3D11_COMPARISON_GREATER_EQUAL,
				D3D11_COMPARISON_ALWAYS,
			};

			D3D11_DEPTH_STENCIL_DESC d3d11desc;
			ZeroMemory(&d3d11desc, sizeof(d3d11desc));
			d3d11desc.DepthEnable = (BOOL)desc.depthTestEnabled;
			d3d11desc.DepthWriteMask = desc.depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			d3d11desc.DepthFunc = COMPARISON[desc.depthComparison];
			d3d11desc.StencilEnable = (BOOL)desc.stencilTestEnabled;
			d3d11desc.StencilReadMask = (UINT8)desc.stencilMask;
			d3d11desc.StencilWriteMask = (UINT8)desc.stencilMask;
			d3d11desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.FrontFace.StencilFunc = COMPARISON[desc.stencilFrontFaceComp];
			d3d11desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			d3d11desc.BackFace.StencilFunc = COMPARISON[desc.stencilBackFaceComp];

			ID3D11DepthStencilState* pDepthStencilState = nullptr;
			HRESULT hr = context->p_d3dDevice->CreateDepthStencilState(&d3d11desc, &m_pDepthStencilState);
		}
		CD3D11::CDepthStencilState::~CDepthStencilState() {
			m_pDepthStencilState->Release();
		}
#endif

		bool CreateContextD3D11(IContext **context, const ContextProperties& properties) {
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WINAPP) || defined(PLATFORM_WINPHONE) || defined(PLATFORM_WINUNIVERSAL)
			*context = nullptr; 
			CD3D11 *tmp = new CD3D11;
			if (tmp->Initialize(properties)) {
				*context = tmp;
				return true;
			}
			tmp->Release();
			return false;
#else
			*context = nullptr;
			return false;
#endif
		}
	}
}*/