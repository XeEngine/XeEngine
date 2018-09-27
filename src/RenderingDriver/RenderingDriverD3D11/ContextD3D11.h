#pragma once

#if WINAPI_FAMILY != WINAPI_FAMILY_TV_TITLE
#include <d3d11.h>
#include <d3d11_1.h>
#else
#include <d3d11_x.h>
#endif

#include <XeSDK/ICoreView.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsTilemap.h>
#include "XeGraphicsCommon.h"
#include "D3D11Surface.h"

using namespace RenderingDriverD3D11;

namespace Xe { namespace Graphics {
	struct ContextState
	{
		Xe::Graphics::IBuffer* VertexBuffer;
		Xe::Graphics::IBuffer* IndexBuffer;
		Xe::Graphics::ISurface* RenderSurface;
	};

	struct CContextD3D11 : public IContext {
		class CDepthStencilState : public IDepthStencilState {
		public:
			IContext *m_context;

			CDepthStencilState(IContext *context, const DepthStencilStateDesc& desc);
			~CDepthStencilState();
		};

		class CDrawing : public IDrawing2d {
			static const svar MaximumQuadsCount = 16383;

			CContextD3D11* m_context;
			ID3D11InputLayout* m_pInputLayout;
			ID3D11VertexShader* m_pVertexShader;
			ID3D11PixelShader* m_pPixelShader;
			ID3D11SamplerState* m_pSamplerState;
			IBuffer* m_pVertexBuffer;
			IBuffer* m_pIndexBuffer;
			Vertex* m_pVertex;
			uvar m_curQuadsCount;
			bool m_IsInitialized;

			inline Vertex *Get() {
				if (m_curQuadsCount < MaximumQuadsCount)
					return m_pVertex + m_curQuadsCount++ * 4;
				Flush();
				m_curQuadsCount = 0;
				return m_pVertex;
			}

		public:
			CDrawing(CContextD3D11 *context);
			~CDrawing();

			bool GetDestinationSurface(ISurface **surface);
			void SetDestinationSurface(ISurface *surface);
			void Flush();
			void DrawRectangle(const Math::Vector2f(&position)[2], const Color &color);
			void DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]);
			void DrawRectangle(const Math::Vector2f(&position)[4], const Color &color);
			void DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]);
			void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color &color);
			void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color(&color)[4]);
			void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color &color);
			void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color(&color)[4]);
			void DrawRectangle(const Math::Vector3f(&position)[4], const Color &color);
			void DrawRectangle(const Math::Vector3f(&position)[4], const Color(&color)[4]);
			void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
			void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
			void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode);
			void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
			void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode);
			void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode);
			void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode);
			void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode);
			void DrawSurface(const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode);
			void DrawSurface(const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
		};

		class CTilemap : public ITilemap {
			IContext* m_pContext;
			IDrawing2d* m_pDrawing;

			Size m_TileSize;
			Vector2f m_TileSizef;

			int m_TilesPerRow;
			Math::Rectangle<float> m_TilesetRectf;
			Vector2f m_TilesetPos;
			Vector2f m_TilesetSize;
			Vector2f m_TilesetMul;
			Vector2f m_TilesetPadding;

			Rectanglef m_Camera;

			Size m_MapSize;
			int m_ParallaxSize;
			TileData* m_Tilemap;
			float* m_Parallax;

			void SetTileset(const TilesetProperties& tileset);

			const Size& GetMapSize() const;
			void SetMapSize(const Size& size);

			void Lock(TilemapData& data);
			void Unlock();

			const Rectanglef& GetCamera() const;
			void SetCamera(const Rectanglef& camera);

			void Draw(int flags);
			void DrawStandard() const;
			void DrawFlip() const;

		public:
			CTilemap(IContext* context);
			~CTilemap();
		};

		Xe::Core::IFrameView* m_pFrameView;
		CDrawing *m_Drawing;
		ContextState m_State;
		Size m_Size;
		Color m_ClearColor;
		float m_ClearDepth;
		int m_ClearStencil;
		FLOAT m_d3dClearColor[4];
		CSurface *m_Surface[0x100];
		CDepthStencilState *m_DepthStencilState;

		IDXGIDevice1* m_dxgiDevice1;
		IDXGIAdapter* m_dxgiAdapter;
		IDXGIFactory* m_pFactory; // DXGI 1.0
		IDXGIFactory1* m_pFactory1; // DXGI 1.1
		IDXGIFactory2* m_pFactory2; // DXGI 1.2

		D3D11_VIEWPORT m_viewport;
		D3D_FEATURE_LEVEL m_FeatureLevel;

#if !_XBOX_ONE
		ID3D11Device* p_d3dDevice;
		ID3D11DeviceContext* m_d3dContext;
#else
		ID3D11DeviceX* p_d3dDevice;
		ID3D11DeviceContextX* m_d3dContext;
#endif

		// D3D11
		ID3D11InputLayout *m_inputLayout;
		ID3D11Buffer *m_matrixBuffer;
		ID3D11VertexShader* p_VShader;
		ID3D11PixelShader* p_FShader;
		// D3D11.1
		ID3D11Device1* p_d3dDevice1;
		ID3D11DeviceContext1* m_d3dContext1;
		DXGI_MODE_ROTATION m_dxgiRotation;

		DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
		DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc1;
		IDXGISwapChain* m_swapChain;
		IDXGISwapChain1* m_swapChain1;
		ID3D11Texture2D* m_pBackbufferTexture;
		ID3D11Texture2D* m_pBackbufferDepthStencil;
		ID3D11RenderTargetView* m_pBackbufferRenderTargetView;
		ID3D11DepthStencilView* m_pDepthStencilView;
			
		bool CreateFactory();
		bool CreateDevice(const ContextInitDesc& properties);
		bool CreateResources();
		bool CreateWindowSizeDependentResources();
		bool CreateResourcesForSwapchain();
		bool GetResourcesFromSwapchain();
		bool CreateDepthBuffer(UINT width, UINT height);
		void SetViewport(const Size& size);
		bool IsSdkLayersAvailable();

	public:
		CContextD3D11();
		~CContextD3D11();

		bool Initialize(const ContextInitDesc& properties);
		void GetDrawing(IDrawing2d** drawing);
		void CreateTilemap(ITilemap** pTilemap);

		void GetCapabilities(Capabilities& capabilities);

		const Color &GetClearColor() const;
		void SetClearColor(const Color &color);
		float GetClearDepth() const;
		int GetClearStencil() const;
		void SetClearStencil(int stencil);
			
		void Clear(svar clearmask);
		void Draw(Primitive primitive, u32 count, u32 start = 0);
		void DrawIndexed(Primitive primitive, u32 count, u32 start = 0);

		bool CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc);
		void SelectSurface(ISurface *surface, svar index);
		void GetRenderingSurface(ISurface** ppSurface);
		void SetRenderingSurface(ISurface* surface);

		bool CreateDepthStencilState(IDepthStencilState **depthStencilState, const DepthStencilStateDesc& desc);
		void SelectDepthStencilState(IDepthStencilState *depthStencilState);

		bool CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData);
		void SelectBuffer(IBuffer *pBuffer);
		void GetVertexBuffer(IBuffer** ppBuffer);
		void SetVertexBuffer(IBuffer* pBuffer);
		void GetIndexBuffer(IBuffer** ppBuffer);
		void SetIndexBuffer(IBuffer* pBuffer);

		void SetClearDepth(float depth);
		void SetInternalResolution(const Size& size);
		void SwapBuffers(VBlankSync sync);

#pragma region Helpers
		static D3D11_USAGE GetUsageType(UsageType usage);
		static UINT GetCpuAccess(UsageType usage);
		static D3D11_MAP GetMapType(LockType lockType);
		static D3D11_BIND_FLAG GetBufferType(BufferType usage);
		static D3D_PRIMITIVE_TOPOLOGY GetPrimitive(Primitive primitive);
#pragma endregion
	};
} }