#pragma once
#include <XeSDK/ICollection.h>
#include <stdexcept>

namespace Xe { namespace Collections {
	template <typename T>
	class Array
	{
	protected:
		int m_Length;
		T* m_Array;
	public:
		template <size_t length>
		Array(T(&items)[length]) :
			Array(items, length)
		{}

		Array() :
			m_Length(0),
			m_Array(nullptr)
		{}

		Array(T* items, int length) :
			m_Length(length),
			m_Array(items)
		{}

		operator T*() const
		{
			return m_Array;
		}

		T& operator[](int index)
		{
			if (index < 0)
				throw std::out_of_range(NAMEOF(index)" cannot be negative");
			if (index >= m_Length)
				throw std::out_of_range(NAMEOF(index)" cannot be greater than the size of the array");
			return m_Array[index];
		}

		const T& operator[](int index) const
		{
			if (index < 0)
				throw std::out_of_range(NAMEOF(index)" cannot be negative");
			if (index >= m_Length)
				throw std::out_of_range(NAMEOF(index)" cannot be greater than the size of the array");
			return m_Array[index];
		}

		bool IsEmpty() const
		{
			return m_Length == 0;
		}
		
		int GetLength() const
		{
			return m_Length;
		}

		int IndexOf(T item) const
		{
			for (int i = 0; i < m_Length; ++i)
			{
				if (m_Array[i] == item)
					return i;
			}

			return -1;
		}

		int LastIndexOf(T item) const
		{
			for (int i = m_Length - 1; i >= 0; --i)
			{
				if (m_Array[i] == item)
					return i;
			}

			return -1;
		}

		bool Contains(T item) const
		{
			return IndexOf(item) >= 0;
		}

		Array SubArray(int startIndex) const
		{
			return SubArray(startIndex, m_Length - startIndex);
		}

		Array SubArray(int startIndex, int length) const
		{
			if (startIndex < 0)
				throw std::out_of_range(NAMEOF(startIndex)" cannot be negative");
			if (length >= m_Length)
				throw std::out_of_range(NAMEOF(length)" cannot benegative");
			if (startIndex + length > m_Length)
				throw std::out_of_range(NAMEOF(startIndex)" and " NAMEOF(length)" are out of range");

			return Array(m_Array + startIndex, length);
		}
	};
} }