#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/IGraphicsSurface.h>
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IFrameView.h>
#include "ContextGL.Common.h"

namespace Xe { namespace Graphics {
	class CContextGL : public CContextGLCommon
	{
		Size m_Size;

		virtual void SwapBuffersSub(VBlankSync sync) = 0;

	public:
		CContextGL(Xe::Core::IFrameView* pView);
		~CContextGL();

		virtual bool Initialize(const ContextInitDesc& properties);

		void GetCapabilities(Capabilities& capabilities);

		void SetClearDepth(float depth);
		void SwapBuffers(VBlankSync sync);
	};
} }
