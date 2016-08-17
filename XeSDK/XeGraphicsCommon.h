#pragma once
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeMathMatrix.h>
#include <XeSDK/XeGraphicsColor.h>

using namespace Xe::Math;

namespace Xe {
	namespace Graphics {
		namespace Common {
			struct Vertex {
				Vector2f pos;
				float u, v, p;
				Color color;
			};

			void FlushDrawElements();

			void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[2], const Color &color);
			void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[2], const Color(&color)[4]);
			void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[4], const Color &color);
			void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[4], const Color(&color)[4]);
			void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color &color, float mode);
			void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
			void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode);
			void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
		}
	}
}