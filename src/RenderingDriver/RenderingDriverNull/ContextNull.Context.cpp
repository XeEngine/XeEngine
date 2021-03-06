#include "pch.h"
#include "ContextNull.h"
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
	namespace Graphics {
		CContextNull::CContextNull() :
			m_Size(800, 480),
			m_ClearColor(Color::Black),
			m_ClearDepth(0.0f),
			m_ClearStencil(0),
			m_DepthStencilState(nullptr),
			m_ScissorEnabled(false)
		{
			Memory::Fill(&m_State, 0, sizeof(m_State));
			Memory::Fill(m_Surface, 0, sizeof(m_Surface));
		}
		CContextNull::~CContextNull() {
			for (svar i = 0; i < lengthof(m_Surface); i++)
				if (m_Surface[i]) m_Surface[i]->Release();
			if (m_DepthStencilState) m_DepthStencilState->Release();
		}

		bool CContextNull::Initialize(const ContextInitDesc& properties) {
			return true;
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

		bool CContextNull::IsScissorEnabled() const
		{
			return m_ScissorEnabled;
		}

		void CContextNull::SetScissorEnabled(bool enabled)
		{
			m_ScissorEnabled = enabled;
		}

		const Xe::Math::Rectangle<int> CContextNull::GetScissor() const
		{
			return m_Scissor;
		}

		void CContextNull::SetScissor(const Xe::Math::Rectangle<int>& scissor)
		{
			m_Scissor = scissor;
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