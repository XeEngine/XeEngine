#pragma once

namespace Xe { namespace Game {
	class CTilemap2d : public ITilemap2d {
		Graphics::IDrawing2d* m_pDrawing;

		Graphics::Size m_TileSize;
		Math::Vector2f m_TileSizef;

		int m_TilesPerRow;
		Math::Rectangle<float> m_TilesetRectf;
		Math::Vector2f m_TilesetPos;
		Math::Vector2f m_TilesetSize;
		Math::Vector2f m_TilesetMul;
		Math::Vector2f m_TilesetPadding;

		Math::Rectanglef m_Camera;

		Graphics::Size m_MapSize;
		int m_ParallaxSize;
		TileData* m_Tilemap;
		float* m_Parallax;

		void SetTileset(const TilesetProperties& tileset);

		const Graphics::Size& GetMapSize() const;
		void SetMapSize(const Graphics::Size& size);

		void Lock(TilemapData& data);
		void Unlock();

		const Math::Rectanglef& GetCamera() const;
		void SetCamera(const Math::Rectanglef& camera);

		void Draw(int flags);
		void DrawStandard() const;
		void DrawFlip() const;

	public:
		CTilemap2d(Graphics::IDrawing2d* context);
		~CTilemap2d();
	};
} }