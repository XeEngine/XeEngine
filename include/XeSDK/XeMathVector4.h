#pragma once
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMathMatrix.h>

namespace Xe {
	namespace Math {
		template <typename T>
		struct Vector4 {
		public:
			T x, y, z, w;

			Vector4() : x(0), y(0), z(0), w(0) {}
			Vector4(T v) : x(v), y(v), z(v), w(v) {}
			Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
			Vector4(const Vector2<T>& v, T z, T w) : x(v.x), y(v.y), z(z), w(w) {}
			Vector4(float x, const Vector2<T>& v, T w) : x(x), y(v.x), z(v.y), w(w) {}
			Vector4(float x, float y, const Vector2<T>& v) : x(x), y(y), z(v.x), w(v.y) {}
			Vector4(const Vector2<T>& v1, const Vector2<T>& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
			Vector4(const Vector3<T>& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
			Vector4(float x, const Vector3<T>& v) : x(x), y(v.x), z(v.y), w(v.z) {}
			Vector4(const Vector4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

			Vector4& operator = (const Vector4& v) {
				x = v.x;
				y = v.y;
				z = v.z;
				w = v.w;
				return *this;
			}
			bool operator == (const Vector4& v) const {
				return x == v.x && y == v.y && z == v.z && w == v.w;
			}
			bool operator != (const Vector4& v) const {
				return x != v.x || y != v.y || z != v.z || w != v.w;
			}
			Vector4 operator + (const Vector4& v) const {
				return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
			}
			Vector4& operator += (const Vector4& v) {
				x += v.x;
				y += v.y;
				z += v.z;
				w += v.w;
				return *this;
			}
			Vector4 operator - (const Vector4& v) const {
				return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
			}
			Vector4 operator - () const {
				return Vector4(-x, -y, -z, -w);
			}
			Vector4& operator -= (const Vector4& v) {
				x -= v.x;
				y -= v.y;
				z -= v.z;
				w -= v.w;
				return *this;
			}
			Vector4 operator * (T scalar) const {
				return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
			}
			Vector4& operator *= (T scalar) {
				x *= scalar;
				y *= scalar;
				z *= scalar;
				w *= scalar;
				return *this;
			}
			Vector4 operator * (const Vector4& v) const {
				return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
			}
			Vector4& operator *= (const Vector4& v) {
				x *= v.x;
				y *= v.y;
				z *= v.z;
				w *= v.w;
				return *this;
			}
			Vector4 operator / (T scalar) const {
				return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
			}
			Vector4& operator /= (T scalar) {
				x /= scalar;
				y /= scalar;
				z /= scalar;
				w /= scalar;
				return *this;
			}
			Vector4 operator / (const Vector4& v) const {
				return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
			}
			Vector4& operator /= (const Vector4& v) {
				x /= v.x;
				y /= v.y;
				z /= v.z;
				w /= v.w;
				return *this;
			}

			void Set(T x, T y, T z, T w) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}
			T Distance(const Vector4& v) const {
				T n1 = x - v.x;
				T n2 = y - v.y;
				T n3 = z - v.z;
				T n4 = w - v.w;
				return Math::Sqrt(n1 * n1 + n2 * n2 + n3 * n3 + n4 * n4);
			}
			T Length() const {
				return Math::Sqrt(x * x + y * y + z * z + w * w);
			}
			T Dot(const Vector4& v) const {
				return x * v.x + y * v.y + z * v.z + w * v.w;
			}
			Vector4 Min(const Vector4& v) const {
				return Vector4(Math::Min(x, v.x), Math::Min(y, v.y), Math::Min(z, v.z), Math::Min(w, v.w));
			}
			Vector4 Max(const Vector4& v) const {
				return Vector4(Math::Max(x, v.x), Math::Max(y, v.y), Math::Max(z, v.z), Math::Min(w, v.w));
			}
			Vector4 Clamp(const Vector4& min, const Vector4& max) const {
				return Vector4(Math::Min(Math::Max(x, min.x), max.x),
					Math::Min(Math::Max(y, min.y), max.y),
					Math::Min(Math::Max(z, min.z), max.z),
					Math::Min(Math::Max(w, min.w), max.w));
			}
			Vector4 Lerp(const Vector4& v, T amount) const {
				return Vector4(x + (v.x - x) * amount, y + (v.y - y) * amount, z + (v.z - z) * amount, w + (v.w - w) * amount);
			}
			Vector4 Normalize() const {
				T normalized = (T)1.0 / Length();
				return Vector4(x * normalized, y * normalized, z * normalized, w * normalized);
			}
			Vector4 Transform(const Matrix4& matrix) const {
				return Vector4(
					((x * matrix._11) + (y * matrix._21) + (z * matrix._31)) + w * matrix._41,
					((x * matrix._12) + (y * matrix._22) + (z * matrix._32)) + w * matrix._42,
					((x * matrix._13) + (y * matrix._23) + (z * matrix._33)) + w * matrix._43,
					((x * matrix._14) + (y * matrix._24) + (z * matrix._34)) + w * matrix._44);
			}
		};

		typedef Vector4<int> Vector4i;
		typedef Vector4<uint> Vector4u;
		typedef Vector4<float> Vector4f;
		typedef Vector4<double> Vector4d;
	}
}