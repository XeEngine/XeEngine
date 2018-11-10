#pragma once
#include <XeSDK/IGraphicsContext.h>
#include <XeGame/IGameDrawing2d.h>

namespace Xe { namespace Game {
	class CDrawing2d : public IDrawing2d
	{
		static const svar MaximumQuadsCount = 16383;

		Graphics::IContext* m_Context;
		Graphics::IBuffer* m_pVertexBuffer;
		Graphics::IBuffer* m_pIndexBuffer;
		Graphics::Vertex* m_pVertex;
		Graphics::Vertex* m_pVertexCur;
		Math::Matrix4 m_Matrix;
		uvar m_curQuadsCount;
		bool m_IsInitialized;

	public:
		CDrawing2d(Graphics::IContext *context);
		~CDrawing2d();

		const Math::Matrix4 &GetMatrix() const;

		void SetMatrix(const Math::Matrix4 &matrix);

		void Flush();

		void DrawRectangle(const Math::Vector2f(&position)[2], const Graphics::Color &color);
		void DrawRectangle(const Math::Vector2f(&position)[2], const Graphics::Color(&color)[4]);
		void DrawRectangle(const Math::Vector2f(&position)[4], const Graphics::Color &color);
		void DrawRectangle(const Math::Vector2f(&position)[4], const Graphics::Color(&color)[4]);
		void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Graphics::Color &color);
		void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Graphics::Color(&color)[4]);
		void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Graphics::Color &color);
		void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Graphics::Color(&color)[4]);
		void DrawRectangle(const Math::Vector3f(&position)[4], const Graphics::Color &color);
		void DrawRectangle(const Math::Vector3f(&position)[4], const Graphics::Color(&color)[4]);
		void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Graphics::Color &color, float mode);
		void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Graphics::Color(&color)[4], float mode);
		void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color &color, float mode);
		void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color(&color)[4], float mode);
		void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color &color, float mode);
		void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color(&color)[4], float mode);
		void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color &color, float mode);
		void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color(&color)[4], float mode);
		void DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color &color, float mode);
		void DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color(&color)[4], float mode);
	};
} }