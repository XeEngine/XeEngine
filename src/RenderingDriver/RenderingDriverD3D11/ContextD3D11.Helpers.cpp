#include "pch.h"
#include "ContextD3D11.h"

using namespace Xe::Graphics;
#include "D3D11Utilities.h"

namespace Xe { namespace Graphics {
	D3D11_USAGE CContextD3D11::GetUsageType(UsageType usage)
	{
		return RenderingDriverD3D11::GetUsageType(usage);
	}

	UINT CContextD3D11::GetCpuAccess(UsageType usage)
	{
		return RenderingDriverD3D11::GetCpuAccess(usage);
	}

	D3D11_MAP CContextD3D11::GetMapType(LockType lockType)
	{
		return RenderingDriverD3D11::GetMapType(lockType);
	}

	D3D11_BIND_FLAG CContextD3D11::GetBufferType(BufferType usage)
	{
		return RenderingDriverD3D11::GetBufferType(usage);
	}

	D3D_PRIMITIVE_TOPOLOGY CContextD3D11::GetPrimitive(Primitive primitive)
	{
		return RenderingDriverD3D11::GetPrimitive(primitive);
	}
} }