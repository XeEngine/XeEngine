#include "pch.h"
#include <XeSDK/XeCoreFrameView.h>
#include "CFrameView.h"

namespace Xe { namespace Core {
	bool Factory(IFrameView*& pFrameView,
		const FrameViewInitDesc& desc)
	{
		auto frameView = new CFrameView();
		if (frameView->Initialize(desc))
		{
			pFrameView = frameView;
			return true;
		}

		pFrameView = nullptr;
		return false;
	}
} }