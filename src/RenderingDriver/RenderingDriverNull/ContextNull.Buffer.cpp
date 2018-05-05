#include "pch.h"
#include "ContextNull.h"

namespace Xe { namespace Graphics {
	bool CContextNull::CreateBuffer(IBuffer **ppBuffer, const BufferDesc& desc, DataDesc* pData) {
		return false;
	}

	void CContextNull::SelectBuffer(IBuffer *pBuffer) {}
	
} }