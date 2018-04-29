#include "pch.h"
#include <cmath>
#include <XeSDK/XeMath.h>

svar Xe::Math::Align(svar x, svar y)
{
	return (x % y) == 0 ? x : x + y - (x % y);
}
double Xe::Math::Asin(double x)
{
	return asin(x);
}
float Xe::Math::Asin(float x)
{
	return asin(x);
}
double Xe::Math::Acos(double x)
{
	return acos(x);
}
float Xe::Math::Acos(float x)
{
	return acos(x);
}
double Xe::Math::Atan(double x)
{
	return atan(x);
}
float Xe::Math::Atan(float x)
{
	return atan(x);
}
double Xe::Math::Atan2(double x, double y)
{
	return atan2(x, y);
}
float Xe::Math::Atan2(float x, float y)
{
	return atan2(x, y);
}
double Xe::Math::Ceiling(double x)
{
	return ceil(x);
}
float Xe::Math::Ceiling(float x)
{
	return ceil(x);
}
double Xe::Math::Cos(double x)
{
	return cos(x);
}
float Xe::Math::Cos(float x)
{
	return cos(x);
}
double Xe::Math::Exp(double x)
{
	return exp(x);
}
float Xe::Math::Exp(float x)
{
	return exp(x);
}
double Xe::Math::Floor(double x)
{
	return floor(x);
}
float Xe::Math::Floor(float x)
{
	return floor(x);
}
double Xe::Math::Fmod(double numerator, double denominator)
{
	return fmod(numerator, denominator);
}
float Xe::Math::Fmod(float numerator, float denominator)
{
	return fmod(numerator, denominator);
}
double Xe::Math::Log(double x)
{
	return log(x);
}
float Xe::Math::Log(float x)
{
	return log(x);
}
double Xe::Math::Log10(double x)
{
	return log10(x);
}
float Xe::Math::Log10(float x)
{
	return log10(x);
}
double Xe::Math::Pow(double base, double exponent)
{
	return pow(base, exponent);
}
float Xe::Math::Pow(float base, float exponent)
{
	return pow(base, exponent);
}
double Xe::Math::Round(double x)
{
	return (double)(x > 0 ? (svar)(x + 0.5) : (svar)(x - 0.5));
}
float Xe::Math::Round(float x)
{
	return (float)(x > 0 ? (svar)(x + 0.5) : (svar)(x - 0.5));
}
double Xe::Math::Sin(double x)
{
	return sin(x);
}
float Xe::Math::Sin(float x)
{
	return sin(x);
}
double Xe::Math::Sqrt(double x)
{
	return sqrt(x);
}
float Xe::Math::Sqrt(float x)
{
	return sqrt(x);
}
u16 Xe::Math::Swap16(u16 x)
{
#if _MSC_VER
	return _byteswap_ushort(x);
#else
	return (x >> 8) | (x << 8);
#endif
}
u32 Xe::Math::Swap32(u32 x)
{
#if _MSC_VER
	return _byteswap_ulong(x);
#else
	return ((x >> 24) & 0xFFL) |
		((x << 8) & 0xFF0000L) |
		((x >> 8) & 0xFF00L) |
		((x << 24) & 0xFF000000L);
#endif
}
u64 Xe::Math::Swap64(u64 x)
{
#if _MSC_VER
	return _byteswap_uint64(x);
#else
	return (((x & 0xFF00000000000000ULL) >> 56) |
		((x & 0x00FF000000000000ULL) >> 40) |
		((x & 0x0000FF0000000000ULL) >> 24) |
		((x & 0x000000FF00000000ULL) >> 8) |
		((x & 0x00000000FF000000ULL) << 8) |
		((x & 0x0000000000FF0000ULL) << 24) |
		((x & 0x000000000000FF00ULL) << 40) |
		((x & 0x00000000000000FFULL) << 56));
#endif
}
double Xe::Math::Tan(double x)
{
	return tan(x);
}
float Xe::Math::Tan(float x)
{
	return tan(x);
}
double Xe::Math::Truncate(double x)
{
	return truncl(x);
}
float Xe::Math::Truncate(float x)
{
	return truncf(x);
}

float Xe::Math::Normalize(float value, float x1, float x2)
{
	float r;
	if (x1 > x2)
		r = NormalizeUnsecure(value, x2, x1);
	else
		r = NormalizeUnsecure(value, x1, x2);
	if (r < 0.0f)
		return 0.0f;
	else if (r > 1.0f)
		return 1.0f;
	return r;
}
double Xe::Math::Normalize(double value, double x1, double x2)
{
	double r;
	if (x1 > x2)
		r = NormalizeUnsecure(value, x2, x1);
	else
		r = NormalizeUnsecure(value, x1, x2);
	if (r < 0.0f)
		return 0.0f;
	else if (r > 1.0f)
		return 1.0f;
	return r;
}
float Xe::Math::NormalizeUnsecure(float value, float x1, float x2)
{
	float range = x2 - x1;
	if (range < 0.0f)
		range = -range;
	return (value - x1) / range;
}
double Xe::Math::NormalizeUnsecure(double value, double x1, double x2)
{
	double range = x2 - x1;
	if (range < 0.0f)
		range = -range;
	return (value - x1) / range;
}
float Xe::Math::InterpolateNearestNeighbor(float m, float x1, float x2)
{
	return m < x2 ? x1 : x2;
}
double Xe::Math::InterpolateNearestNeighbor(double m, double x1, double x2)
{
	return m < x2 ? x1 : x2;
}
float Xe::Math::Lerp(float m, float x1, float x2)
{
	return (x1 * (1.0f - m) + x2 * m);
}
double Xe::Math::Lerp(double m, double x1, double x2)
{
	return (x1 * (1.0 - m) + x2 * m);
}
float Xe::Math::InterpolateSine(float m, float x1, float x2)
{
	float t = (1.0f - (float)Math::Cos(m * Math::PI)) / 2.0f;
	return (x1 * (1.0f - t) + x2 * t);
}
double Xe::Math::InterpolateSine(double m, double x1, double x2)
{
	double t = (1.0 - Math::Cos(m * Math::PI)) / 2.0;
	return (x1 * (1.0f - t) + x2 * t);
}