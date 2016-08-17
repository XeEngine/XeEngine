#pragma once
#include <XeSDK/XeDef.h>

namespace Xe {
	namespace Math {
		//! \brief randommic numbers generator
		/** \details this random system uses a fixed way to generate
		* causal numbers; when you initialize it with the specified
		* seed (if you don't call it, a default seed is used), every
		* time that you call Next, the same sequence of numbers
		* will be generated every time; this is to get the ability
		* to replay at the same way some events; if you want to
		* get true random values, set a different seed every time
		* that you run the application, for example the current
		* timer in milliseconds (with this technique, it's almost
		* impossible to get every time the same sequence of numbers);
		* this implementation uses 64-bit random values to extreme
		* precision on random values;
		* current implementation is based on a variant of Mersenne Twister
		* MT 19937
		*/
		class Random
		{
			u64 m_seed;
		public:
			static Random Global;

			Random();
			Random(u64 seed);

			//! \brief get the seed currently used for random value
			//! \return current seed value from SetSeed and Next * n
			//! \details use this to get the current state of Next
			//! \sa SetSeed, Next
			u64 GetSeed();

			//! \brief set the seed for random value
			//! \param[in] seed value to set
			//! \details from this depends the sequence number of Next
			//! \sa GetSeed, Next
			void SetSeed(u64 seed);

			//! \brief get the next random value
			//! \return a 64-bit random value between MinU64 and MaxU64
			//! \sa Nextf, Nextd
			u64 Next();

			//! \brief Get a random value between specified range
			u64 NextRange(u64 min, u64 max);

			//! \brief get the next random value
			//! \return a 32-bit random value between 0.0 and 1.0
			//! \sa Next, Nextd
			float Nextf();

			//! \brief get the next random value
			//! \return a 64-bit random value between 0.0 and 1.0
			//! \sa Next, Nextf
			double Nextd();
		};
	}
}