#include "pch.h"
#include <XeGame/XeGameTilemap2d.h>
#include "CGameTilemap2d.h"

using namespace Xe;
using namespace Xe::Graphics;
using namespace Xe::Game;

void Xe::Game::Factory(ITilemap2d** ppTilemap2d, IDrawing2d* pDrawing2d)
{
	*ppTilemap2d = new CTilemap2d();
}

void Xe::Game::Draw(
	const Xe::Game::TilemapDrawList& drawList,
	Xe::Graphics::IContext* pContext,
	Xe::Game::ITextureManager* pTextureManager,
	const Xe::Math::Rectanglef& rectangle)
{
	if (drawList.VerticesCount == 0 || drawList.IndicesCount == 0)
		return;

	const auto TexSize = pTextureManager->GetTextureSize(drawList.TextureId);
	const float CameraWidth = (float)rectangle.GetWidth();
	const float CameraHeight = (float)rectangle.GetHeight();

	pTextureManager->SelectTexture(drawList.TextureId, 0);
	pTextureManager->SelectClut(1);

	Vertex* vData = new Vertex[drawList.VerticesCount];
	for (size_t i = 0; i < drawList.VerticesCount; i++)
	{
		auto& vDst = vData[i];
		auto& vSrc = drawList.VerticesData[i];

		vDst.Position.x = +(rectangle.left + vSrc.x) / CameraWidth * 2 - 1.0f;
		vDst.Position.y = -(rectangle.top + vSrc.y) / CameraHeight * 2 + 1.0f;
		vDst.Position.z = 0.0f;
		vDst.Texture.x = vSrc.u / TexSize.x;
		vDst.Texture.y = vSrc.v / TexSize.y;
		vDst.Texture.z = drawList.TextureModeData[vSrc.TextureModeIndex];
		vDst.Color = drawList.ColorsData[vSrc.ColorIndex];
	}

	u32 vertexLength = drawList.VerticesCount * sizeof(Vertex);
	BufferDesc vertexBufferDesc{ Usage_Static, vertexLength, BufferType_Vertex };
	DataDesc vertexDataDesc{ vData, (svar)vertexLength };

	ObjPtr<IBuffer> vertexBuffer;
	LOGFA(pContext->CreateBuffer(&vertexBuffer, vertexBufferDesc, &vertexDataDesc));
	delete[] vData;

	u32 indexLength = drawList.IndicesCount * sizeof(TilemapDrawIndex);
	BufferDesc indexBufferDesc{ Usage_Static, indexLength, BufferType_Index16 };
	DataDesc indexDataDesc{ drawList.IndicesData, (svar)indexLength };

	ObjPtr<IBuffer> indexBuffer;
	LOGFA(pContext->CreateBuffer(&indexBuffer, indexBufferDesc, &indexDataDesc));

	pContext->SetVertexBuffer(vertexBuffer);
	pContext->SetIndexBuffer(indexBuffer);
	pContext->DrawIndexed(Xe::Graphics::Primitive_TriangleList, drawList.IndicesCount, 0);
	pContext->SetVertexBuffer(nullptr);
	pContext->SetIndexBuffer(nullptr);
}