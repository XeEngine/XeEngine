#pragma once
#include <XeSDK/IGraphicsBuffer.h>
#include <XeSDK/IGraphicsContext.h>
#include "ContextGL.Library.h"

namespace Xe { namespace Graphics {
	struct CGLBuffer : public IBuffer
	{
		static const GLint GLTARGET[];
		const GLuint m_IdBuffer;
		const GLint m_Target;

		CGLBuffer(
			Xe::Graphics::IContext* pContext,
			UsageType usage,
			int length,
			BufferType type,
			GLuint idBuffer);

		~CGLBuffer();

		bool SubLock(DataDesc& map, LockType lockType);
		void SubUnlock();
	};
} }