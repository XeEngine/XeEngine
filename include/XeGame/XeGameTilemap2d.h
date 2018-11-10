#pragma once

namespace Xe { namespace Game {
	interface ITilemap2d;
	void Factory(ITilemap2d** ppTilemap2d, Xe::Graphics::IDrawing2d* pDrawing2d);
} }