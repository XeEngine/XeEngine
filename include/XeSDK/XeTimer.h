#pragma once
#include <XeSDK/XeDef.h>

namespace Xe {
	//! \brief manage timing in microseconds
	//! \details precision depends to the operating system and CPU.
	class Timer {
	public:
		//! \brief Get a timer that it's equal to zero.
		static Timer Zero();
		//! \brief Get the current time since the engine has been initialized.
		static Timer Current();

		//! \brief convert current timer to an integer value
		s64 AsInteger() const;
		//! \brief convert current timer to a decimal value
		double AsDouble() const;

		Timer operator+(const Timer&) const;
		Timer operator-(const Timer&) const;
		const Timer& operator=(const Timer&);
		const Timer& operator+=(const Timer&);
		const Timer& operator-=(const Timer&);
		bool operator==(const Timer&);
		bool operator!=(const Timer&);
		bool operator>(const Timer&);
		bool operator>=(const Timer&);
		bool operator<(const Timer&);
		bool operator<=(const Timer&);

	private:
		s64 m_time;
	};
}