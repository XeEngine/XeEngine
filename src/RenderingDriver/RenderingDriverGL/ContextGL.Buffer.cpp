#include "pch.h"
#include "ContextGL.Common.h"

#ifdef PLATFORM_GL

namespace Xe { namespace Graphics {
	bool CContextGLCommon::CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData)
	{
		ASSERT(!"CreateBuffer not implemented yet.");
		return false;
	}

	void CContextGLCommon::SelectBuffer(IBuffer *pBuffer)
	{
		ASSERT(!"SelectBuffer not implemented yet.");
	}
} }

#endif