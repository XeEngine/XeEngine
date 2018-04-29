#pragma once
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeMathVector3.h>
#include <XeSDK/XeMathMatrix.h>
#include <XeSDK/XeGraphicsColor.h>

using namespace Xe::Math;

namespace Xe {
    namespace Graphics {
        namespace Common {
            struct Vertex {
                Vector3f pos;
                float u, v, p;
                Color color;
            };

            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[2], const Color &color);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[2], const Color(&color)[4]);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[4], const Color &color);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[4], const Color(&color)[4]);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[2], float z, const Color &color);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[2], float z, const Color(&color)[4]);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[4], float z, const Color &color);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector2f(&position)[4], float z, const Color(&color)[4]);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector3f(&position)[4], const Color &color);
            void DrawRectangle(Vertex *vOut, const Matrix4 &m, const Math::Vector3f(&position)[4], const Color(&color)[4]);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color &color, float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[2], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color &color, float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector2f(&position)[4], const Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color &color, float mode);
            void DrawSurface(Vertex *vOut, const Matrix4 &m, const Vector3f(&position)[4], const Vector2f(&uvCoord)[4], const Color(&color)[4], float mode);
        }
    }
}