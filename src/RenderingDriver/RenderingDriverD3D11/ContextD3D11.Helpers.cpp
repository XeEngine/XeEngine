#include "pch.h"
#include "ContextD3D11.h"

namespace Xe { namespace Graphics {
	const D3D11_USAGE LUT_USAGETYPE[]
	{
		D3D11_USAGE_DEFAULT, // Usage_Standard
		D3D11_USAGE_IMMUTABLE, // Usage_Static
		D3D11_USAGE_DYNAMIC, // Usage_Dynamic
		D3D11_USAGE_STAGING, // Usage_Full
	};

	const UINT LUT_CPUACCESS[]
	{
		0, // Usage_Standard
		0, // Usage_Static
		D3D11_CPU_ACCESS_WRITE, // Usage_Dynamic
		D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, // Usage_Full
	};

	const D3D11_MAP LUT_MAPTYPE[]
	{
		D3D11_MAP_READ, // Lock_Read
		D3D11_MAP_WRITE_DISCARD, // Lock_Write
		D3D11_MAP_READ_WRITE, // Lock_ReadWrite
	};

	const D3D11_BIND_FLAG LUT_BUFFERTYPE[]
	{
		D3D11_BIND_VERTEX_BUFFER, // BufferType_Vertex
		D3D11_BIND_INDEX_BUFFER, // BufferType_Index
	};

	D3D11_USAGE CContextD3D11::GetUsageType(UsageType usage) {
		return LUT_USAGETYPE[usage];
	}

	UINT CContextD3D11::GetCpuAccess(UsageType usage) {
		return LUT_CPUACCESS[usage];
	}

	D3D11_MAP CContextD3D11::GetMapType(LockType lockType)
	{
		return LUT_MAPTYPE[lockType];
	}

	D3D11_BIND_FLAG CContextD3D11::GetBufferType(BufferType usage) {
		return LUT_BUFFERTYPE[usage];
	}
} }