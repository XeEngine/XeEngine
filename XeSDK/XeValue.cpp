#include "pch.h"
#include <XeSDK/XeValue.h>
#include <XeSDK/XeMath.h>

namespace Xe {
	const Int8 Int8::DefaultValue = 0;
	const Int8 Int8::MinValue = 0x80;
	const Int8 Int8::MaxValue = 0x7F;
	Int8::Int8() : Int8(DefaultValue) {}
	Int8::Int8(signed char value) : ValueInteger(value) {}

	const UInt8 UInt8::DefaultValue = 0U;
	const UInt8 UInt8::MinValue = 0x00U;
	const UInt8 UInt8::MaxValue = 0xFFU;
	UInt8::UInt8() : UInt8(DefaultValue) {}
	UInt8::UInt8(unsigned char value) : ValueInteger(value) {}

	const Int16 Int16::DefaultValue = 0;
	const Int16 Int16::MinValue = 0x8000;
	const Int16 Int16::MaxValue = 0x7FFF;
	Int16::Int16() : Int16(DefaultValue) {}
	Int16::Int16(signed short value) : ValueInteger(value) {}
	Int16 Int16::SwapEndian() const {
		return Int16(
			((m_value & 0xFF) << 8) |
			((m_value >> 8) & 0xFF));
	}

	const UInt16 UInt16::DefaultValue = 0U;
	const UInt16 UInt16::MinValue = 0x0000U;
	const UInt16 UInt16::MaxValue = 0xFFFFU;
	UInt16::UInt16() : UInt16(DefaultValue) {}
	UInt16::UInt16(unsigned short value) : ValueInteger(value) {}
	UInt16 UInt16::SwapEndian() const {
		return UInt16(
			((m_value & 0xFF) << 8) |
			((m_value >> 8) & 0xFF));
	}

	const Int32 Int32::DefaultValue = 0;
	const Int32 Int32::MinValue = 0x80000000;
	const Int32 Int32::MaxValue = 0x7FFFFFFF;
	Int32::Int32() : Int32(DefaultValue) {}
	Int32::Int32(signed int value) : ValueInteger(value) {}
	Int32 Int32::SwapEndian() const {
		return Int32(
			(((m_value >> 0) & 0xFF) << 24) |
			(((m_value >> 8) & 0xFF) << 16) |
			(((m_value >> 16) & 0xFF) << 8) |
			(((m_value >> 24) & 0xFF) << 0));
	}

	const UInt32 UInt32::DefaultValue = 0U;
	const UInt32 UInt32::MinValue = 0x00000000U;
	const UInt32 UInt32::MaxValue = 0xFFFFFFFFU;
	UInt32::UInt32() : UInt32(DefaultValue) {}
	UInt32::UInt32(unsigned int value) : ValueInteger(value) {}
	UInt32 UInt32::SwapEndian() const {
		return UInt32(
			(((m_value >> 0) & 0xFF) << 24) |
			(((m_value >> 8) & 0xFF) << 16) |
			(((m_value >> 16) & 0xFF) << 8) |
			(((m_value >> 24) & 0xFF) << 0));
	}

	const Int64 Int64::DefaultValue = 0;
	const Int64 Int64::MinValue = 0x8000000000000000LL;
	const Int64 Int64::MaxValue = 0x7FFFFFFFFFFFFFFFLL;
	Int64::Int64() : Int64(DefaultValue) {}
	Int64::Int64(signed long long value) : ValueInteger(value) {}
	Int64 Int64::SwapEndian() const {
		return UInt32(
			(((m_value >> 0) & 0xFF) << 56) |
			(((m_value >> 8) & 0xFF) << 48) |
			(((m_value >> 16) & 0xFF) << 40) |
			(((m_value >> 24) & 0xFF) << 32) |
			(((m_value >> 32) & 0xFF) << 24) |
			(((m_value >> 40) & 0xFF) << 16) |
			(((m_value >> 48) & 0xFF) << 8) |
			(((m_value >> 56) & 0xFF) << 0));
	}

	const UInt64 UInt64::DefaultValue = 0;
	const UInt64 UInt64::MinValue = 0x0000000000000000LL;
	const UInt64 UInt64::MaxValue = 0xFFFFFFFFFFFFFFFFLL;
	UInt64::UInt64() : UInt64(DefaultValue) {}
	UInt64::UInt64(unsigned long long value) : ValueInteger(value) {}
	UInt64 UInt64::SwapEndian() const {
		return UInt64(
			(((m_value >> 0) & 0xFF) << 56) |
			(((m_value >> 8) & 0xFF) << 48) |
			(((m_value >> 16) & 0xFF) << 40) |
			(((m_value >> 24) & 0xFF) << 32) |
			(((m_value >> 32) & 0xFF) << 24) |
			(((m_value >> 40) & 0xFF) << 16) |
			(((m_value >> 48) & 0xFF) << 8) |
			(((m_value >> 56) & 0xFF) << 0));
	}

	const Float Float::DefaultValue = 0.0f;
	const Float Float::MinValue = *(float*)&UInt32(0xFF7FFFFFU);
	const Float Float::MaxValue = *(float*)&UInt32(0x7F7FFFFFU);
	const Float Float::PositiveInfinity = *(float*)&UInt32(0x7F800000U);
	const Float Float::NegativeInfinity = *(float*)&UInt32(0xFF800000U);
	const Float Float::NaN = *(float*)&UInt32(0x7FFFFFFFU);
	Float::Float() : Float(DefaultValue) {}
	Float::Float(float value) : Value(value) {}
	Float::Float(double value) : Value(value) {}
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
			Math::Max<float>(1.0f, Math::Abs(m_value), Math::Abs(value));
	}

	const Double Double::DefaultValue = 0.0;
	const Double Double::MinValue = *(Double*)&UInt64(0xFFEFFFFFFFFFFFFFULL);
	const Double Double::MaxValue = *(Double*)&UInt64(0x7FEFFFFFFFFFFFFFULL);
	const Double Double::PositiveInfinity = *(Double*)&UInt64(0x7FF0000000000000ULL);
	const Double Double::NegativeInfinity = *(Double*)&UInt64(0xFFF0000000000000ULL);
	const Double Double::NaN = *(Double*)&UInt64(0x7FFFFFFFFFFFFFFFULL);
	Double::Double() : Double(DefaultValue) {}
	Double::Double(float value) : Value(value) {}
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
		return Math::Abs<Double>(m_value - value) <= epsilon *
			Math::Max<Double>(1.0, Math::Abs(m_value), Math::Abs(value));
	}
}