#include "pch.h"
#include <XeSDK/XeGameComponentTexture2d.h>

bool Xe::Game::Component::Texture2d::Query(IObject **obj, UID id) {
	switch (id) {
	case IObject::ID:
	case IComponent::ID:
	case Texture2d::ID:
		this->AddRef();
		*obj = this;
		return true;
	}
	return false;
}
Xe::Game::Component::Texture2d::Texture2d() :
	m_pSurface(0)
{
	SetSquareCoord(0.0f, 0.0f, 1.0f, 1.0f);
}
Xe::Game::Component::Texture2d::~Texture2d() {
	if (m_pSurface)
		m_pSurface->Release();
}
void Xe::Game::Component::Texture2d::OnUpdate(float update) {

}
void Xe::Game::Component::Texture2d::OnDraw() {

}
bool Xe::Game::Component::Texture2d::GetSurface(Graphics::ISurface **surface) {
	*surface = m_pSurface;
	if (m_pSurface) {
		m_pSurface->AddRef();
		return true;
	}
	return false;
}
void Xe::Game::Component::Texture2d::SetSurface(Graphics::ISurface *surface) {
	if (m_pSurface != surface) {
		if (m_pSurface)
			m_pSurface->Release();
		if (surface)
			surface->AddRef();
		m_pSurface = surface;
	}
}
void Xe::Game::Component::Texture2d::SetSquareCoord(float left, float top, float right, float bottom) {
	m_uv[0].x = m_uv[2].x = left;
	m_uv[0].y = m_uv[1].y = top;
	m_uv[1].x = m_uv[3].x = right;
	m_uv[2].y = m_uv[3].y = bottom;
}
void Xe::Game::Component::Texture2d::SetUVCoord(const Math::Vector2f(&coord)[4]) {
	m_uv[0] = coord[0];
	m_uv[1] = coord[1];
	m_uv[2] = coord[2];
	m_uv[3] = coord[3];
}
const Xe::Math::Vector2f(&Xe::Game::Component::Texture2d::GetUVCoord() const)[4]{
	return m_uv;
}