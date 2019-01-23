#include "pch.h"
#include <XeGame/XeGameTilemap2d.h>
#include "CGameTilemap2d.h"

void Xe::Game::Factory(ITilemap2d** ppTilemap2d, IDrawing2d* pDrawing2d)
{
	*ppTilemap2d = new CTilemap2d();
}