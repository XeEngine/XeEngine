#pragma once
#include <XeSDK/XeMath.h>

namespace Xe { namespace Game {
	typedef float(*EaseFuncf)(float x);
	typedef double(*EaseFuncd)(double x);

	struct Ease
	{
		template <typename T> static T Zero(T x) { return (T)0.0; }
		template <typename T> static T Half(T x) { return (T)0.5; }
		template <typename T> static T One(T x) { return (T)1.0; }
		template <typename T> static T Linear(T x) { return x; }
		template <typename T> static T Floor(T x) { return Xe::Math::Floor(x); }
		template <typename T> static T Ceiling(T x) { return Xe::Math::Ceiling(x); }
		template <typename T> static T Round(T x) { return Xe::Math::Round(x); }
		template <typename T> static T Quadratic(T x) { return x > 0.5 ? 2.0 * x * x : ((-2.0 * x * x) + (4.0 * x) - 1.0); }
		template <typename T> static T QuadraticEaseIn(T x) { return x * x; }
		template <typename T> static T QuadraticEaseOut(T x) { return -(x * (x - 2)); }
		template <typename T> static T CubicEaseIn(T x) { return x * x * x; }
		template <typename T> static T CubicEaseOut(T x) { T f = (x - 1.0); return f * f * f + 1.0; }
		template <typename T> static T QuarticEaseIn(T x) { return x * x * x * x; }
		template <typename T> static T QuarticEaseOut(T x) { T f = (x - 1.0); return f * f * f * (1 - x) + 1; }
		template <typename T> static T QuarticEaseInOut(T x)
		{
			if (x < 0.5)
				return 8 * x * x * x * x;
			T f = (x - 1);
			return -8.0 * f * f * f * f + 1;
		}
		template <typename T> static T QuinticEaseIn(T x) { return x * x * x * x * x; }
		template <typename T> static T QuinticEaseOut(T x) { T f = (x - 1.0); f * f * f * f * f + 1; }
		template <typename T> static T QuinticEaseInOut(T x)
		{
			if (x < 0.5)
				return 16.0 * x * x * x * x * x;
			T f = ((2 * x) - 2.0);
			return 0.5 * f * f * f * f * f + 1;
		}
		template <typename T> static T SineEaseIn(T x) { return Xe::Math::Sin((x - 1) * Xe::Math::PI_DIV2) + 1.0; }
		template <typename T> static T SineEaseOut(T x) { return Xe::Math::Sin(x * Xe::Math::PI_DIV2); }
		template <typename T> static T SineEaseInOut(T x) { return 0.5 * (1.0 - Xe::Math::Cos(x * Xe::Math::PI)); }
		template <typename T> static T CircularEaseIn(T x) { return 1 - Xe::Math::Sqrt(1 - (x * x)); }
		template <typename T> static T CircularEaseOut(T x) { return Xe::Math::Sqrt((2 - x) * x); }
		template <typename T> static T CircularEaseInOut(T x)
		{
			if (x < 0.5)
				return 0.5 * (1 - Xe::Math::Sqrt(1 - 4 * (x * x)));
			return 0.5 * (Xe::Math::Sqrt(-((2 * x) - 3) * ((2 * x) - 1)) + 1);
		}
	};
} }