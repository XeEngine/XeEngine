#include "pch.h"
#include <XeSDK/XeGameComponentPosition2d.h>

Xe::Game::Component::Position2d::Position2d() :
	m_translation(Math::Vector3f(0, 0, 0)),
	m_rotation(Math::Vector3f(0, 0, 0)),
	m_scaling(Math::Vector3f(1, 1, 1)),
	m_isScalingInvalidated(false),
	m_isRotationXYInvalidated(0),
	m_isRotatedZ(0),
	m_isMatrixInvaldiated(0),
	m_matrix(Math::Matrix4().Identity())
{
	m_position2[0].Set(-1.0f, +1.0f);
	m_position2[1].Set(+1.0f, +1.0f);
	m_position2[2].Set(-1.0f, -1.0f);
	m_position2[3].Set(+1.0f, -1.0f);
}
Xe::Game::Component::Position2d::~Position2d() {

}
void Xe::Game::Component::Position2d::OnUpdate(float update) {

}
void Xe::Game::Component::Position2d::OnDraw() {

}

const Xe::Math::Vector3f &Xe::Game::Component::Position2d::GetTranslation() const {
	return m_translation;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::SetTranslation(const Math::Vector3f &translation) {
	m_isMatrixInvaldiated = true;
	m_isTranslationInvalidated = true;
	return m_translation = translation;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::GetCenter() const {
	return m_center;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::SetCenter(const Math::Vector3f &center) {
	m_isMatrixInvaldiated = true;
	m_isScalingInvalidated = true;
	return m_center = center;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::GetRotation() const {
	return m_rotation;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::SetRotation(const Math::Vector3f &rotation) {
	m_isMatrixInvaldiated = true;
	m_isTranslationInvalidated = true;
	m_isScalingInvalidated = true;
	if (rotation.x != 0 || rotation.y != 0)
		m_isRotationXYInvalidated = false;
	m_isRotatedZ = rotation.z != 0;
	return m_rotation = rotation;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::GetScaling() const {
	return m_scaling;
}
const Xe::Math::Vector3f &Xe::Game::Component::Position2d::SetScaling(const Math::Vector3f &scaling) {
	m_isMatrixInvaldiated = true;
	m_isTranslationInvalidated = true;
	m_isScalingInvalidated = true;
	return m_scaling = scaling;
}
const Xe::Math::Matrix4 &Xe::Game::Component::Position2d::GetMatrix() const {
	((Component::Position2d*)this)->UpdateMatrix();
	return m_matrix;
}

const Xe::Math::Vector2f(&Xe::Game::Component::Position2d::GetPosition2d() const)[4]{
	((Component::Position2d*)this)->UpdatePosition2();
	return m_position2;
}
void Xe::Game::Component::Position2d::UpdateMatrix() {
	if (m_isMatrixInvaldiated) {
		m_isMatrixInvaldiated = true;

		float sr = Math::Sin(m_rotation.y);
		float cr = Math::Cos(m_rotation.y);
		float sp = Math::Sin(m_rotation.x);
		float cp = Math::Cos(m_rotation.x);
		float sy = Math::Sin(m_rotation.z);
		float cy = Math::Cos(m_rotation.z);

		float _11 = cp * cy;
		float _12 = sr * sp * cy - cr * sy;
		float _13 = cr * sp * cy + sr * sy;
		float _21 = cp * sy;
		float _22 = sr * sp * sy + cr * cy;
		float _23 = cr * sp * sy - sr * cy;
		float _31 = -sp;
		float _32 = sr * cp;
		float _33 = cr * cp;
		m_matrix._11 = _11 * m_scaling.x;
		m_matrix._12 = _12 * m_scaling.y;
		m_matrix._13 = _13 * m_scaling.z;
		m_matrix._14 = 0.0f;
		m_matrix._21 = _21 * m_scaling.x;
		m_matrix._22 = _22 * m_scaling.y;
		m_matrix._23 = _23 * m_scaling.z;
		m_matrix._24 = 0.0f;
		m_matrix._31 = _31 * m_scaling.x;
		m_matrix._32 = _32 * m_scaling.y;
		m_matrix._33 = _33 * m_scaling.z;
		m_matrix._34 = 0.0f;
		m_matrix._41 = (m_center.x * _11 + m_center.y * _21 + m_center.z * _31) * m_scaling.x + m_translation.x;
		m_matrix._42 = (m_center.x * _12 + m_center.y * _22 + m_center.z * _32) * m_scaling.y + m_translation.y;
		m_matrix._43 = (m_center.x * _13 + m_center.y * _23 + m_center.z * _33) * m_scaling.z + m_translation.z;
		m_matrix._44 = 1.0f;
	}
}
void Xe::Game::Component::Position2d::UpdatePosition2() {
	if (m_isTranslationInvalidated) {
		m_isTranslationInvalidated = false;
		m_isScalingInvalidated = true;
		if (m_isScalingInvalidated) {
			m_isScalingInvalidated = false;

			float roty;
			float rotx;
			if (m_isRotationXYInvalidated) {
				m_isRotationXYInvalidated = false;
				rotx = Math::Cos(m_rotation.x);
				roty = Math::Cos(m_rotation.y);
			}
			else {
				rotx = 1.0f;
				roty = 1.0f;
			}
			if (m_isRotatedZ) {
				float sy = Math::Sin(m_rotation.z);
				float cy = Math::Cos(m_rotation.z);

				float ycy = roty * cy;
				float xsy = rotx * sy;
				float ysy = roty * -sy;
				float xcy = rotx * cy;
				float _11 = xcy * m_scaling.x;
				float _12 = ysy * m_scaling.y;
				float _21 = xsy * m_scaling.x;
				float _22 = ycy * m_scaling.y;
				float _41 = (m_center.x * xcy + m_center.y * xsy) * m_scaling.x + m_translation.x;
				float _42 = (m_center.x * ysy + m_center.y * ycy) * m_scaling.y + m_translation.y;

				m_position2[0].x = _41 - _11 + _21;
				m_position2[0].y = _42 - _12 + _22;
				m_position2[1].x = _41 + _11 + _21;
				m_position2[1].y = _42 + _12 + _22;
				m_position2[2].x = _41 - _11 - _21;
				m_position2[2].y = _42 - _12 - _22;
				m_position2[3].x = _41 + _11 - _21;
				m_position2[3].y = _42 + _12 - _22;
			}
			else {
				float _11 = rotx * m_scaling.x;
				float _22 = roty * m_scaling.y;
				float _41 = m_center.x * rotx * m_scaling.x + m_translation.x;
				float _42 = m_center.y * roty * m_scaling.y + m_translation.y;
				m_position2[0].x = _41 - _11;
				m_position2[0].y = _42 + _22;
				m_position2[1].x = _41 + _11;
				m_position2[1].y = _42 + _22;
				m_position2[2].x = _41 - _11;
				m_position2[2].y = _42 - _22;
				m_position2[3].x = _41 + _11;
				m_position2[3].y = _42 - _22;
			}
		}
		else {
			// Se è stata effettuata solo una traslazione, non è necessario ricalcolare
			// tutto quanto.
			// TODO trovare un modo per gestire efficientemente la cosa.
		}
	}
}