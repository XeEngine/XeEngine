#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeGame/IGameDrawing2d.h>

namespace Xe { namespace Game {
	void Factory(Xe::Game::IDrawing2d** ppDrawing2d, Xe::Graphics::IContext* pContext);
} }