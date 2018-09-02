#include "pch.h"
#include "ContextNull.h"

namespace Xe {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////
		// IDrawing2d implementation
		CContextNull::CDrawing::CDrawing(CContextNull *context) :
			m_context(context) {
		}
		CContextNull::CDrawing::~CDrawing() {
		}

		bool CContextNull::CDrawing::GetDestinationSurface(ISurface **surface) {
			return false;
		}
		void CContextNull::CDrawing::SetDestinationSurface(ISurface *surface) {}
		void CContextNull::CDrawing::Flush() {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color &color) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], const Color(&color)[4]) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color &color) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], const Color(&color)[4]) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color &color) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[2], float z, const Color(&color)[4]) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color &color) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector2f(&position)[4], float z, const Color(&color)[4]) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector3f(&position)[4], const Color &color) {}
		void CContextNull::CDrawing::DrawRectangle(const Math::Vector3f(&position)[4], const Color(&color)[4]) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[2], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color &color, float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector2f(&position)[4], const Math::Vector2f(&uvCoord)[4], float z, const Color(&color)[4], float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color &color, float mode) {}
		void CContextNull::CDrawing::DrawSurface(const Math::Vector3f(&position)[4], const Math::Vector2f(&uvCoord)[4], const Color(&color)[4], float mode) {}
	}
}