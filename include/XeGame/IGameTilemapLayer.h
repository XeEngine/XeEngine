#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeString.h>
#include <XeGame/XeGameTilemapDef.h>
#include <XeGame/ITextureManager.h>

namespace Xe { namespace Game {

	//! \brief How to lock the resource
	enum LockType
	{
		//! \brief The resource will contain its content
		Lock_Read = 1,

		//! \brief The resource will create a buffer to write in on Unlock
		Lock_Write = 2,

		/** \brief The resource will create a buffer containing its content,
		 * then writing it back on Unlock.
		 */
		Lock_ReadWrite = Lock_Read | Lock_Write
	};

	struct TilemapData
	{
		TileData* Data;
		Math::Vector2u Size;
		size_t Stride;

		inline TileData& Tile(unsigned x, unsigned y)
		{
			return ((TileData*)((u8*)Data + Stride * y))[x];
		}
	};

	//! \brief Layer of tile maps; can store tiles and render them.
	interface ITilemapLayer : public IObject
	{
		//! \brief Get the name of the layer, useful for debugging
		//! \sa SetName
		virtual const String& GetName() const = 0;

		//! \brief Set the name of the layer, useful for debugging
		//! \sa GetName
		virtual void SetName(const StringSpan& name) = 0;

		//! \brief Get the size of the buffer
		/** \details The size of the buffer corresponds on how much tiles the
		 * layer can store horizontally and vertically. The more big is the
		 * buffer the less will be the tile request that the layer will perform
		 * when the layer position is changed.
		 */
		//! \sa SetBufferSize
		virtual const Xe::Math::Vector2u& GetBufferSize() const = 0;

		//! \brief Set the size of the buffer
		//! \param[in] size New size of the buffer. Must be greater than 0.
		/** \details Change the buffer size of the layer. All the previous
		 * content will be invalidated.
		 */
		 //! \sa GetBufferSize
		virtual void SetBufferSize(const Xe::Math::Vector2u& size) = 0;

		//! \brief Get the scroll value of the layer
		//! \sa SetPosition
		virtual const Xe::Math::Vector2f& GetPosition() const = 0;

		//! \brief Set the scroll value of the layer
		/** \details The scrolling (or position for convenience) is it used to
		 * move the drawing area across the buffer and to fetch the required
		 * tiles from the source.
		 */
		 //! \sa GetPosition
		virtual void SetPosition(const Xe::Math::Vector2f& position) = 0;

		//! \brief If the layer can be drawn
		//! \details The default value is false
		//! \sa SetVisible
		virtual bool IsVisible() const = 0;

		//! \brief Set the visibility of the layer
		/** \details When a layer is visible, when the position is changed the
		* layer might ask to fetch the required tiles. Plus, the content of
		 * the layer will be put on the draw list when it is requested. A non
		 * visible layer will not perform any of the tasks mentioned above.
		 */
		//! \sa IsVisible
		virtual void SetVisible(bool visibility) = 0;

		//! \brief Get the texture id used to render the tiles of the layer
		//! \sa SetTexture, GetTilesPerRow
		virtual TexId GetTexture() const = 0;

		//! \brief Get how many tiles are contained in a row for 
		//! \sa GetTexture, SetTexture
		virtual size_t GetTilesPerRow() const = 0;

		//! \brief Set the texture that will be used to render the layer
		//! \param[in] textureId Texture used for drawing; can be TexInvalid.
		//! \param[in] tilesPerRow How many tiles are contained in a texture row; cannot be 0.
		//! \sa GetTexture, GetTilesPerRow
		virtual void SetTexture(TexId textureId, size_t tilesPerRow) = 0;

		//! \brief Get the palette used to render the layer
		virtual ClutId GetPalette() = 0;

		//! \brief Set the palette to render the layer
		//! \param[in] clutId Clut to use; can be ClutInvalid
		virtual void SetPalette(ClutId clutId) = 0;

		//! \brief Lock the buffer to provide read or write functionalities
		//! \param[out] tilemapData The structure that will process the buffer
		//! \param[in] lockType How to lock the buffer
		//! \return true if the lock succeeded.
		/** \details When a buffer is locked with success, the tilemapData
		 * structure will be filled with the information represented by the
		 * buffer.
		 * When the LockType specifies reading, the content of the buffer will
		 * be copied into tilemapData.Data and it will not represent the
		 * buffer itself. If LockType does not specify reading, then Data will
		 * contain just invalid data that does not represent the content of
		 * buffer at all.
		 * When LockType specifies write, then whatever is the content of Data
		 * it will be copied to the buffer when Unlock is called.
		 * A locked buffer cannot be locked and every Tilemap operation to it
		 * like draw or flush will throw an exception until Unlock is called.
		 * It is a common practice to lock, process the data and immediately
		 * unlock the layer. A function to check if a layer is locked or not
		 * is not provided.
		 * Once the layer is unlocked, Data content will not be valid anymore.
		 */
		virtual bool Lock(TilemapData& tilemapData, LockType lockType) = 0;

		//! \brief Unlock a previously locked layer.
		//! \details If the Lock specified Write, the buffer will be updated.
		virtual void Unlock() = 0;
	};
} }