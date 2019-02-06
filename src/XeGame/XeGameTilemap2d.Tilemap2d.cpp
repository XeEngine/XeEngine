#include "pch.h"
#include "CGameTilemap2d.h"
#include "CGameTilemapLayer.h"

using namespace Xe;
using namespace Xe::Game;

CTilemap2d::CTilemap2d() :
	m_RequestTilesDelegate(nullptr),
	m_DrawDelegate(nullptr),
	m_Timer(0.0)
{
	SetLayersCount(1);
}

CTilemap2d::~CTilemap2d()
{
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

const Xe::Math::Vector2f& CTilemap2d::GetCameraPosition() const
{
	return m_CameraPosition;
}

void CTilemap2d::SetCameraPosition(const Xe::Math::Vector2f& cameraPosition)
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

size_t CTilemap2d::GetLayerCount() const
{
	return m_Layers.size();
}

void CTilemap2d::SetLayersCount(size_t layersCount)
{
	m_Layers.resize(layersCount);

	for (size_t i = 0; i < layersCount; ++i)
	{
		if (m_Layers[i].Get() == nullptr)
		{
			m_Layers[i] = new CTilemapLayer();
		}
	}
}

ObjPtr<ITilemapLayer> CTilemap2d::GetLayer(size_t index)
{
	if (index >= GetLayerCount())
		throw std::invalid_argument(NAMEOF(index) " exceeds " NAMEOF(GetLayerCount));

	return m_Layers[index];
}

const Xe::Math::Vector2i& CTilemap2d::GetBufferSize() const
{
	m_BufferSizeDELETEME.x = m_Layers[0]->GetBufferSize().x;
	m_BufferSizeDELETEME.y = m_Layers[0]->GetBufferSize().y;
	return m_BufferSizeDELETEME;
}

void CTilemap2d::SetBufferSize(const Xe::Math::Vector2i& bufferSize)
{
	m_Layers[0]->SetBufferSize({ (size_t)bufferSize.x, (size_t)bufferSize.y });
}

void CTilemap2d::Update(double deltaTime)
{
	m_Timer += deltaTime;
}

void CTilemap2d::Flush()
{
	if (m_TileSize.x <= 0 || m_TileSize.y <= 0)
		return;

	size_t layerIndex = 0;
	for (auto layer : m_Layers)
	{
		FetchLayer((CTilemapLayer&)*layer, layerIndex++);
	}
}

void CTilemap2d::Draw()
{
	TilemapDrawArgs args;
	Draw(args);

	TilemapArgs<TilemapDrawArgs> e;
	e.Sender = this;
	e.Arguments = &args;
	(*m_DrawDelegate)(e);
}

void CTilemap2d::Draw(TilemapDrawArgs& drawArgs)
{
	if (RenderBegin(drawArgs))
	{
		DrawBackground();

		PushColor(Xe::Graphics::Color::White);
		PushTexModeTexture();

		for (auto layer : m_Layers)
		{
			DrawLayer(*(CTilemapLayer*)layer.Get());
		}
	}

	RenderEnd(drawArgs);
}

void CTilemap2d::DrawLayer(size_t layerIndex, TilemapDrawArgs& drawArgs)
{
	if (RenderBegin(drawArgs))
	{
		PushColor(Xe::Graphics::Color::White);
		PushTexModeTexture();

		DrawLayer(*(CTilemapLayer*)GetLayer(layerIndex).Get());
	}

	RenderEnd(drawArgs);
}

inline u16 CTilemap2d::PeekColor() const
{
	return (u16)(m_DrawColors.Count - 1);
}

inline u16 CTilemap2d::PushColor(const Graphics::Color& color)
{
	auto count = m_DrawColors.Count;
	m_DrawColors.Reserve(1);
	m_DrawColors.Data[count] = color;
	return (u16)count;
}

inline u16 CTilemap2d::PeekTexMode() const
{
	return (u16)(m_DrawTextureModes.Count - 1);
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

void CTilemap2d::FetchLayer(CTilemapLayer& layer, size_t layerIndex)
{
	const auto& layerSize = layer.GetBufferSize();
	auto requiredSizeX = Math::Min<size_t>(layerSize.x, m_CameraSize.x / m_TileSize.x + 1);
	auto requiredSizeY = Math::Min<size_t>(layerSize.y, m_CameraSize.y / m_TileSize.y + 1);

	if (requiredSizeX <= 0 || requiredSizeY <= 0)
		return;

	auto position = layer.ProcessPosition(GetCameraPosition());

	TilemapRequestTilesArgs args;
	args.Position.x = (int)Math::Floor(position.x / m_TileSize.x);
	args.Position.y = (int)Math::Floor(position.y / m_TileSize.y);

	layer.Lock(args.Destination, Lock_ReadWrite);
	//args.Destination.Data += requiredSizeX + requiredSizeY * layerSize.y;
	args.Destination.Size.x = requiredSizeX;
	args.Destination.Size.y = requiredSizeY;
	args.LayerIndex = layerIndex;

	TilemapArgs<TilemapRequestTilesArgs> e;
	e.Sender = this;
	e.Arguments = &args;
	(*m_RequestTilesDelegate)(e);

	layer.Unlock();
}

bool CTilemap2d::RenderBegin(TilemapDrawArgs& drawArgs)
{
	m_DrawVertices.Clear();
	m_DrawIndices.Clear();
	m_DrawColors.Clear();
	m_DrawTextureModes.Clear();

	if (m_TileSize.x == 0 || m_TileSize.y == 0)
		return false;

	return true;
}

void CTilemap2d::RenderEnd(TilemapDrawArgs& drawArgs)
{
	drawArgs.Draws.push_back(TilemapDrawList
		{
			m_DrawVertices.Data,
			m_DrawIndices.Data,
			m_DrawColors.Data,
			m_DrawTextureModes.Data,

			m_Layers[0]->GetTexture(),
			m_Layers[0]->GetPalette(),

			m_DrawVertices.Count,
			m_DrawIndices.Count,
			m_DrawColors.Count,
			m_DrawTextureModes.Count
		});
}

void CTilemap2d::DrawBackground()
{
	if (m_BgColor.a <= 0)
		return;

	const auto vertexIndex = m_DrawVertices.Count;
	const auto indices = m_DrawIndices.Count;
	const auto colorIndex = PushColor(m_BgColor);
	const auto texModeIndex = PushTexModeNoTexture();

	m_DrawVertices.Reserve(4);
	m_DrawVertices.Data[vertexIndex + 0] = { 0, 0, 0, 0, colorIndex, texModeIndex };
	m_DrawVertices.Data[vertexIndex + 1] = { (float)m_CameraSize.x, 0, 0, 0, colorIndex, texModeIndex };
	m_DrawVertices.Data[vertexIndex + 2] = { 0, (float)m_CameraSize.y, 0, 0, colorIndex, texModeIndex };
	m_DrawVertices.Data[vertexIndex + 3] = { (float)m_CameraSize.x, (float)m_CameraSize.y, 0, 0, colorIndex, texModeIndex };

	m_DrawIndices.Reserve(6);
	m_DrawIndices.Data[indices + 0] = { (u16)(vertexIndex + 0) };
	m_DrawIndices.Data[indices + 1] = { (u16)(vertexIndex + 1) };
	m_DrawIndices.Data[indices + 2] = { (u16)(vertexIndex + 2) };
	m_DrawIndices.Data[indices + 3] = { (u16)(vertexIndex + 1) };
	m_DrawIndices.Data[indices + 4] = { (u16)(vertexIndex + 3) };
	m_DrawIndices.Data[indices + 5] = { (u16)(vertexIndex + 2) };
}

void CTilemap2d::DrawLayer(const CTilemapLayer& layer)
{
	if (!layer.IsVisible())
		return;

	auto vertexIndex = m_DrawVertices.Count;
	auto indices = m_DrawIndices.Count;
	auto colorIndex = PeekColor();
	auto texModeIndex = PeekTexMode();

	auto position = layer.ProcessPosition(GetCameraPosition());
	int width = (m_CameraSize.x + m_TileSize.x) / m_TileSize.x;
	int height = (m_CameraSize.y + m_TileSize.y) / m_TileSize.y;
	float tx = (float)m_TileSize.x;
	float ty = (float)m_TileSize.y;
	float cameraShiftX = -Math::Fmod(position.x, (float)m_TileSize.x);
	float cameraShiftY = -Math::Fmod(position.y, (float)m_TileSize.y);

	auto layerSize = layer.GetBufferSize();
	auto tilesPerRow = layer.GetTilesPerRow();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileData tileData = layer.m_Data[(x % layerSize.x) + (y % layerSize.y) * layerSize.x];
			tileData = GetTileData(tileData);
			u32 tile = tileData.Tile;

			if (tile == 0)
				continue;

			tile--;

			float fx = cameraShiftX + x * tx;
			float fy = cameraShiftY + y * ty;

			float u1 = (float)((tile % tilesPerRow) * m_TileSize.x);
			float v1 = (float)((tile / tilesPerRow) * m_TileSize.y);
			float u2 = u1 + m_TileSize.x;
			float v2 = v1 + m_TileSize.x;

			m_DrawVertices.Reserve(4);
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

			m_DrawIndices.Reserve(6);
			m_DrawIndices.Data[indices + 0] = { (u16)(vertexIndex + 0) };
			m_DrawIndices.Data[indices + 1] = { (u16)(vertexIndex + 1) };
			m_DrawIndices.Data[indices + 2] = { (u16)(vertexIndex + 2) };
			m_DrawIndices.Data[indices + 3] = { (u16)(vertexIndex + 1) };
			m_DrawIndices.Data[indices + 4] = { (u16)(vertexIndex + 3) };
			m_DrawIndices.Data[indices + 5] = { (u16)(vertexIndex + 2) };
			vertexIndex += 4;
			indices += 6;
		}

		ASSERT(m_DrawVertices.Count == vertexIndex);
		ASSERT(m_DrawIndices.Count == indices);
	}
}
