#include "pch.h"
#include "ContextGL.Common.h"

#ifdef PLATFORM_GL

namespace Xe { namespace Graphics {
	bool CContextGLCommon::CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData)
	{
		return false;
	}

	void CContextGLCommon::SelectBuffer(IBuffer *pBuffer)
	{

	}
} }

#endif