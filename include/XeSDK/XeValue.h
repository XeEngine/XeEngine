#pragma once

namespace Xe {
	template <class T>
	struct Value {
		Value<T>() : m_value(0) {}
		Value<T>(const T& value) : m_value(value) { }
		operator T() const { return m_value; }
		Value<T> operator +() const { return Value<T>(+m_value); }
		Value<T> operator -() const { return Value<T>(-m_value); }
		Value<T>& operator ++() { m_value++; return *this; }
		Value<T>& operator --() { m_value--; return *this; }
		Value<T> operator ++(int) { return Value<T>(m_value++); }
		Value<T> operator --(int) { return Value<T>(m_value--); }
		Value<T>& operator = (const T& value) { m_value = value; return *this; }
		Value<T>& operator += (const T& value) { m_value += value; return *this; }
		Value<T>& operator -= (const T& value) { m_value -= value; return *this; }
		Value<T>& operator *= (const T& value) { m_value *= value; return *this; }
		Value<T>& operator /= (const T& value) { m_value /= value; return *this; }
		bool operator == (const T& value) const { return m_value == value; }
		bool operator != (const T& value) const { return m_value != value; }
		bool operator >= (const T& value) const { return m_value >= value; }
		bool operator < (const T& value) const { return m_value < value; }
	protected:
		T m_value;
	};
	template <class T>
	struct ValueInteger : Value<T> {
		ValueInteger<T>() : Value(0) {}
		ValueInteger<T>(const T& value) : Value(value) {}
		//ValueInteger<T> operator % (const T& value) const { return ValueInteger<T>(m_value % value); }
		//ValueInteger<T> operator & (const T& value) const { return ValueInteger<T>(m_value & value); }
		//ValueInteger<T> operator | (const T& value) const { return ValueInteger<T>(m_value | value); }
		//ValueInteger<T> operator ^ (const T& value) const { return ValueInteger<T>(m_value ^ value); }
		//ValueInteger<T> operator >> (const T& value) const { return ValueInteger<T>(m_value >> value); }
		//ValueInteger<T> operator << (const T& value) const { return ValueInteger<T>(m_value << value); }
		ValueInteger<T> operator ~() const { return ValueInteger<T>(~m_value); }
		ValueInteger<T> operator !() const { return ValueInteger<T>(!m_value); }
		ValueInteger<T> operator &&(ValueInteger<T> value) const { return ValueInteger<T>(m_value && value); }
		ValueInteger<T> operator ||(ValueInteger<T> value) const { return ValueInteger<T>(m_value || value); }
		ValueInteger<T>& operator %= (const T& value) { m_value %= value; return *this; }
		ValueInteger<T>& operator &= (const T& value) { m_value &= value; return *this; }
		ValueInteger<T>& operator |= (const T& value) { m_value |= value; return *this; }
		ValueInteger<T>& operator ^= (const T& value) { m_value ^= value; return *this; }
		ValueInteger<T>& operator >>= (const T& value) { m_value >>= value; return *this; }
		ValueInteger<T>& operator <<= (const T& value) { m_value <<= value; return *this; }
		bool TestBit(T bit) const { return !!(m_value & (1 << bit)); }
		void ResetBit(T bit) { m_value &= ~(1 << bit); }
		void SetBit(T bit) { m_value |= 1 << bit; }
		bool EqualBits(T bits) const { return (m_value & bits) == bits; }
	};

	struct Int8 : public ValueInteger<signed char> {
		static const Int8 DefaultValue;
		static const Int8 MinValue;
		static const Int8 MaxValue;
		Int8();
		Int8(signed char value);
		Int8 Abs() const;
		Int8 Sign() const;
	};
	struct UInt8 : public ValueInteger<unsigned char> {
		static const UInt8 DefaultValue;
		static const UInt8 MinValue;
		static const UInt8 MaxValue;
		UInt8();
		UInt8(unsigned char value);
		UInt8 Abs() const;
		UInt8 Sign() const;
	};
	struct Int16 : public ValueInteger<signed short> {
		static const Int16 DefaultValue;
		static const Int16 MinValue;
		static const Int16 MaxValue;
		Int16();
		Int16(signed short value);
		Int16 Abs() const;
		Int16 Sign() const;
		Int16 SwapEndian() const;
	};
	struct UInt16 : public ValueInteger<unsigned short> {
		static const UInt16 DefaultValue;
		static const UInt16 MinValue;
		static const UInt16 MaxValue;
		UInt16();
		UInt16(unsigned short value);
		UInt16 Abs() const;
		UInt16 Sign() const;
		UInt16 SwapEndian() const;
	};
	struct Int32 : public ValueInteger<signed int> {
		static const Int32 DefaultValue;
		static const Int32 MinValue;
		static const Int32 MaxValue;
		Int32();
		Int32(signed int value);
		Int32 Abs() const;
		Int32 Sign() const;
		Int32 SwapEndian() const;
	};
	struct UInt32 : public ValueInteger<unsigned int> {
		static const UInt32 DefaultValue;
		static const UInt32 MinValue;
		static const UInt32 MaxValue;
		UInt32();
		UInt32(unsigned int value);
		UInt32 Abs() const;
		UInt32 Sign() const;
		UInt32 SwapEndian() const;
	};
	struct Int64 : public ValueInteger<signed long long> {
		static const Int64 DefaultValue;
		static const Int64 MinValue;
		static const Int64 MaxValue;
		Int64();
		Int64(signed long long value);
		Int64 Abs() const;
		Int64 Sign() const;
		Int64 SwapEndian() const;
	};
	struct UInt64 : public ValueInteger<unsigned long long> {
		static const UInt64 DefaultValue;
		static const UInt64 MinValue;
		static const UInt64 MaxValue;
		UInt64();
		UInt64(unsigned long long value);
		UInt64 Abs() const;
		UInt64 Sign() const;
		UInt64 SwapEndian() const;
	};
	struct Float : public Value<float> {
		static const Float DefaultValue;
		static const Float MinValue;
		static const Float MaxValue;
		static const Float PositiveInfinity;
		static const Float NegativeInfinity;
		static const Float NaN;
		Float();
		Float(float value);
		Float(double value);
		Float operator % (Float denominator) const;
		Float& operator %= (Float denominator);
		bool IsInfinity() const;
		bool IsPositiveInfinity() const;
		bool IsNegativeInfinity() const;
		bool IsNaN() const;
		bool IsEqual(Float value, Float epsilon) const;
		Float Abs() const;
		Float Sign() const;
		Float Floor() const;
		Float Ceiling() const;
		Float Round() const;
	};
	struct Double : public Value<double> {
		static const Double DefaultValue;
		static const Double MinValue;
		static const Double MaxValue;
		static const Double PositiveInfinity;
		static const Double NegativeInfinity;
		static const Double NaN;
		Double();
		Double(float value);
		Double(double value);
		Double operator % (Double denominator) const;
		Double& operator %= (Double denominator);
		bool IsInfinity() const;
		bool IsPositiveInfinity() const;
		bool IsNegativeInfinity() const;
		bool IsNaN() const;
		bool IsEqual(Double value, Double epsilon) const;
		Double Abs() const;
		Double Sign() const;
		Double Floor() const;
		Double Ceiling() const;
		Double Round() const;
	};

	struct Bool {
		static const Bool False;
		static const Bool True;

		Bool() : m_value(0) {}
		Bool(bool value) : m_value(!!value) { }
		operator bool() const { return m_value; }
		Bool& operator = (bool value) { m_value = value; return *this; }
		bool operator == (bool value) const { return m_value == value; }
		bool operator != (bool value) const { return m_value != value; }
		bool operator > (bool value) const { return m_value > value; }
		bool operator >= (bool value) const { return m_value >= value; }
		bool operator < (bool value) const { return m_value < value; }
		bool operator <= (bool value) const { return m_value <= value; }
		Bool operator !() const { return Bool(!m_value); }
		Bool operator &&(bool value) const { return Bool(m_value && value); }
		Bool operator ||(bool value) const { return Bool(m_value || value); }
		Bool& operator &= (bool value) { m_value &= value; return *this; }
		Bool& operator |= (bool value) { m_value |= value; return *this; }
		Bool& operator ^= (bool value) { m_value ^= value; return *this; }
		Bool IsFalse() const { return !m_value; }
		Bool IsTrue() const { return m_value; }
	private:
		bool m_value;
	};
}