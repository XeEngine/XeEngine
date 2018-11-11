#include "pch.h"
#include <XeGame/XeGameDrawing2d.h>
#include "CGameDrawing2d.h"

using namespace Xe;
using namespace Xe::Math;
using namespace Xe::Graphics;
using namespace Xe::Game;

const float Xe::Game::IDrawing2d::MODE_PALETTIZED = 0.0f;
const float Xe::Game::IDrawing2d::MODE_TEXTURED = 0.75f;
const float Xe::Game::IDrawing2d::MODE_SOLIDCOLOR = 1.0f;

CDrawing2d::CDrawing2d(IContext *pContext) :
	m_Context(pContext),
	m_pVertexBuffer(nullptr),
	m_pVertex(new Vertex[MaximumQuadsCount * 4]),
	m_pVertexCur(m_pVertex),
	m_Matrix(Math::Matrix4::Identity()),
	m_curQuadsCount(0),
	m_IsInitialized(false)
{
	BufferDesc vertexBufferDesc;
	vertexBufferDesc.Usage = Usage_Dynamic;
	vertexBufferDesc.Length = MaximumQuadsCount * 4 * sizeof(Vertex);
	vertexBufferDesc.Type = BufferType_Vertex;

	Memory::Fill(m_pVertex, 0, vertexBufferDesc.Length);
	if (!m_Context->CreateBuffer(&m_pVertexBuffer, vertexBufferDesc, nullptr))
	{
		LOGE("Unable to create default vertex buffer.");
	}

	// INDICES BUFFER INITIALIZATION
	u16* indices = new u16[MaximumQuadsCount * 6];
	u32* pIndices = (u32*)indices;
	for (int i = 0, nx = 0x00000000; i < MaximumQuadsCount * 3; i += 3)
	{
		pIndices[i + 0] = nx + 0x00000001;
		pIndices[i + 1] = nx + 0x00010002;
		pIndices[i + 2] = nx + 0x00030002;

		nx += 0x00040004;
	}

	BufferDesc indexBufferDesc;
	indexBufferDesc.Usage = Usage_Static;
	indexBufferDesc.Length = MaximumQuadsCount * 6 * sizeof(u16);
	indexBufferDesc.Type = BufferType_Index16;

	DataDesc indexDataDesc;
	indexDataDesc.data = pIndices;
	indexDataDesc.pitch = indexBufferDesc.Length;

	if (!m_Context->CreateBuffer(&m_pIndexBuffer, indexBufferDesc, &indexDataDesc))
	{
		LOGE("Unable to create default index buffer.");
	}

	m_IsInitialized = true;
}

CDrawing2d::~CDrawing2d()
{
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	delete[] m_pVertex;
}

const Math::Matrix4& CDrawing2d::GetMatrix() const
{
	return m_Matrix;
}

void CDrawing2d::SetMatrix(const Math::Matrix4& matrix)
{
	m_Matrix = matrix;
}

void CDrawing2d::Flush()
{
	if (m_IsInitialized)
	{
		static const int start = 0;
		int stride = sizeof(Vertex), offsets = 0;

		// Upload new vertex buffer's content
		DataDesc desc;
		if (m_pVertexBuffer->Lock(desc, Lock_Write))
		{
			m_curQuadsCount = m_pVertexCur - m_pVertex;

			Memory::Copy((void*)desc.data, m_pVertex, sizeof(Vertex) * m_curQuadsCount * 4);
			m_pVertexBuffer->Unlock();

			m_Context->SetVertexBuffer(m_pVertexBuffer);
			m_Context->SetIndexBuffer(m_pIndexBuffer);
			m_Context->DrawIndexed(Primitive_TriangleList, m_curQuadsCount * 6, start);
		}
		else
		{
			LOGE("Unable to upload new buffer's content.");
		}
	}

	m_pVertexCur = m_pVertex;
	m_curQuadsCount = 0;
}

void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[2], const Color &color) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);

	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;

	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;

	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;

	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[4], const Color &color) {
	m_pVertexCur->Position = Math::Vector3f(position[0], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]) {
	m_pVertexCur->Position = Math::Vector3f(position[0], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color &color) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);

	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;

	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;

	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;

	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color(&color)[4]) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color &color) {
	m_pVertexCur->Position = Math::Vector3f(position[0], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color(&color)[4]) {
	m_pVertexCur->Position = Math::Vector3f(position[0], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], z).Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Math::Vector3f(&position)[4], const Color &color) {
	m_pVertexCur->Position = position[0].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = position[1].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = position[2].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = position[3].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawRectangle(const Vector3f(&position)[4], const Color(&color)[4]) {
	m_pVertexCur->Position = position[0].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position = position[1].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position = position[2].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position = position[3].Transform(m_Matrix);
	m_pVertexCur->Texture.z = 1.0f;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = 0.0f;
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
	m_pVertexCur->Position = Math::Vector3f(position[0], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
	m_pVertexCur->Position = Math::Vector3f(position[0], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], 0.0f).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
	const Vector2f v1 = position[0].Transform(m_Matrix);
	const Vector2f v2 = position[1].Transform(m_Matrix);
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v1.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v1.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position.x = v2.x;
	m_pVertexCur->Position.y = v2.y;
	m_pVertexCur->Position.z = z;
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {
	m_pVertexCur->Position = Math::Vector3f(position[0], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {
	m_pVertexCur->Position = Math::Vector3f(position[0], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[1], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[2], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position = Math::Vector3f(position[3], z).Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode) {
	m_pVertexCur->Position = position[0].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = position[1].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = position[2].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
	m_pVertexCur->Position = position[3].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color;
	m_pVertexCur++;
}
void CDrawing2d::DrawSurface(const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {
	m_pVertexCur->Position = position[0].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[0].x;
	m_pVertexCur->Texture.y = uvCoord[0].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[0];
	m_pVertexCur++;
	m_pVertexCur->Position = position[1].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[1].x;
	m_pVertexCur->Texture.y = uvCoord[1].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[1];
	m_pVertexCur++;
	m_pVertexCur->Position = position[2].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[2].x;
	m_pVertexCur->Texture.y = uvCoord[2].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[2];
	m_pVertexCur++;
	m_pVertexCur->Position = position[3].Transform(m_Matrix);
	m_pVertexCur->Texture.x = uvCoord[3].x;
	m_pVertexCur->Texture.y = uvCoord[3].y;
	m_pVertexCur->Texture.z = mode;
	m_pVertexCur->Color = color[3];
	m_pVertexCur++;
}

void Xe::Game::Factory(Xe::Game::IDrawing2d** ppDrawing2d, Xe::Graphics::IContext* pContext)
{
	*ppDrawing2d = new CDrawing2d(pContext);
}