#include "pch.h"
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMathMatrix.h>
#include <XeSDK/XeMathVector3.h>

Xe::Math::Matrix4::Matrix4() {

}
Xe::Math::Matrix4::Matrix4(float _11, float _12, float _13, float _14,
	float _21, float _22, float _23, float _24,
	float _31, float _32, float _33, float _34,
	float _41, float _42, float _43, float _44) :
	_11(_11), _12(_12), _13(_13), _14(_14),
	_21(_21), _22(_22), _23(_23), _24(_24),
	_31(_31), _32(_32), _33(_33), _34(_34),
	_41(_41), _42(_42), _43(_43), _44(_44)
{}

Xe::Math::Matrix4 Xe::Math::Matrix4::Multiply(const Matrix4& m) const {
	return Matrix4(_11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41,
		_11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42,
		_11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43,
		_11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44,
		_21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41,
		_21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42,
		_21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43,
		_21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44,
		_31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41,
		_31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42,
		_31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43,
		_31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44,
		_41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41,
		_41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42,
		_41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43,
		_41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44);
}
Xe::Math::Matrix4 Xe::Math::Matrix4::Inverse() const {
	Matrix4 out;
	float a[6];
	float b[4];
	float det;

	a[0] = (_33 * _44) - (_34 * _43);
	a[1] = (_32 * _44) - (_34 * _42);
	a[2] = (_32 * _43) - (_33 * _42);
	a[3] = (_31 * _44) - (_34 * _41);
	a[4] = (_31 * _43) - (_33 * _41);
	a[5] = (_31 * _42) - (_32 * _41);
	b[0] = ((_22 * a[0]) - (_23 * a[1])) + (_24 * a[2]);
	b[1] = -(((_21 * a[0]) - (_23 * a[3])) + (_24 * a[4]));
	b[2] = ((_21 * a[1]) - (_22 * a[3])) + (_24 * a[5]);
	b[3] = -(((_21 * a[2]) - (_22 * a[4])) + (_23 * a[5]));
	det = 1.0f / ((((_11 * b[0]) + (_12 * b[1])) + (_13 * b[2])) + (_14 * b[3]));
	out._11 = b[0] * det;
	out._21 = b[1] * det;
	out._31 = b[2] * det;
	out._41 = b[3] * det;
	out._12 = -(((_12 * a[0]) - (_13 * a[1])) + (_14 * a[2])) * det;
	out._22 = (((_11 * a[0]) - (_13 * a[3])) + (_14 * a[4])) * det;
	out._32 = -(((_11 * a[1]) - (_12 * a[3])) + (_14 * a[5])) * det;
	out._42 = (((_11 * a[2]) - (_12 * a[4])) + (_13 * a[5])) * det;
	a[0] = (_23 * _44) - (_24 * _43);
	a[1] = (_22 * _44) - (_24 * _42);
	a[2] = (_22 * _43) - (_23 * _42);
	a[3] = (_21 * _44) - (_24 * _41);
	a[4] = (_21 * _43) - (_23 * _41);
	a[5] = (_21 * _42) - (_22 * _41);
	out._13 = (((_12 * a[0]) - (_13 * a[1])) + (_14 * a[2])) * det;
	out._23 = -(((_11 * a[0]) - (_13 * a[3])) + (_14 * a[4])) * det;
	out._33 = (((_11 * a[1]) - (_12 * a[3])) + (_14 * a[5])) * det;
	out._43 = -(((_11 * a[2]) - (_12 * a[4])) + (_13 * a[5])) * det;
	a[0] = (_23 * _34) - (_24 * _33);
	a[1] = (_22 * _34) - (_24 * _32);
	a[2] = (_22 * _33) - (_23 * _32);
	a[3] = (_21 * _34) - (_24 * _31);
	a[4] = (_21 * _33) - (_23 * _31);
	a[5] = (_21 * _32) - (_22 * _31);
	out._14 = -(((_12 * a[0]) - (_13 * a[1])) + (_14 * a[2])) * det;
	out._24 = (((_11 * a[0]) - (_13 * a[3])) + (_14 * a[4])) * det;
	out._34 = -(((_11 * a[1]) - (_12 * a[3])) + (_14 * a[5])) * det;
	out._44 = (((_11 * a[2]) - (_12 * a[4])) + (_13 * a[5])) * det;
	return out;
}

Xe::Math::Matrix4 &Xe::Math::Matrix4::Translate(float x, float y, float z) {
	_11 = 1.0f;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;
	_21 = 0.0f;
	_22 = 1.0f;
	_23 = 0.0f;
	_24 = 0.0f;
	_31 = 0.0f;
	_32 = 0.0f;
	_33 = 1.0f;
	_34 = 0.0f;
	_41 = x;
	_42 = y;
	_43 = z;
	_44 = 1.0f;
	return *this;
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::Translate(const Math::Vector3<float> &translate) {
	return Translate(translate.x, translate.y, translate.z);
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::Scale(float x, float y, float z) {
	_11 = x;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;
	_21 = 0.0f;
	_22 = y;
	_23 = 0.0f;
	_24 = 0.0f;
	_31 = 0.0f;
	_32 = 0.0f;
	_33 = z;
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
	return *this;
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::Scale(const Math::Vector3<float> &scale) {
	return Scale(scale.x, scale.y, scale.z);
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::RotateX(float radiant) {
	float s = Math::Sin(radiant);
	float c = Math::Cos(radiant);
	_11 = 1.0f;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;
	_21 = 0.0f;
	_22 = c;
	_23 = s;
	_24 = 0.0f;
	_31 = 0.0f;
	_32 = -s;
	_33 = c;
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
	return *this;
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::RotateY(float radiant) {
	float s = Math::Sin(radiant);
	float c = Math::Cos(radiant);
	_11 = c;
	_12 = 0.0f;
	_13 = -s;
	_14 = 0.0f;
	_21 = 0.0f;
	_22 = 1.0f;
	_23 = 0.0f;
	_24 = 0.0f;
	_31 = s;
	_32 = 0.0f;
	_33 = c;
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
	return *this;
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::RotateZ(float radiant) {
	float s = Math::Sin(radiant);
	float c = Math::Cos(radiant);
	_11 = c;
	_12 = -s;
	_13 = 0.0f;
	_14 = 0.0f;
	_21 = s;
	_22 = c;
	_23 = 0.0f;
	_24 = 0.0f;
	_31 = 0.0f;
	_32 = 0.0f;
	_33 = 1.0f;
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
	return *this;
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::Rotation(float x, float y, float z) {
	float sr = Math::Sin(y);
	float cr = Math::Cos(y);
	float sp = Math::Sin(x);
	float cp = Math::Cos(x);
	float sy = Math::Sin(z);
	float cy = Math::Cos(z);
	float srsp = sr * sp;
	float crsp = cr * sp;

	_11 = (cp * cy);
	_12 = (srsp * cy - cr * sy);
	_13 = (crsp * cy + sr * sy);
	_14 = 0.0f;
	_21 = (cp * sy);
	_22 = (srsp * sy + cr * cy);
	_23 = (crsp * sy - sr * cy);
	_24 = 0.0f;
	_31 = (-sp);
	_32 = (sr * cp);
	_33 = (cr * cp);
	_34 = 0.0f;
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
	return *this;
}
Xe::Math::Matrix4 &Xe::Math::Matrix4::Rotation(const Math::Vector3<float> &rotation) {
	return Rotation(rotation.x, rotation.y, rotation.z);
}

Xe::Math::Matrix4 Xe::Math::Matrix4::Identity() {
	return Matrix4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
Xe::Math::Matrix4 Xe::Math::Matrix4::Ortho(float left, float right, float bottom, float top, float Znear, float Zfar) {
	return Matrix4(
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f / (Zfar - Znear), 0.0f,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(Zfar + Znear) / (Zfar - Znear), 1.0f
	);
}