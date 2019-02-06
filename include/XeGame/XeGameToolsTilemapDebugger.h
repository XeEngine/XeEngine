#pragma once
#include <XeGame/IGameToolsFrame.h>
#include <XeGame/IGameTilemap2d.h>
#include <XeGame/XeGameDrawing2d.h>

namespace Xe { namespace Game { namespace Tools
{
	class TilemapDebugger :	public Xe::Game::Tools::IFrame
	{
		ObjPtr<Xe::Graphics::IContext> m_Context;
		ObjPtr<Xe::Graphics::ISurface> m_LayerSurface;
		ObjPtr<Xe::Game::ITextureManager> m_TextureManager;
		ObjPtr<Xe::Game::ITilemap2d> m_Tilemap;
		ObjPtr<Xe::Game::IDrawing2d> m_Drawing;
		int m_LayerSelection;
		bool m_IsVisible;

		void Show(Xe::Game::ITilemap2d& tilemap);
		void CtrlBackgroundColor(Xe::Game::ITilemap2d& tilemap);
		void CtrlCameraSize(Xe::Game::ITilemap2d& tilemap);
		void CtrlCameraPosition(Xe::Game::ITilemap2d& tilemap);
		void CtrlTileSize(Xe::Game::ITilemap2d& tilemap);
		void CtrlLayers(Xe::Game::ITilemap2d& tilemap);

		void Show(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerName(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerBufferSize(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerScrollOffset(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerScrollMultiplier(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerVisibility(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerTexture(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerTilesPerRow(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerPalette(Xe::Game::ITilemapLayer& layer);
		void CtrlLayerImage(Xe::Game::ITilemapLayer& layer);

		void ShowLayerDataTooltip(Xe::Game::ITilemapLayer& layer, float posX, float posY);

		bool RenderToTexture(Math::Vector2i size);
		void RenderLayer(size_t layerIndex, const Math::Vector2i& size);
		void RenderLayer(size_t layerIndex, const Math::Rectanglef& rectangle);
	public:
		TilemapDebugger(
			ObjPtr<Xe::Graphics::IContext> context,
			ObjPtr<Xe::Game::ITextureManager> textureManager,
			ObjPtr<Xe::Game::ITilemap2d> tilemap);
		virtual ~TilemapDebugger();

		const char* GetTitle() const;
		bool IsVisible() const;
		void SetVisible(bool visible);

		void operator()(const Xe::Game::ImGuiFrameArgs& args);
	};
} } }