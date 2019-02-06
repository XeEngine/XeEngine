#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMathRectangle.h>

namespace Xe { namespace Game {
	interface ITilemap2d;
	interface IDrawing2d;
	void Factory(ITilemap2d** ppTilemap2d, IDrawing2d* pDrawing2d);

	struct TilemapDrawList;
	interface ITextureManager;
	void Draw(
		const Xe::Game::TilemapDrawList& drawList,
		Xe::Graphics::IContext* pContext,
		Xe::Game::ITextureManager* pTextureManager,
		const Xe::Math::Rectanglef& rectangle);
} }