#pragma once

namespace RenderingDriverD3D11
{
	D3D11_USAGE GetUsageType(UsageType usage);

	UINT GetCpuAccess(UsageType usage);

	D3D11_MAP GetMapType(LockType lockType);

	D3D11_BIND_FLAG GetBufferType(BufferType usage);

	D3D_PRIMITIVE_TOPOLOGY GetPrimitive(Primitive primitive);
}