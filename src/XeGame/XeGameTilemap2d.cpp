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

CTilemapLayer::CTilemapLayer() :
	m_TextureId(TexInvalid),
	m_ClutId(ClutInvalid),
	m_TilesPerRow(1),
	m_Flags({ 0 }),
	m_Data(nullptr),
	m_LockedData(nullptr)
{
	SetBufferSize({ 1, 1 });
}

CTilemapLayer::~CTilemapLayer()
{
	if (m_Data) Xe::Memory::Free(m_Data);
	if (m_LockedData) Xe::Memory::Free(m_LockedData);
}

const String& CTilemapLayer::GetName() const
{
	return m_Name;
}

void CTilemapLayer::SetName(const StringSpan& name)
{
	m_Name = name;
}

const Vector2u& CTilemapLayer::GetBufferSize() const
{
	return m_BufferSize;
}

void CTilemapLayer::SetBufferSize(const Vector2u& size)
{
	if (size.x == 0)
		throw std::invalid_argument(NAMEOF(size.x) " must be greater than 0");
	if (size.y == 0)
		throw std::invalid_argument(NAMEOF(size.y) " must be greater than 0");

	if (m_BufferSize == size)
		return;

	m_BufferSize = size;

	if (m_Data)
		Xe::Memory::Free(m_Data);
	if (m_LockedData)
	{
		Xe::Memory::Free(m_LockedData);
		m_LockedData = nullptr;
	}

	m_Data = (TileData*)Xe::Memory::Alloc(m_BufferSize.x * m_BufferSize.y * sizeof(TileData));
}

const Vector2f& CTilemapLayer::GetPosition() const
{
	return m_Position;
}

void CTilemapLayer::SetPosition(const Vector2f& position)
{
	m_Position = position;
}

bool CTilemapLayer::IsVisible() const
{
	return !!m_Flags.Visible;
}

void CTilemapLayer::SetVisible(bool visibility)
{
	m_Flags.Visible = visibility;
}

TexId CTilemapLayer::GetTexture() const
{
	return m_TextureId;
}

size_t CTilemapLayer::GetTilesPerRow() const
{
	return m_TilesPerRow;
}

void CTilemapLayer::SetTexture(TexId textureId, size_t tilesPerRow)
{
	if (tilesPerRow == 0)
		throw std::invalid_argument(NAMEOF(tilesPerRow) " must be greater than 0");

	m_TextureId = textureId;
	m_TilesPerRow = tilesPerRow;
}

ClutId CTilemapLayer::GetPalette()
{
	return m_ClutId;
}
void CTilemapLayer::SetPalette(ClutId clutId)
{
	m_ClutId = clutId;
}

bool CTilemapLayer::IsLocked() const
{
	return !!m_Flags.Locked;
}

bool CTilemapLayer::Lock(TilemapData& tilemapData, Game::LockType lockType)
{
	if (IsLocked())
		throw std::logic_error("Cannot lock an already locked resource");

	m_Flags.Locked = true;
	m_LockType = lockType;

	tilemapData.Size = GetBufferSize();
	tilemapData.Stride = tilemapData.Size.x * sizeof(TileData);

	size_t length = tilemapData.Stride * tilemapData.Size.y;

	if (m_LockedData == nullptr)
		m_LockedData = (TileData*)Xe::Memory::Alloc(length);

	if (m_LockType & Lock_Read)
	{
		Xe::Memory::Copy(m_LockedData, m_Data, length);
	}

	tilemapData.Data = m_LockedData;

	return true;
}
void CTilemapLayer::Unlock()
{
	if (!IsLocked())
		throw std::logic_error("Cannot unlock a resource that is not locked");

	m_Flags.Locked = false;

	const auto& bufferSize = GetBufferSize();
	size_t length = bufferSize.x * sizeof(TileData) * bufferSize.y;

	if (m_LockType & Lock_Write)
	{
		Xe::Memory::Copy(m_Data, m_LockedData, length);
	}
}

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

	const auto& layerSize = m_Layers[0]->GetBufferSize();
	auto requiredSizeX = Math::Min<size_t>(layerSize.x, m_CameraSize.x / m_TileSize.x + 1);
	auto requiredSizeY = Math::Min<size_t>(layerSize.y, m_CameraSize.y / m_TileSize.y + 1);

	if (requiredSizeX <= 0 || requiredSizeY <= 0 ||
		m_TileSize.x <= 0 || m_TileSize.y <= 0)
		return;

	const auto& position = m_Layers[0]->GetPosition();

	TilemapRequestTilesArgs args;
	args.Position.x = (int)Math::Floor(position.x / m_TileSize.x);
	args.Position.y = (int)Math::Floor(position.y / m_TileSize.y);

	m_Layers[0]->Lock(args.Destination, Lock_ReadWrite);
	//args.Destination.Data += requiredSizeX + requiredSizeY * layerSize.y;
	args.Destination.Size.x = requiredSizeX;
	args.Destination.Size.y = requiredSizeY;

	TilemapArgs<TilemapRequestTilesArgs> e;
	e.Sender = this;
	e.Arguments = &args;
	(*m_RequestTilesDelegate)(e);

	m_Layers[0]->Unlock();
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

	const auto& position = m_Layers[0]->GetPosition();
	float tx = (float)m_TileSize.x;
	float ty = (float)m_TileSize.y;
	int width = (m_CameraSize.x + m_TileSize.x) / m_TileSize.x;
	int height = (m_CameraSize.y + m_TileSize.y) / m_TileSize.y;
	float cameraShiftX = -Math::Fmod(position.x, (float)m_TileSize.x);
	float cameraShiftY = -Math::Fmod(position.y, (float)m_TileSize.y);
	colorIndex = PushColor(Xe::Graphics::Color::White);
	texModeIndex = PushTexModeTexture();

	const auto& layer = *(CTilemapLayer*)m_Layers[0].Get();
	auto layerSize = layer.GetBufferSize();
	if (layer.IsVisible())
	{
		auto tilesPerRow = layer.GetTilesPerRow();
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				TileData tileData = layer.m_Data[(x % layerSize.x) + (y % layerSize.y) * layerSize.x];
				tileData = GetTileData(tileData);
				u32 tile = tileData.Tile;
				if (tile > 0)
				{
					tile--;
					m_DrawVertices.Reserve(4);
					m_DrawIndices.Reserve(6);

					float fx = cameraShiftX + x * tx;
					float fy = cameraShiftY + y * ty;

					float u1 = (float)((tile % tilesPerRow) * m_TileSize.x);
					float v1 = (float)((tile / tilesPerRow) * m_TileSize.y);
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
	}


	TilemapDrawArgs args;
	args.Draws.push_back(TilemapDrawList
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

void Xe::Game::Factory(ITilemap2d** ppTilemap2d, IDrawing2d* pDrawing2d)
{
	*ppTilemap2d = new CTilemap2d();
}