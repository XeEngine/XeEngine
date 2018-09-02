#include "pch.h"
#include "ContextNull.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// ISurface implementation
		bool CContextNull::CreateSurface(ISurface **surface, SurfaceType type, const Size &size, Color::Format format, const DataDesc& dataDesc) {
			*surface = new CSurface(this, type, size, format);
			return *surface != nullptr;
		}
		void CContextNull::SelectSurface(ISurface *surface, svar index) {
			if (index >= 0 && index < lengthof(m_Surface) && m_Surface[index] != surface) {
				if (surface != nullptr) {
					CSurface *p;
					if (p = static_cast<CSurface*>(surface)) {
						if (p->m_context == this) {
							m_Drawing->Flush();
							if (m_Surface[index]) m_Surface[index]->Release();
							m_Surface[index] = p;
							m_Surface[index]->AddRef();
							// TODO set state
						}
					}
					// If the object does not belong to current context, then ignore it.
				}
				else {
					if (m_Surface[index]) m_Surface[index]->Release();
					m_Surface[index] = nullptr;
					// TODO clear state
				}
			}
		}

		CContextNull::CSurface::CSurface(IContext *context, SurfaceType type, const Size &size, Color::Format format) :
			ISurface(context, type, size, format),
			m_context(context) {
		}
		CContextNull::CSurface::~CSurface() {
		}
		bool CContextNull::CSurface::SubLock(DataDesc & map, LockType type)
		{
			map.data = nullptr;
			map.pitch = 0;
			return false;
		}
		void CContextNull::CSurface::SubUnlock()
		{
		}
	}
}