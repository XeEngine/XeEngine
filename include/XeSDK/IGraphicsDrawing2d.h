#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMathMatrix.h>

namespace Xe {
	namespace Graphics {
		class IDrawing2d : public IObject {
			Math::Matrix4 m_matrix;
		public:
			static const UID ID = 0xfd85e7e3c24f4b83ULL;

			static const float MODE_PALETTIZED;
			static const float MODE_TEXTURED;
			static const float MODE_SOLIDCOLOR;

			IDrawing2d() : m_matrix(Math::Matrix4::Identity()) {}

			const Math::Matrix4 &GetMatrix() const;
			void SetMatrix(const Math::Matrix4 &matrix);

			virtual void Flush() = 0;
			virtual void DrawRectangle(const Math::Vector2f (&position)[2], const Color &color) = 0;
			virtual void DrawRectangle(const Math::Vector2f (&position)[2], const Color (&color)[4]) = 0;
			virtual void DrawRectangle(const Math::Vector2f (&position)[4], const Color &color) = 0;
			virtual void DrawRectangle(const Math::Vector2f (&position)[4], const Color (&color)[4]) = 0;

			virtual void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) = 0;
			virtual void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) = 0;
			virtual void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) = 0;
			virtual void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) = 0;
		};
	}
}