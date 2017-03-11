#include "pch.h"
#include <XeSDK/XeValue.h>
#include <XeSDK/XeMath.h>

namespace Xe {
	const Int8 Int8::DefaultValue = 0;
	const Int8 Int8::MinValue = (signed char)0x80;
	const Int8 Int8::MaxValue = (signed char)0x7F;
	Int8::Int8() : Int8(DefaultValue) {}
	Int8::Int8(signed char value) : ValueInteger(value) {}
	Int8 Int8::Abs() const { return m_value < 0 ? -m_value : m_value; }

	const UInt8 UInt8::DefaultValue = 0U;
	const UInt8 UInt8::MinValue = (unsigned char)0x00U;
	const UInt8 UInt8::MaxValue = (unsigned char)0xFFU;
	UInt8::UInt8() : UInt8(DefaultValue) {}
	UInt8::UInt8(unsigned char value) : ValueInteger(value) {}
	UInt8 UInt8::Abs() const { return m_value; }

	const Int16 Int16::DefaultValue = 0;
	const Int16 Int16::MinValue = (signed short)0x8000;
	const Int16 Int16::MaxValue = (signed short)0x7FFF;
	Int16::Int16() : Int16(DefaultValue) {}
	Int16::Int16(signed short value) : ValueInteger(value) {}
	Int16 Int16::Abs() const { return m_value < 0 ? -m_value : m_value; }
	Int16 Int16::SwapEndian() const { return (signed short)Math::Swap16(m_value); }

	const UInt16 UInt16::DefaultValue = 0U;
	const UInt16 UInt16::MinValue = (unsigned short)0x0000U;
	const UInt16 UInt16::MaxValue = (unsigned short)0xFFFFU;
	UInt16::UInt16() : UInt16(DefaultValue) {}
	UInt16::UInt16(unsigned short value) : ValueInteger(value) {}
	UInt16 UInt16::Abs() const { return m_value; }
	UInt16 UInt16::SwapEndian() const { return Math::Swap16(m_value); }

	const Int32 Int32::DefaultValue = 0;
	const Int32 Int32::MinValue = (signed int)0x80000000;
	const Int32 Int32::MaxValue = (signed int)0x7FFFFFFF;
	Int32::Int32() : Int32(DefaultValue) {}
	Int32::Int32(signed int value) : ValueInteger(value) {}
	Int32 Int32::Abs() const { return m_value < 0 ? -m_value : m_value; }
	Int32 Int32::SwapEndian() const { return (signed int)Math::Swap32(m_value); }

	const UInt32 UInt32::DefaultValue = 0U;
	const UInt32 UInt32::MinValue = (unsigned int)0x00000000U;
	const UInt32 UInt32::MaxValue = (unsigned int)0xFFFFFFFFU;
	UInt32::UInt32() : UInt32(DefaultValue) {}
	UInt32::UInt32(unsigned int value) : ValueInteger(value) {}
	UInt32 UInt32::Abs() const { return m_value; }
	UInt32 UInt32::SwapEndian() const { return Math::Swap32(m_value); }

	const Int64 Int64::DefaultValue = 0;
	const Int64 Int64::MinValue = (signed long long)0x8000000000000000LL;
	const Int64 Int64::MaxValue = (signed long long)0x7FFFFFFFFFFFFFFFLL;
	Int64::Int64() : Int64(DefaultValue) {}
	Int64::Int64(signed long long value) : ValueInteger(value) {}
	Int64 Int64::Abs() const { return m_value < 0 ? -m_value : m_value; }
	Int64 Int64::SwapEndian() const { return (signed long long)Math::Swap64(m_value); }

	const UInt64 UInt64::DefaultValue = 0;
	const UInt64 UInt64::MinValue = (unsigned long long)0x0000000000000000LL;
	const UInt64 UInt64::MaxValue = (unsigned long long)0xFFFFFFFFFFFFFFFFLL;
	UInt64::UInt64() : UInt64(DefaultValue) {}
	UInt64::UInt64(unsigned long long value) : ValueInteger(value) {}
	UInt64 UInt64::Abs() const { return m_value; }
	UInt64 UInt64::SwapEndian() const { return Math::Swap64(m_value); }

	const Float Float::DefaultValue = 0.0f;
	const Float Float::MinValue = *(float*)&UInt32(0xFF7FFFFFU);
	const Float Float::MaxValue = *(float*)&UInt32(0x7F7FFFFFU);
	const Float Float::PositiveInfinity = *(float*)&UInt32(0x7F800000U);
	const Float Float::NegativeInfinity = *(float*)&UInt32(0xFF800000U);
	const Float Float::NaN = *(float*)&UInt32(0x7FFFFFFFU);
	Float::Float() : Float(DefaultValue) {}
	Float::Float(float value) : Value(value) {}
	Float::Float(double value) : Value((float)value) {}
	Float Float::operator % (Float denominator) const {
		return Float(Math::Fmod(m_value, denominator));
	}
	Float& Float::operator %= (Float denominator) {
		m_value = Math::Fmod(m_value, denominator);
		return *this;
	}
	bool Float::IsInfinity() const {
		return (*(const unsigned int*)&m_value & 0x7FFFFFFF) == 0x7F800000;
	}
	bool Float::IsPositiveInfinity() const {
		return *(const unsigned int*)&m_value == 0x7F800000;
	}
	bool Float::IsNegativeInfinity() const {
		return *(const unsigned int*)&m_value == 0xFF800000;
	}
	bool Float::IsNaN() const {
		return (*(const unsigned int*)&m_value & 0x7FFFFFFF) > 0x7F800000;
	}
	bool Float::IsEqual(Float value, Float epsilon) const {
		return Math::Abs<float>(m_value - value) <= epsilon *
			Math::Max<float>(1.0f, Abs(), value.Abs());
	}
	Float Float::Abs() const { return m_value < 0 ? -m_value : m_value; }
	Float Float::Floor() const { return Math::Floor(m_value); }
	Float Float::Ceiling() const { return Math::Ceiling(m_value); }
	Float Float::Round() const { return Math::Ceiling(m_value); }

	const Double Double::DefaultValue = 0.0;
	const Double Double::MinValue = *(Double*)&UInt64(0xFFEFFFFFFFFFFFFFULL);
	const Double Double::MaxValue = *(Double*)&UInt64(0x7FEFFFFFFFFFFFFFULL);
	const Double Double::PositiveInfinity = *(Double*)&UInt64(0x7FF0000000000000ULL);
	const Double Double::NegativeInfinity = *(Double*)&UInt64(0xFFF0000000000000ULL);
	const Double Double::NaN = *(Double*)&UInt64(0x7FFFFFFFFFFFFFFFULL);
	Double::Double() : Double(DefaultValue) {}
	Double::Double(float value) : Value((double)value) {}
	Double::Double(double value) : Value(value) {}
	Double Double::operator % (Double denominator) const {
		return Double(Math::Fmod(m_value, denominator));
	}
	Double& Double::operator %= (Double denominator) {
		m_value = Math::Fmod(m_value, denominator);
		return *this;
	}
	bool Double::IsInfinity() const {
		return (*(const unsigned long long*)&m_value & 0x7FFFFFFFFFFFFFFFULL) == 0x7FF0000000000000ULL;
	}
	bool Double::IsPositiveInfinity() const {
		return *(const unsigned long long*)&m_value == 0x7FF0000000000000ULL;
	}
	bool Double::IsNegativeInfinity() const {
		return *(const unsigned long long*)&m_value == 0xFFF0000000000000ULL;
	}
	bool Double::IsNaN() const {
		return (*(const unsigned long long*)&m_value & 0x7FFFFFFFFFFFFFFFULL) > 0x7FF0000000000000ULL;
	}
	bool Double::IsEqual(Double value, Double epsilon) const {
		return Math::Abs<double>(m_value - value) <= epsilon *
			Math::Max<Double>(1.0, Abs(), value.Abs());
	}
	Double Double::Abs() const { return m_value < 0 ? -m_value : m_value; }
	Double Double::Floor() const { return Math::Floor(m_value); }
	Double Double::Ceiling() const { return Math::Ceiling(m_value); }
	Double Double::Round() const { return Math::Ceiling(m_value); }
}