#include "pch.h"
#include <XeSDK/IGraphicsDrawing2d.h>

namespace Xe {
    namespace Graphics {
        const float IDrawing2d::MODE_PALETTIZED = 0.0f;
        const float IDrawing2d::MODE_TEXTURED = 0.75f;
        const float IDrawing2d::MODE_SOLIDCOLOR = 1.0f;

        const Math::Matrix4 &IDrawing2d::GetMatrix() const {
            return m_matrix;
        }
        void IDrawing2d::SetMatrix(const Math::Matrix4 &matrix) {
            m_matrix = matrix;
        }
    }
}