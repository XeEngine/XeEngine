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
	m_Timer(0.0),
	m_Layer({ 0, {0, 0}, true })
{
}

CTilemap2d::~CTilemap2d()
{
	if (m_Layer.Data) Xe::Memory::Free(m_Layer.Data);
	m_DrawVertices.Release();
	m_DrawIndices.Release();
}

void CTilemap2d::SetRequestTilesCallback(TilemapRequestTilesDelegate* delegate)
{
	m_RequestTilesDelegate = delegate;
}

void CTilemap2d::SetDrawCallback(TilemapDrawDelegate* delegate)
{
	m_DrawDelegate = delegate;
}

const Xe::Graphics::Color& CTilemap2d::GetBackgroundColor() const
{
	return m_BgColor;
}

void CTilemap2d::SetBackgroundColor(const Xe::Graphics::Color& color)
{
	m_BgColor = color;
}

const Math::Vector2i& CTilemap2d::GetCameraSize() const
{
	return m_CameraSize;
}

void CTilemap2d::SetCameraSize(const Vector2i& cameraSize)
{
	m_CameraSize = cameraSize;
}

const Math::Vector2f& CTilemap2d::GetCameraPosition() const
{
	return m_CameraPosition;
}

void CTilemap2d::SetCameraPosition(const Vector2f& cameraPosition)
{
	m_CameraPosition = cameraPosition;
}

const Xe::Math::Vector2i& CTilemap2d::GetTileSize() const
{
	return m_TileSize;
}

void CTilemap2d::SetTileSize(const Xe::Math::Vector2i& tileSize)
{
	m_TileSize = tileSize;
}

bool CTilemap2d::GetTileSequence(TileData tile, std::vector<TileFrame>& frames) const
{
	for (auto sequence : m_AnimatedTiles)
	{
		if (sequence.TileId.Tile == tile.Tile)
		{
			frames.clear();
			frames.resize(sequence.Sequence.Count);
			for (size_t i = 0; i < sequence.Sequence.Count; ++i)
			{
				frames[i] = sequence.Sequence.Data[i];
			}

			return true;
		}
	}

	return false;
}

void CTilemap2d::AddTileSequence(TileData tile, const Xe::Collections::Array<TileFrame>& frames)
{
	bool found = false;
	auto it = m_AnimatedTiles.begin();
	for (; it != m_AnimatedTiles.end(); ++it)
	{
		if ((*it).TileId == tile.Tile)
		{
			found = true;
			break;
		}
	}

	if (found == false)
	{
		m_AnimatedTiles.push_back(TileSequence());
		it = std::prev(m_AnimatedTiles.end());
	}

	auto& sequence = *it;
	sequence.TileId = tile;
	sequence.Speed = 1.0;
	sequence.Sequence.Clear();
	sequence.Sequence.Reserve(frames.GetLength());

	for (size_t i = 0; i < sequence.Sequence.Count; ++i)
	{
		sequence.Sequence.Data[i] = frames[i];
	}
}

void CTilemap2d::RemoveTileSequence(TileData tile)
{
	for (auto it = m_AnimatedTiles.begin(); it != m_AnimatedTiles.end(); ++it)
	{
		if ((*it).TileId.Tile == tile.Tile)
		{
			m_AnimatedTiles.erase(it);
			break;
		}
	}
}

const Xe::Math::Vector2i& CTilemap2d::GetBufferSize() const
{
	return m_BufferSize;
}

void CTilemap2d::SetBufferSize(const Xe::Math::Vector2i& bufferSize)
{
	if (m_BufferSize == bufferSize)
		return;

	m_BufferSize = bufferSize;
	ResizeLayer(bufferSize, m_Layer);
}

bool CTilemap2d::IsLayerVisible() const
{
	return m_Layer.Visible;
}

void CTilemap2d::SetLayerVisible(bool visible)
{
	m_Layer.Visible = visible;
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
	m_Timer += deltaTime;
}

void CTilemap2d::Flush()
{
	if (m_TileSize.x <= 0 || m_TileSize.y <= 0)
		return;

	auto requiredSizeX = Math::Min<size_t>(m_Layer.Size.x, m_CameraSize.x / m_TileSize.x + 1);
	auto requiredSizeY = Math::Min<size_t>(m_Layer.Size.y, m_CameraSize.y / m_TileSize.y + 1);

	if (requiredSizeX <= 0 || requiredSizeY <= 0 ||
		m_TileSize.x <= 0 || m_TileSize.y <= 0)
		return;

	TilemapRequestTilesArgs args;
	args.Position.x = (int)Math::Floor(m_CameraPosition.x / m_TileSize.x);
	args.Position.y = (int)Math::Floor(m_CameraPosition.y / m_TileSize.y);

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
	m_DrawVertices.Clear();
	m_DrawIndices.Clear();
	m_DrawColors.Clear();
	m_DrawTextureModes.Clear();

	if (m_TileSize.x == 0 || m_TileSize.y == 0)
		return;

	int vertexIndex = 0;
	int indices = 0;
	u16 colorIndex;
	u16 texModeIndex;

	if (m_BgColor.a > 0)
	{
		m_DrawVertices.Reserve(4);
		m_DrawIndices.Reserve(6);

		colorIndex = PushColor(m_BgColor);
		texModeIndex = PushTexModeNoTexture();
		m_DrawVertices.Data[vertexIndex + 0] = { 0, 0, 0, 0, colorIndex, texModeIndex };
		m_DrawVertices.Data[vertexIndex + 1] = { (float)m_CameraSize.x, 0, 0, 0, colorIndex, texModeIndex };
		m_DrawVertices.Data[vertexIndex + 2] = { 0, (float)m_CameraSize.y, 0, 0, colorIndex, texModeIndex };
		m_DrawVertices.Data[vertexIndex + 3] = { (float)m_CameraSize.x, (float)m_CameraSize.y, 0, 0, colorIndex, texModeIndex };
		
		m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 1) };
		m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 0) };
		m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 2) };
		m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 1) };
		m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 2) };
		m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 3) };
		vertexIndex += 4;
	}

	float tx = (float)m_TileSize.x;
	float ty = (float)m_TileSize.y;
	int width = (m_CameraSize.x + m_TileSize.x) / m_TileSize.x;
	int height = (m_CameraSize.y + m_TileSize.y) / m_TileSize.y;
	float cameraShiftX = -Math::Fmod(m_CameraPosition.x, (float)m_TileSize.x);
	float cameraShiftY = -Math::Fmod(m_CameraPosition.y, (float)m_TileSize.y);
	colorIndex = PushColor(Xe::Graphics::Color::White);
	texModeIndex = PushTexModeTexture();

	if (m_Layer.Visible == false)
		return;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileData tileData = m_Layer.Data[(x % m_Layer.Size.x) + (y % m_Layer.Size.y) * m_Layer.Size.x];
			tileData = GetTileData(tileData);
			u32 tile = tileData.Tile;
			if (tile > 0)
			{
				tile--;
				m_DrawVertices.Reserve(4);
				m_DrawIndices.Reserve(6);

				float fx = cameraShiftX + x * tx;
				float fy = cameraShiftY + y * ty;

				float u1 = (float)((tile % m_Tileset.TilesPerRow) * m_TileSize.x);
				float v1 = (float)((tile / m_Tileset.TilesPerRow) * m_TileSize.y);
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

					m_DrawVertices.Data[vertexIndex + 0] = { fx, fy, u1, v1, colorIndex, texModeIndex };
					m_DrawVertices.Data[vertexIndex + 1] = { fx + tx, fy, u1, v2, colorIndex, texModeIndex };
					m_DrawVertices.Data[vertexIndex + 2] = { fx, fy + ty, u2, v1, colorIndex, texModeIndex };
					m_DrawVertices.Data[vertexIndex + 3] = { fx + tx, fy + ty, u2, v2, colorIndex, texModeIndex };
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

					m_DrawVertices.Data[vertexIndex + 0] = { fx, fy, u1, v1, colorIndex, texModeIndex };
					m_DrawVertices.Data[vertexIndex + 1] = { fx + tx, fy, u2, v1, colorIndex, texModeIndex };
					m_DrawVertices.Data[vertexIndex + 2] = { fx, fy + ty, u1, v2, colorIndex, texModeIndex };
					m_DrawVertices.Data[vertexIndex + 3] = { fx + tx, fy + ty, u2, v2, colorIndex, texModeIndex };
				}

				m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 1) };
				m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 0) };
				m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 2) };
				m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 1) };
				m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 2) };
				m_DrawIndices.Data[indices++] = { (u16)(vertexIndex + 3) };
				vertexIndex += 4;
			}
		}

		ASSERT(m_DrawVertices.Count == vertexIndex);
		ASSERT(m_DrawIndices.Count == indices);
	}

	TilemapDrawArgs args;
	args.Draws.push_back(TilemapDrawList
	{
		m_DrawVertices.Data,
		m_DrawIndices.Data,
		m_DrawColors.Data,
		m_DrawTextureModes.Data,

		m_DrawVertices.Count,
		m_DrawIndices.Count,
		m_DrawColors.Count,
		m_DrawTextureModes.Count
	});

	TilemapArgs<TilemapDrawArgs> e;
	e.Sender = this;
	e.Arguments = &args;
	(*m_DrawDelegate)(e);
}

inline u16 CTilemap2d::PushColor(const Color& color)
{
	auto count = m_DrawColors.Count;
	m_DrawColors.Reserve(1);
	m_DrawColors.Data[count] = color;
	return (u16)count;
}

inline u16 CTilemap2d::PushTexMode(float mode)
{
	auto count = m_DrawTextureModes.Count;
	m_DrawTextureModes.Reserve(1);
	m_DrawTextureModes.Data[count] = mode;
	return (u16)count;
}

inline u16 CTilemap2d::PushTexModeNoTexture()
{
	return PushTexMode(1.0f);
}

inline u16 CTilemap2d::PushTexModeTexture()
{
	return PushTexMode(0.5f);
}

inline u16 CTilemap2d::PushTexModePalette(float palette)
{
	return PushTexMode(palette / 2.0f);
}

TileData CTilemap2d::GetTileData(TileData tile) const
{
	// TODO iterating all the possible animated tiles for every tile that the
	// engine wants to draw it is not the best thing. An idea would be to
	// create a look-up table, reducing the complexity of GetTileData from
	// O(n) to O(1). The only down-side of this approach is that the possible
	// amount of tile indices is 2^29, which is 2GB of required memory... :D.
	// Since it is impossible for a map to use 2^29 tiles, if the highest tile
	// index is known, it would be possible to create the LUT. Let's suppose
	// that, with a tileset of 1024x1024 with 16x16 tiles, the highest tile is
	// 4096; in this case, the LUT will be 4096*sizeof(TileData) = 16KB!!!

	for (auto seq : m_AnimatedTiles)
	{
		if (tile.Tile == seq.TileId.Tile)
		{
			float totalLength = 0.0f;
			for (size_t j = 0; j < seq.Sequence.Count; ++j)
			{
				totalLength += seq.Sequence.Data[j].DelayMs;
			}

			double msIndex = Math::Fmod(m_Timer * 1000.0, (double)totalLength);
			totalLength = 0.0f;
			for (size_t j = 0; j < seq.Sequence.Count; ++j)
			{
				const auto& frame = seq.Sequence.Data[j];
				totalLength += frame.DelayMs;
				if (msIndex < totalLength)
				{
					return { (u32)(frame.Tile + 1) };
				}
			}

			return tile;
		}
	}

	return tile;
}

void CTilemap2d::ResizeLayer(const Xe::Math::Vector2i& size, Layer& layer)
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