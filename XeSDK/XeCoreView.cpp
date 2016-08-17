#include "pch.h"
#include <XeSDK/ICoreView.h>

namespace Xe {
	namespace Core {
		bool IView::DispatchEvents(DispatchType type) {
			return m_FrameView->DispatchEvents(type);
		}
		void IView::SetTitle(const String& title) {
			m_FrameView->SetTitle(title);
		}
		Graphics::Size IView::GetSize() const {
			return m_FrameView->GetSize();
		}
		bool IView::SetSize(const Graphics::Size& size) {
			return m_FrameView->SetSize(size);
		}
		Xe::Graphics::Orientation IView::GetOrientation() const {
			return m_FrameView->GetOrientation();
		}
		void IView::SetPreferredOrientation(Xe::Graphics::Orientation orientation) {
			m_FrameView->SetPreferredOrientation(orientation);
		}
		float IView::GetScale() const {
			return m_FrameView->GetScale();
		}
	}
}