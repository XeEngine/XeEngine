#include "pch.h"
#include "ContextNull.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsDrawing2d.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
	namespace Graphics {
		CContextNull::CContextNull() :
			m_Drawing(nullptr),
			m_Size(800, 480),
			m_ClearColor(Color::Black),
			m_ClearDepth(0.0f),
			m_ClearStencil(0),
			m_DepthStencilState(nullptr)
		{
			Memory::Fill(&m_State, 0, sizeof(m_State));
			Memory::Fill(m_Surface, 0, sizeof(m_Surface));
		}
		CContextNull::~CContextNull() {
			for (svar i = 0; i < lengthof(m_Surface); i++)
				if (m_Surface[i]) m_Surface[i]->Release();
			if (m_DepthStencilState) m_DepthStencilState->Release();
			if (m_Drawing) m_Drawing->Release();
		}

		bool CContextNull::Initialize(const ContextInitDesc& properties) {
			m_Drawing = new CDrawing(this);
			return true;
		}
		void CContextNull::GetDrawing(IDrawing2d** drawing) {
			m_Drawing->AddRef();
			*drawing = m_Drawing;
		}
		void CContextNull::CreateTilemap(ITilemap** tilemap) {
			*tilemap = new CTilemap(this);
		}

		void CContextNull::GetCapabilities(Capabilities& capabilities)
		{
			capabilities.MaxTexture1DSize = 0;
			capabilities.MaxTexture1DArrayLength = 0;
			capabilities.MaxTexture2DSize = 0;
			capabilities.MaxTexture2DArrayLength = 0;
			capabilities.MaxTexture3DSize = 0;
			capabilities.MaxTextureCubeSize = 0;
			capabilities.MaxTextureUnits = 0;
			capabilities.MaxAntisotropy = 0;
		}

		const Color &CContextNull::GetClearColor() const {
			return m_ClearColor;
		}
		void CContextNull::SetClearColor(const Color &color) {
			m_ClearColor = color;
		}
		float CContextNull::GetClearDepth() const {
			return m_ClearDepth;
		}
		void CContextNull::SetClearDepth(float depth) {
			m_ClearDepth = depth;
		}
		int CContextNull::GetClearStencil() const {
			return m_ClearStencil;
		}
		void CContextNull::SetClearStencil(int stencil) {
			m_ClearStencil = stencil;
		}

		void CContextNull::Clear(svar clearmask) {

		}

		void CContextNull::Draw(Primitive primitive, u32 count, u32 start) {}
		void CContextNull::DrawIndexed(Primitive primitive, u32 count, u32 start) {}

		void CContextNull::SwapBuffers(VBlankSync sync) {

		}
		void CContextNull::SetInternalResolution(const Size &size) {

		}

		bool CreateContextNull(IContext **context, const ContextInitDesc& properties) {
			CContextNull *tmp = new CContextNull;
			if (tmp->Initialize(properties)) {
				*context = tmp;
				return true;
			}
			tmp->Release();
			return false;
		}
	}
}