#include "pch.h"
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeGraphicsDef.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeGame/IGameDrawing2d.h>
#include <XeGame/IGameTilemap2d.h>
#include <XeGame/XeGameTilemap2d.h>
#include "CGameTilemap2d.h"

using namespace Xe;
using namespace Xe::Math;
using namespace Xe::Graphics;
using namespace Xe::Game;

CTilemap2d::CTilemap2d() :
	m_RequestTilesDelegate(nullptr),
	m_DrawDelegate(nullptr),
	m_Layer({0})
{

}

CTilemap2d::~CTilemap2d()
{
	if (m_Layer.Data)
		Xe::Memory::Free(m_Layer.Data);
}

void CTilemap2d::SetRequestTilesCallback(TilemapRequestTilesDelegate* delegate)
{
	m_RequestTilesDelegate = delegate;
}

void CTilemap2d::SetDrawCallback(TilemapDrawDelegate* delegate)
{
	m_DrawDelegate = delegate;
}

const Math::Vector2i& CTilemap2d::GetCameraSize()
{
	return m_CameraSize;
}

void CTilemap2d::SetCameraSize(const Vector2i& cameraSize)
{
	m_CameraSize = cameraSize;
}

const Math::Vector2i& CTilemap2d::GetCameraPosition()
{
	return m_CameraPosition;
}

void CTilemap2d::SetCameraPosition(const Vector2i& cameraPosition)
{
	m_CameraPosition = cameraPosition;
}

const TilemapTileSize& CTilemap2d::GetTileSize()
{
	return m_TileSize;
}

void CTilemap2d::SetTileSize(const TilemapTileSize& tileSize)
{
	m_TileSize = tileSize;
}

const TilemapBufferSize& CTilemap2d::GetBufferSize()
{
	return m_BufferSize;
}

void CTilemap2d::SetBufferSize(const TilemapBufferSize& bufferSize)
{
	if (m_BufferSize == bufferSize)
		return;

	ValidateTilesetProperties(bufferSize.x);
	ValidateTilesetProperties(bufferSize.y);

	m_BufferSize = bufferSize;
	ResizeLayer(bufferSize, m_Layer);
}

bool CTilemap2d::GetBuffer(TilemapData* layer)
{
	assert(!!layer);

	layer->Tilemap = m_Layer.Data;
	layer->Size.x = m_Layer.Size.x;
	layer->Size.y = m_Layer.Size.y;
	layer->Stride = layer->Size.x * sizeof(TileData);

	return true;
}

void CTilemap2d::SetTileset(const TilesetProperties& tileset)
{
	m_Tileset = tileset;
}

void CTilemap2d::Update(double deltaTime)
{

}

void CTilemap2d::Flush()
{
	auto requiredSizeX = Math::Min<size_t>(m_Layer.Size.x, m_CameraSize.x);
	auto requiredSizeY = Math::Min<size_t>(m_Layer.Size.y, m_CameraSize.y);

	if (requiredSizeX <= 0 || requiredSizeY <= 0 ||
		m_TileSize.x <= 0 || m_TileSize.y <= 0)
		return;

	TilemapRequestTilesArgs args;
	args.Position.x = m_CameraPosition.x / m_TileSize.x;
	args.Position.y = m_CameraPosition.y / m_TileSize.y;

	TilemapData& tilemapData = args.Destination;
	tilemapData.Tilemap = m_Layer.Data;
	tilemapData.Size = Math::Vector2i(requiredSizeX, requiredSizeY);
	tilemapData.Stride = m_Layer.Size.x * sizeof(TileData);

	TilemapArgs<TilemapRequestTilesArgs> e;
	e.Sender = this;
	e.Arguments = &args;
	(*m_RequestTilesDelegate)(e);
}

void CTilemap2d::Draw(int flags)
{
	m_DrawVertices.clear();
	m_DrawIndices.clear();

	u16 index = 0;
	float tx = (float)m_TileSize.x;
	float ty = (float)m_TileSize.y;
	int width = (m_CameraSize.x + m_TileSize.x  - 1) / m_TileSize.x;
	int height = (m_CameraSize.y + m_TileSize.y - 1) / m_TileSize.y;
	float cameraShiftX = -Math::Fmod(m_CameraPosition.x, (float)m_TileSize.x);
	float cameraShiftY = -Math::Fmod(m_CameraPosition.y, (float)m_TileSize.y);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileData tileData = m_Layer.Data[(x % m_Layer.Size.x) + (y % m_Layer.Size.y) * m_Layer.Size.x];
			u32 tile = tileData.Tile;
			if (tile > 0)
			{
				tile--;

				float fx = cameraShiftX + x * tx;
				float fy = cameraShiftY + y * ty;

				float u1 = (tile % m_Tileset.TilesPerRow) * m_TileSize.x;
				float v1 = (tile / m_Tileset.TilesPerRow) * m_TileSize.y;
				float u2 = u1 + m_TileSize.x;
				float v2 = v1 + m_TileSize.x;

				if (tileData.Rotate)
				{
					if (tileData.Flip)
					{
						std::swap(u1, u2);
					}
					if (tileData.Mirror)
					{
						std::swap(v1, v2);
					}

					m_DrawVertices.push_back({ fx, fy, u1, v1 });
					m_DrawVertices.push_back({ fx + tx, fy, u1, v2 });
					m_DrawVertices.push_back({ fx, fy + ty, u2, v1 });
					m_DrawVertices.push_back({ fx + tx, fy + ty, u2, v2 });
				}
				else
				{
					if (tileData.Mirror)
					{
						std::swap(u1, u2);
					}
					if (tileData.Flip)
					{
						std::swap(v1, v2);
					}

					m_DrawVertices.push_back({ fx, fy, u1, v1 });
					m_DrawVertices.push_back({ fx + tx, fy, u2, v1 });
					m_DrawVertices.push_back({ fx, fy + ty, u1, v2 });
					m_DrawVertices.push_back({ fx + tx, fy + ty, u2, v2 });
				}

				m_DrawIndices.push_back({ (u16)(index + 1) });
				m_DrawIndices.push_back({ (u16)(index + 0) });
				m_DrawIndices.push_back({ (u16)(index + 2) });
				m_DrawIndices.push_back({ (u16)(index + 1) });
				m_DrawIndices.push_back({ (u16)(index + 2) });
				m_DrawIndices.push_back({ (u16)(index + 3) });
				index += 4;
			}
		}
	}

	TilemapDrawArgs args;
	args.Draws.push_back(TilemapDrawList
	{
		m_DrawVertices.data(),
		m_DrawIndices.data(),
		m_DrawVertices.size(),
		m_DrawIndices.size()
	});

	TilemapArgs<TilemapDrawArgs> e;
	e.Sender = this;
	e.Arguments = &args;
	(*m_DrawDelegate)(e);
}

inline void CTilemap2d::ValidateTilesetProperties(TilemapBufferSizeType bufferSizeType)
{
	//switch (bufferSizeType)
	//{
	//case TilemapBuffer_16:
	//case TilemapBuffer_32:
	//case TilemapBuffer_64:
	//case TilemapBuffer_128:
	//case TilemapBuffer_256:
	//	break;
	//default:
	//	throw std::invalid_argument(NAMEOF(bufferSizeType)" must be in the string range");
	//}
}

void CTilemap2d::ResizeLayer(const TilemapBufferSize& size, Layer& layer)
{
	if (layer.Data)
		Xe::Memory::Free(layer.Data);

	auto stride = size.x * sizeof(TileData);
	layer.Size = size;
	layer.Data = (TileData*)Xe::Memory::Alloc(stride * size.y);
}

void Xe::Game::Factory(ITilemap2d** ppTilemap2d, IDrawing2d* pDrawing2d)
{
	*ppTilemap2d = new CTilemap2d();
}