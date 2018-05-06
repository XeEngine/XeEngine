#include "pch.h"
#include "ContextGL.Common.h"

namespace Xe { namespace Graphics {
	const GLenum PRIMITIVE[]
	{
		GL_POINTS,// Primitive_PointList,
		GL_LINES, // Primitive_LineList,
		GL_LINE_STRIP, // Primitive_LineStrip,
		GL_TRIANGLES, // Primitive_TriangleList,
		GL_TRIANGLE_STRIP, // Primitive_TriangleStrip
	};

	GLenum CContextGLCommon::GetPrimitive(Primitive primitive)
	{
		return PRIMITIVE[primitive];
	}
} }