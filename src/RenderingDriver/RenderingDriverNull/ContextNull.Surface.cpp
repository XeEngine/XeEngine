#include "pch.h"
#include "ContextNull.h"

namespace Xe { namespace Graphics {
	bool CContextNull::CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc)
	{
		*surface = new CSurface(this, type, size, format);
		return *surface != nullptr;
	}

	void CContextNull::SelectSurface(ISurface *surface, svar index)
	{
		if (index >= 0 && index < lengthof(m_Surface) && m_Surface[index] != surface)
		{
			if (surface != nullptr)
			{
				CSurface *p;
				if (p = static_cast<CSurface*>(surface))
				{
					if (m_Surface[index]) m_Surface[index]->Release();
					m_Surface[index] = p;
					m_Surface[index]->AddRef();
					// TODO set state
				}

				// If the object does not belong to current context, then ignore it.
			}
			else if (m_Surface[index])
			{
				m_Surface[index]->Release();
				m_Surface[index] = nullptr;
				// TODO clear state
			}
		}
	}

	void CContextNull::GetRenderingSurface(ISurface** ppSurface)
	{
		if (m_State.RenderSurface) m_State.RenderSurface->AddRef();
		*ppSurface = m_State.RenderSurface;
	}

	void CContextNull::SetRenderingSurface(ISurface* surface)
	{
		if (m_State.RenderSurface) m_State.RenderSurface->Release();
		m_State.RenderSurface = surface;
	}
} }