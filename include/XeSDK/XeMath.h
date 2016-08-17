#pragma once
#include <XeSDK/XeDef.h>

namespace Xe
{
	//! \brief mathematics library
	/* \details most of this content is a wrapper of cmath:
	 * http://www.cplusplus.com/reference/cmath/
	 */
	namespace Math
	{
		//! \brief E
		static const double E = 2.7182818284590451;

		//! \brief PI
		static const double PI_DIV4 = 0.78539816339744830961566084581975;
		static const double PI_DIV2 = 1.5707963267948966192313216916395;
		static const double PI = 3.141592653589793238462643383279;
		static const double PI_2 = 6.283185307179586476925286766558;
		static const double PI_1DIVPI2 = 0.15915494309189533576888376337254;
		static const double PI_1DIVPI = 0.31830988618379067153776752674508;

		//! \brief absolute value
		//! \param[in] x value to process
		//! \return result
		/** \details
		* Abs(3) = 3
		* Abs(-3) = 3
		* Abs(0) = 0
		*/
		template <class T>
		T Abs(T x)
		{
			return x < 0 ? -x : x;
		}

		//! \brief minimum value
		//! \param[in] x value to process
		//! \param[in] y value to process
		//! \return result
		/** \details
		* Min(1, 2) = 1
		* Min(2, 1) = 1
		* Min(3, 3) = 3
		*/
		template <class T>
		T Min(T x, T y)
		{
			return x <= y ? x : y;
		}

		//! \brief maximum value
		//! \param[in] x value to process
		//! \param[in] y value to process
		//! \return result
		/** \details
		* Max(1, 2) = 2
		* Max(2, 1) = 2
		* Max(3, 3) = 3
		*/
		template <class T>
		T Max(T x, T y)
		{
			return x >= y ? x : y;
		}

		//! \brief combine minimum and maximum to block a value between the specified range
		//! \param[in] x value to process
		//! \param[in] min minimum value
		//! \param[in] max maximum value
		//! \details if min is greater than max then min = max and max = min
		template <class T>
		T Range(T x, T min, T max) {
			return min < max ?
				Math::Max(min, Math::Min(max, x)) :
				Math::Max(max, Math::Min(min, x));
		}

		//! \brief get the sign of a value
		//! \param[in] x value to process
		//! \return -1 if < 0, else +1
		template <class T>
		T Sign(T x)
		{
			return x < 0 ? (T)-1 : (T)+1;
		}

		//! \brief align a value
		//! \param[in] x value to align
		//! \param[in] y alignment value
		//! \return aligned value
		svar Align(svar x, svar y);

		//! \brief arc sine in radiants
		//! \param[in] x value to process
		//! \return result
		double Asin(double x);
		//! \brief arc sine in radiants
		//! \param[in] x value to process
		//! \return result
		float Asin(float x);

		//! \brief arc cosine in radiants
		//! \param[in] x value to process
		//! \return result
		double Acos(double x);
		//! \brief arc cosine in radiants
		//! \param[in] x value to process
		//! \return result
		float Acos(float x);

		//! \brief arc tangent in radiants
		//! \param[in] x value to process
		//! \return result
		double Atan(double x);
		//! \brief arc tangent in radiants
		//! \param[in] x value to process
		//! \return result
		float Atan(float x);

		//! \brief arc tangent in radiants
		//! \param[in] x value to process
		//! \param[in] y value to process
		//! \return result
		double Atan2(double x, double y);
		//! \brief arc tangent in radiants
		//! \param[in] x value to process
		//! \param[in] y value to process
		//! \return result
		float Atan2(float x, float y);

		//! \brief rounds upward
		//! \param[in] x value to process
		//! \return result
		double Ceiling(double x);
		//! \brief rounds upward
		//! \param[in] x value to process
		//! \return result
		float Ceiling(float x);

		//! \brief cosine in radiants
		//! \param[in] x value to process
		//! \return result
		double Cos(double x);
		//! \brief cosine in radiants
		//! \param[in] x value to process
		//! \return result
		float Cos(float x);

		//! \brief exponential value
		//! \param[in] x value to process
		//! \return result
		double Exp(double x);
		//! \brief exponential value
		//! \param[in] x value to process
		//! \return result
		float Exp(float x);

		//! \brief rounds downward
		//! \param[in] x value to process
		//! \return result
		double Floor(double x);
		//! \brief rounds downward
		//! \param[in] x value to process
		//! \return result
		float Floor(float x);

		//! \brief get remainder
		//! \param[in] numerator
		//! \param[in] denominator
		//! \return result
		double Fmod(double numerator, double denominator);
		//! \brief get remainder
		//! \param[in] numerator
		//! \param[in] denominator
		//! \return result
		float Fmod(float numerator, float denominator);

		//! \brief natural logarithm
		//! \param[in] x value to process
		//! \return result
		double Log(double x);
		//! \brief natural logarithm
		//! \param[in] x value to process
		//! \return result
		float Log(float x);

		//! \brief logarithm with base 10
		//! \param[in] x value to process
		//! \return result
		double Log10(double x);
		//! \brief logarithm with base 10
		//! \param[in] x value to process
		//! \return result
		float Log10(float x);

		//! \brief perform base^exponent operation
		//! \param[in] base value to process
		//! \param[in] exponent value to process
		//! \return result
		double Pow(double base, double exponent);
		//! \brief perform base^exponent operation
		//! \param[in] base value to process
		//! \param[in] exponent value to process
		//! \return result
		float Pow(float base, float exponent);

		//! \brief integer nearest
		//! \param[in] x value to process
		//! \return result
		double Round(double x);
		//! \brief integer nearest
		//! \param[in] x value to process
		//! \return result
		float Round(float x);

		//! \brief sinus in radiants
		//! \param[in] x value to process
		//! \return result
		double Sin(double x);
		//! \brief sinus in radiants
		//! \param[in] x value to process
		//! \return result
		float Sin(float x);

		//! \brief calculate square root
		//! \param[in] x value to process
		//! \return square root of x
		double Sqrt(double x);
		//! \brief calculate square root
		//! \param[in] x value to process
		//! \return square root of x
		float Sqrt(float x);

		//! \brief invert the endian of specified 16-bit value
		//! \param[in] x value to invert
		//! \return x with inverted endian
		u16 Swap16(u16 x);

		//! \brief invert the endian of specified 32-bit value
		//! \param[in] x value to invert
		//! \return x with inverted endian
		u32 Swap32(u32 x);

		//! \brief invert the endian of specified 64-bit value
		//! \param[in] x value to invert
		//! \return x with inverted endian
		u64 Swap64(u64 x);

		//! \brief tangent in radiants
		//! \param[in] x value to process
		//! \return result
		double Tan(double x);
		//! \brief tangent in radiants
		//! \param[in] x value to process
		//! \return result
		float Tan(float x);

		//! \brief truncate decimal value
		//! \param[in] x value to process
		//! \return result
		double Truncate(double x);
		//! \brief truncate decimal value
		//! \param[in] x value to process
		//! \return result
		float Truncate(float x);

		//! \brief normalize a value between a range
		//! \param[in] value to normalize
		//! \param[in] x1 first point of the range
		//! \param[in] x2 second point of the range
		//! \return normalized value between [0, 1]
		float Normalize(float value, float x1, float x2);
		//! \brief normalize a value between a range
		//! \param[in] value to normalize
		//! \param[in] x1 first point of the range
		//! \param[in] x2 second point of the range
		//! \return normalized value between [0, 1]
		double Normalize(double value, double x1, double x2);

		//! \brief normalize a value between a range
		//! \param[in] value to normalize
		//! \param[in] x1 first point of the range
		//! \param[in] x2 second point of the range
		//! \return normalized value
		/** \details if value is between x1 and x2, a value between [0, 1] is
		* returned. If x1 > x2, returned value is negative but normaized.
		* If value is outside the [x1, x2] range, a value outside [0, 1] is
		* returned.
		*/
		float NormalizeUnsecure(float value, float x1, float x2);
		//! \brief normalize a value between a range
		//! \param[in] value to normalize
		//! \param[in] x1 first point of the range
		//! \param[in] x2 second point of the range
		//! \return normalized value between [0, 1]
		/** \details if value is between x1 and x2, a value between [0, 1] is
		* returned. If x1 > x2, returned value is negative but normaized.
		* If value is outside the [x1, x2] range, a value outside [0, 1] is
		* returned.
		*/
		double NormalizeUnsecure(double value, double x1, double x2);

		//! \brief calculate a nearest neighbor interpolation
		//! \param[in] m position between [0, 1]
		//! \param[in] x1 first point
		//! \param[in] x2 second point
		//! \return interpolated value
		/** \details fastest way to interpolate. If m is < x2, then x1 is
		* returned, else x2.
		*/
		float InterpolateNearestNeighbor(float m, float x1, float x2);
		//! \brief calculate a nearest neighbor interpolation
		//! \param[in] m position between [0, 1]
		//! \param[in] x1 first point
		//! \param[in] x2 second point
		//! \return interpolated value
		/** \details fastest way to interpolate. If m is < x2, then x1 is
		* returned, else x2.
		*/
		double InterpolateNearestNeighbor(double m, double x1, double x2);

		//! \brief calculate linear interpolation
		//! \param[in] m position between [0, 1]
		//! \param[in] x1 first point
		//! \param[in] x2 second point
		//! \return interpolated value between [x1, x2]
		/** \details very fast interpolation; more the m value is
		* near to 1, more returned value is near to x2 than x1.
		*/
		float Lerp(float m, float x1, float x2);
		//! \brief calculate linear interpolation
		//! \param[in] m position between [0, 1]
		//! \param[in] x1 first point
		//! \param[in] x2 second point
		//! \return interpolated value between [x1, x2]
		/** \details very fast interpolation; more the m value is
		* near to 1, more returned value is near to x2 than x1.
		*/
		double Lerp(double m, double x1, double x2);

		//! \brief calculate sine interpolation
		//! \param[in] m position between [0, 1]
		//! \param[in] x1 first point
		//! \param[in] x2 second point
		//! \return interpolated value
		/** \details fast interpolation that make uses Sin function. Similar
		* to InterpolateLinear but it decelerates near x1 and x2.
		*/
		float InterpolateSine(float m, float x1, float x2);
		//! \brief calculate sine interpolation
		//! \param[in] m position between [0, 1]
		//! \param[in] x1 first point
		//! \param[in] x2 second point
		//! \return interpolated value between [x1, x2]
		/** \details fast interpolation that make uses Sin function. Similar
		* to InterpolateLinear but it decelerates near x1 and x2.
		*/
		double InterpolateSine(double m, double x1, double x2);
	}
}
