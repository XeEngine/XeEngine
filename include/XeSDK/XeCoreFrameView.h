#pragma once
#include <XeSDK/IFrameView.h>

namespace Xe { namespace Core {

	//! \brief Used to define some properties of IView.
	struct FrameViewInitDesc {
		//! \brief The title of the application, if available.
		String Title;

		//! \brief Initial size of window, if supported.
		Graphics::Size Size;
		
		//! \brief Set if window's context can change its size.
		//! \details In Windows Store builds this value is ignored.
		bool IsResizable;
		
		//! \brief Set if the application run in full-screen.
		bool IsFullscreen;

		FrameViewInitDesc() :
			Title("XeEngine application"),
			Size(1280, 720),
			IsResizable(false),
			IsFullscreen(false)
		{ }
	};

	bool Factory(IFrameView*& pFrameView, const FrameViewInitDesc& desc);
}}