#include "pch.h"
#include <XeSDK/XeCoreFrameView.h>
#include <XeSDK/XeObjPtr.h>
#include "CFrameView.h"

namespace Xe { namespace Core {

	bool Run(Xe::Core::IFrameHandler* pFrameHandler, const FrameViewInitDesc& desc)
	{
		struct CFrameEventHandler : IFrameEventHandler
		{
			IFrameHandler* m_pFrameHandler;

			CFrameEventHandler(IFrameHandler* pFrameHandler) :
				m_pFrameHandler(pFrameHandler)
			{ }

			void operator()(AttachEventArgs& args)
			{
				args.Succeeded = m_pFrameHandler->OnAttach(args.FrameView);
			}

			void operator()(ClosingEventArgs& args)
			{
				args.CloseApproved = m_pFrameHandler->OnClosing(args.IsCloseForced);
			}

			void operator()(const FocusChangedEventArgs& args)
			{
				if (args.IsFocused)
				{
					m_pFrameHandler->OnFocusGot();
				}
				else
				{
					m_pFrameHandler->OnFocusLost();
				}
			}

			void operator()(const VisibilityChangedEventArgs& args)
			{
				m_pFrameHandler->OnVisibilityChanged(args.IsVisible);
			}

			void operator()(const SizeChangedEventArgs& args)
			{
				m_pFrameHandler->OnSizeChanged(args.Size);
			}

			void operator()(const OrientationChangedEventArgs& args)
			{
				m_pFrameHandler->OnOrientationChanged(args.Orientation);
			}
		};

		return Run(&CFrameEventHandler(pFrameHandler), desc);
	}

	bool Run(Xe::Core::IFrameEventHandler* pFrameHandler, const FrameViewInitDesc& desc)
	{
		ASSERT(pFrameHandler);

		Xe::ObjPtr<CFrameView> pFrameView = new CFrameView(pFrameHandler);

		AttachEventArgs attach;
		attach.FrameView = pFrameView;
		(*pFrameHandler)(attach);

		if (attach.Succeeded)
		{
			return pFrameView->Initialize(desc) ? pFrameView->Run() : false;
		}
		else
		{
			LOGE("Attach failed");
			return false;
		}
	}
} }