#pragma once
#include <XeGame/IGameTilemap2d.h>

namespace Xe { namespace Game {
	class CTilemap2d : public ITilemap2d
	{
		struct Layer
		{
			TileData* Data;
			TilemapBufferSize Size;
		};

		TilemapRequestTilesDelegate* m_RequestTilesDelegate;
		TilemapDrawDelegate* m_DrawDelegate;
		Math::Vector2i m_CameraSize;
		Math::Vector2i m_CameraPosition;
		TilemapTileSize m_TileSize;
		TilemapBufferSize m_BufferSize;
		TilesetProperties m_Tileset;
		Layer m_Layer;

		std::vector<TilemapDrawVertex> m_DrawVertices;
		std::vector<TilemapDrawIndex> m_DrawIndices;

		static void ValidateTilesetProperties(TilemapBufferSizeType bufferSizeType);

		static void ResizeLayer(const TilemapBufferSize& size, Layer& layer);
	public:
		CTilemap2d();
		~CTilemap2d();

		void SetRequestTilesCallback(TilemapRequestTilesDelegate* delegate);
		void SetDrawCallback(TilemapDrawDelegate* delegate);

		const Math::Vector2i& GetCameraSize();
		void SetCameraSize(const Math::Vector2i& cameraSize);

		const Math::Vector2i& GetCameraPosition();
		void SetCameraPosition(const Math::Vector2i& cameraPosition);

		const TilemapTileSize& GetTileSize();
		void SetTileSize(const TilemapTileSize& tileSize);

		const TilemapBufferSize& GetBufferSize();
		void SetBufferSize(const TilemapBufferSize& bufferSize);

		bool GetBuffer(TilemapData* layer);

		void SetTileset(const TilesetProperties& tileset);

		void Update(double deltaTime);
		void Flush();
		void Draw(int flags);
	};
} }