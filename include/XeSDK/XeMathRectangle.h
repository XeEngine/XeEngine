#pragma once
#include <XeSDK/XeMathVector2.h>

namespace Xe {
	namespace Math {
		//! \brief Specify a rectangle
		template <typename T>
		struct Rectangle {
			T left, top, right, bottom;

			Rectangle() : left(0), top(0), right(0), bottom(0) {}
			Rectangle(T left, T top, T right, T bottom) :
				left(left), top(top), right(right), bottom(bottom) {}
			Rectangle(const Rectangle& rect) :
				left(rect.left), top(rect.top),
				right(rect.right), bottom(rect.bottom) {}

			Rectangle& operator = (const Rectangle& rect) {
				left = rect.left;
				top = rect.top;
				right = rect.right;
				bottom = rect.bottom;
				return *this;
			}
			bool operator == (const Rectangle& rect) const {
				return left == rect.left &&
					top == rect.top &&
					right == rect.right &&
					bottom == rect.bottom;
			}
			bool operator != (const Rectangle& rect) const {
				return left != rect.left ||
					top != rect.top ||
					right != rect.right ||
					bottom != rect.bottom;
			}
			Rectangle operator + (const Rectangle& rect) const {
				return Rectangle(left + rect.left,
					top + rect.top,
					right + rect.right,
					bottom + rect.bottom);
			}
			Rectangle operator - (const Rectangle& rect) const {
				return Rectangle(left - rect.left,
					top - rect.top,
					right - rect.right,
					bottom - rect.bottom);
			}
			const Rectangle& operator += (const Rectangle& rect) {
				left += rect.left;
				top += rect.top;
				right += rect.right;
				bottom += rect.bottom;
				return *this;
			}
			const Rectangle& operator -= (const Rectangle& rect) {
				left -= rect.left;
				top -= rect.top;
				right -= rect.right;
				bottom -= rect.bottom;
				return *this;
			}

			T GetSourceX() const {
				return left;
			}
			T GetSourceY() const {
				return top;
			}
			T GetWidth() const {
				return right - left;
			}
			T GetHeight() const {
				return bottom - top;
			}

			//! \brief A rectangle is empty when all his values are 0
			bool IsEmpty() const {
				return left == 0 && top == 0 && right == 0 && bottom == 0;
			}

			static Rectangle FromSize(T x, T y, T width, T height) {
				return Rectangle(x, y, x + width, y + height);
			}
			static Rectangle FromSize(const Vector2<T>& pos, const Vector2<T>& size) {
				return Rectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
			}
			Point2<T> AsPosition() const {
				return Point2<T>(left, top);
			}
			Point2<T> AsSize() const {
				return Point2<T>(GetWidth(), GetHeight());
			}

			//! \brief Check if the specified point is contained by the rectangle
			bool Contains(const Point2<T>& point) const {
				return Contains(point.x, point.y);
			}
			//! \brief Check if the specified point is contained by the rectangle
			bool Contains(T x, T y) const {
				return x >= left && x <= right &&
					y >= top && y <= bottom;
			}
			//! \brief Normalize the specified point
			Point2<float> Normalize(const Point2<T>& point) const {
				return Normalize(point.x, point.y);
			}
			//! \brief Normalize the specified point
			Point2<double> Normalize(const Point2<double>& point) const {
				return Normalize(point.x, point.y);
			}
			//! \brief Normalize the specified point
			Point2<float> Normalize(T x, T y) const {
				return Point2<float>(
					NormalizeUnsecure((float)x, (float)left, (float)right),
					NormalizeUnsecure((float)y, (float)top, (float)bottom));
			}
			//! \brief Normalize the specified point
			Point2<double> Normalize(double x, double y) const {
				return Point2<double>(
					NormalizeUnsecure(x, (double)left, (double)right),
					NormalizeUnsecure(y, (double)top, (double)bottom));
			}
			//! \brief Translate the rectangle by specified vector
			Rectangle Translate(const Vector2<T>& vector) const {
				return Rectangle(left + vector.x,
					top + vector.y,
					right + vector.x,
					bottom + vector.y);
			}
			//! \brief Enlarge the rectangle by the specified amount
			Rectangle Inflate(T width, T height) const {
				return Rectangle(left - width,
					top - height,
					right - width,
					bottom - height);
			}
			//! \brief Calculate the intersection between current and specified rectangle
			Rectangle Intersect(const Rectangle& rect) const {
				T left = Max(this->left, rect.left);
				T top = Max(this->top, rect.top);
				T right = Min(this->right, rect.right);
				T bottom = Min(this->bottom, rect.bottom);
				if ((right >= left) && (bottom >= top))
					return Rectangle(left, top, right, bottom);
				return Rectangle();
			}
			// \brief Calculate the union between current and specified rectangle
			Rectangle Union(const Rectangle& rect) const {
				return Rectangle(
					Min(left, rect.left),
					Min(top, rect.top),
					Max(right, rect.right),
					Max(bottom, rect.bottom));
			}

			Rectangle Lerp(const Rectangle& rect, T amount) const {
				return Rectangle(
					Math::Lerp(amount, left, rect.left),
					Math::Lerp(amount, top, rect.top),
					Math::Lerp(amount, right, rect.right),
					Math::Lerp(amount, bottom, rect.bottom)
				);
			}
		};
	
		typedef Rectangle<float> Rectanglef;
	}
}