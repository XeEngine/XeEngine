#include "pch.h"
#include <XeSDK/XeMathRandom.h>

namespace Xe
{
	namespace Math
	{
		Random Random::Global;

		Random::Random() :
			m_seed(0xCF8531D17FC7DDDULL)
		{ }
		Random::Random(u64 seed) :
			m_seed(seed)
		{ }

		u64 Random::GetSeed() {
			return m_seed;
		}
		void Random::SetSeed(u64 seed) {
			m_seed = seed;
		}
		u64 Random::Next() {
			u64 x = m_seed;
			x ^= (x >> 29) & 0x5555555555555555ULL;
			x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
			x ^= (x << 37) & 0xFFF7EEE000000000ULL;
			x ^= (x >> 43);
			m_seed = 0x5851F42D4C957F2DULL * (m_seed ^ ((u64)m_seed >> 62)) + x;
			return x;
		}
		u64 Random::NextRange(u64 min, u64 max) {
			if (min > max) {
				u64 tmp = min;
				min = max;
				max = tmp;
			}
			return Next() % (max - min + 1) + min;
		}
		float Random::Nextf() {
			static const float MinimumFloatValue = 1.0f / (float)0xFFFFFFFFFFFFFFFFULL;
			return Next() * MinimumFloatValue;
		}
		double Random::Nextd() {
			static const double MinimumDoubleValue = 1.0 / (double)0xFFFFFFFFFFFFFFFFULL;
			return Next() * MinimumDoubleValue;
		}
	}
}