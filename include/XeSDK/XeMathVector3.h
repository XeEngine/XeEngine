#pragma once
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMathMatrix.h>

namespace Xe {
	namespace Math {
		template <typename T>
		struct Vector3 {
		public:
			T x, y, z;

			Vector3() : x(0), y(0), z(0) {}
			Vector3(T v) : x(v), y(v), z(v) {}
			Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
			Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

			Vector3& operator = (const Vector3& v) {
				x = v.x;
				y = v.y;
				z = v.z;
				return *this;
			}
			bool operator == (const Vector3& v) const {
				return x == v.x && y == v.y && z == v.z;
			}
			bool operator != (const Vector3& v) const {
				return x != v.x || y != v.y || z != v.z;
			}
			Vector3 operator + (const Vector3& v) const {
				return Vector3(x + v.x, y + v.y, z + v.z);
			}
			Vector3& operator += (const Vector3& v) {
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}
			Vector3 operator - (const Vector3& v) const {
				return Vector3(x - v.x, y - v.y, z - v.z);
			}
			Vector3 operator - () const {
				return Vector3(-x, -y, -z);
			}
			Vector3& operator -= (const Vector3& v) {
				x -= v.x;
				y -= v.y;
				z -= v.z;
				return *this;
			}
			Vector3 operator * (const Vector3& v) const {
				return Vector3(x * v.x, y * v.y, z * v.z);
			}
			Vector3& operator *= (const Vector3& v) {
				x *= v.x;
				y *= v.y;
				z *= v.z;
				return *this;
			}
			Vector3 operator / (const Vector3& v) const {
				return Vector3(x / v.x, y / v.y, z / v.z);
			}
			Vector3& operator /= (const Vector3& v) {
				x /= v.x;
				y /= v.y;
				z /= v.z;
				return *this;
			}

			void Set(T x, T y, T z) {
				this->x = x;
				this->y = y;
				this->z = z;
			}
			T Distance(const Vector3& v) const {
				T n1 = x - v.x;
				T n2 = y - v.y;
				T n3 = z - v.z;
				return Math::Sqrt(n1 * n1 + n2 * n2 + n3 * n3);
			}
			T Length() const {
				return Math::Sqrt(x * x + y * y + z * z);
			}
			T Dot(const Vector3& v) const {
				return x * v.x + y * v.y + z * v.z;
			}
			Vector3 Min(const Vector3& v) const {
				return Vector3(Math::Min(x, v.x), Math::Min(y, v.y), Math::Min(z, v.z));
			}
			Vector3 Max(const Vector3& v) const {
				return Vector3(Math::Max(x, v.x), Math::Max(y, v.y), Math::Max(z, v.z));
			}
			Vector3 Clamp(const Vector3& min, const Vector3& max) const {
				return Vector3(Math::Min(Math::Max(x, min.x), max.x),
					Math::Min(Math::Max(y, min.y), max.y),
					Math::Min(Math::Max(z, min.z), max.z));
			}
			Vector3 Lerp(const Vector3& v, T amount) const {
				return Vector3(x + (v.x - x) * amount, y + (v.y - y) * amount, z + (v.z - z) * amount);
			}
			Vector3 Normalize() const {
				T normalized = (T)1.0 / Length();
				return Vector3(x * normalized, y * normalized, z * normalized);
			}
			Vector3 Transform(const Matrix4& matrix) const {
				return Vector3(
					((x * matrix._11) + (y * matrix._21) + (z * matrix._31)) + matrix._41,
					((x * matrix._12) + (y * matrix._22) + (z * matrix._32)) + matrix._42,
					((x * matrix._13) + (y * matrix._23) + (z * matrix._33)) + matrix._43);
			}
		};

		typedef Vector3<float> Vector3f;
		typedef Vector3<double> Vector3d;
	}
}