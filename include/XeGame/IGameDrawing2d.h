#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeMathVector3.h>
#include <XeSDK/XeMathMatrix.h>

namespace Xe { namespace Game {
	interface IDrawing2d : public IObject {
	public:
		static const float MODE_PALETTIZED;
		static const float MODE_TEXTURED;
		static const float MODE_SOLIDCOLOR;

		virtual void Flush() = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[2], const Graphics::Color &color) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[2], const Graphics::Color (&color)[4]) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[4], const Graphics::Color &color) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[4], const Graphics::Color (&color)[4]) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Graphics::Color &color) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[2], float z, const Graphics::Color(&color)[4]) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Graphics::Color &color) = 0;
		virtual void DrawRectangle(const Math::Vector2f(&position)[4], float z, const Graphics::Color(&color)[4]) = 0;
		virtual void DrawRectangle(const Math::Vector3f(&position)[4], const Graphics::Color &color) = 0;
		virtual void DrawRectangle(const Math::Vector3f(&position)[4], const Graphics::Color(&color)[4]) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Graphics::Color &color, float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Graphics::Color(&color)[4], float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color &color, float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color(&color)[4], float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color &color, float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color(&color)[4], float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color &color, float mode) = 0;
		virtual void DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Graphics::Color(&color)[4], float mode) = 0;
		virtual void DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color &color, float mode) = 0;
		virtual void DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Graphics::Color(&color)[4], float mode) = 0;

	};
} }