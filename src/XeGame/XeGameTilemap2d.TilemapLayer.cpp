#include "pch.h"
#include "CGameTilemapLayer.h"

using namespace Xe;
using namespace Xe::Math;
using namespace Xe::Game;

CTilemapLayer::CTilemapLayer() :
	m_ScrollMultiplier({ 1.0f, 1.0f }),
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

const Vector2f& CTilemapLayer::GetScrollOffset() const
{
	return m_ScrollOffset;
}

void CTilemapLayer::SetScrollOffset(const Vector2f& offset)
{
	m_ScrollOffset = offset;
}

const Xe::Math::Vector2f& CTilemapLayer::GetScrollMultiplier() const
{
	return m_ScrollMultiplier;
}

void CTilemapLayer::SetScrollMultiplier(const Xe::Math::Vector2f& multiplier)
{
	m_ScrollMultiplier = multiplier;
}

Xe::Math::Vector2f CTilemapLayer::ProcessPosition(
	const Xe::Math::Vector2f& cameraPosition) const
{
	return cameraPosition * m_ScrollMultiplier + m_ScrollOffset;
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
