#include "pch.h"
#include <XeSDK/XeCoreFrameView.h>
#include <XeSDK/XeObjPtr.h>
#include "CFrameView.h"

namespace Xe { namespace Core {

	bool Run(Xe::Core::IFrameHandler* pFrameHandler, const FrameViewInitDesc& desc)
	{
		ASSERT(pFrameHandler);

		Xe::ObjPtr<CFrameView> pFrameView = new CFrameView(pFrameHandler);
		pFrameHandler->OnAttach(pFrameView.Get());
		return pFrameView->Initialize(desc) ? pFrameView->Run() : false;
	}
} }