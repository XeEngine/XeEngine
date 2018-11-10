#pragma once

namespace Xe { namespace Game {
	interface ITilemap2d;
	interface IDrawing2d;
	void Factory(ITilemap2d** ppTilemap2d, IDrawing2d* pDrawing2d);
} }