#pragma once
#include <XeSDK/XeMath.h>
#include <XeSDK/XeMathMatrix.h>

namespace Xe {
	namespace Math {
		template <typename T>
		struct Vector2 {
		public:
			T x, y;

			Vector2() : x((T)0), y((T)0) { }
			Vector2(T v) : x(v), y(v) { }
			Vector2(T x, T y) : x(x), y(y) { }
			Vector2(const Vector2& v) : x(v.x), y(v.y) { }

			Vector2& operator = (const Vector2& v) {
				x = v.x;
				y = v.y;
				return *this;
			}
			bool operator == (const Vector2& v) const {
				return x == v.x && y == v.y;
			}
			bool operator != (const Vector2& v) const {
				return x != v.x || y != v.y;
			}
			Vector2 operator + (const Vector2& v) const {
				return Vector2(x + v.x, y + v.y);
			}
			Vector2& operator += (const Vector2& v) {
				x += v.x;
				y += v.y;
				return *this;
			}
			Vector2 operator - (const Vector2& v) const {
				return Vector2(x - v.x, y - v.y);
			}
			Vector2 operator - () const {
				return Vector2(-x, -y);
			}
			Vector2& operator -= (const Vector2& v) {
				x -= v.x;
				y -= v.y;
				return *this;
			}
			Vector2 operator * (const Vector2& v) const {
				return Vector2(x * v.x, y * v.y);
			}
			Vector2& operator *= (const Vector2& v) {
				x *= v.x;
				y *= v.y;
				return *this;
			}
			Vector2 operator / (const Vector2& v) const {
				return Vector2(x / v.x, y / v.y);
			}
			Vector2& operator /= (const Vector2& v) {
				x /= v.x;
				y /= v.y;
				return *this;
			}

			void Set(T x, T y) {
				this->x = x;
				this->y = y;
			}
			T Distance(const Vector2& v) const {
				T n1 = x - v.x;
				T n2 = y - v.y;
				return Math::Sqrt(n1 * n1 + n2 * n2);
			}
			T Length() const {
				return Math::Sqrt(x * x + y * y);
			}
			T Dot(const Vector2& v) const {
				return x * v.x + y * v.y;
			}
			Vector2 Min(const Vector2& v) const {
				return Vector2(Math::Min(x, v.x), Math::Min(y, v.y));
			}
			Vector2 Max(const Vector2& v) const {
				return Vector2(Math::Max(x, v.x), Math::Max(y, v.y));
			}
			Vector2 Clamp(const Vector2& min, const Vector2& max) const {
				return Vector2(Math::Min(Math::Max(x, min.x), max.x),
					Math::Min(Math::Max(y, min.y), max.y));
			}
			Vector2 Lerp(const Vector2& v, T amount) const {
				return Vector2(x + (v.x - x) * amount, y + (v.y - y) * amount);
			}
			Vector2 Normalize() const {
				T normalized = (T)1.0 / Length();
				return Vector2(x * normalized, y * normalized);
			}
			Vector2 Transform(const Matrix4& matrix) const {
				return Vector2(
					((x * matrix._11) + (y * matrix._21)) + matrix._41,
					((x * matrix._12) + (y * matrix._22)) + matrix._42);
			}
		};

#define Point2 Vector2
		typedef Vector2<int> Vector2i;
		typedef Vector2<uint> Vector2u;
		typedef Vector2<float> Vector2f;
		typedef Vector2<double> Vector2d;
		typedef Vector2<int> Point2i;
		typedef Vector2<uint> Point2u;
		typedef Vector2<float> Point2f;
		typedef Vector2<double> Point2d;
	}
}