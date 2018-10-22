#include "pch.h"
#include <d3d11.h>
#include <XeSDK/IGraphicsContext.h>

using namespace Xe::Graphics;
#include "D3D11Utilities.h"

namespace RenderingDriverD3D11
{
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

	const D3D_PRIMITIVE_TOPOLOGY LUT_PRIMITIVE[]
	{
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST, // Primitive_PointList,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST, // Primitive_LineList,
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, // Primitive_LineStrip,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, // Primitive_TriangleList,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, // Primitive_TriangleStrip
	};

	D3D11_USAGE GetUsageType(UsageType usage)
	{
		return LUT_USAGETYPE[usage];
	}

	UINT GetCpuAccess(UsageType usage)
	{
		return LUT_CPUACCESS[usage];
	}

	D3D11_MAP GetMapType(LockType lockType)
	{
		return LUT_MAPTYPE[lockType];
	}

	D3D11_BIND_FLAG GetBufferType(BufferType usage)
	{
		return LUT_BUFFERTYPE[usage];
	}

	D3D_PRIMITIVE_TOPOLOGY GetPrimitive(Primitive primitive)
	{
		return LUT_PRIMITIVE[primitive];
	}
}