#pragma once
#include <XeSDK/ICollection.h>
#include <stdexcept>

namespace Xe { namespace Collections {
	template <typename T, size_t length>
	class StaticArray
	{
		typedef int(&ArrayRef)[length];

		T (&m_Array)[length];
	public:
		StaticArray(T (&items)[length]) :
			m_Array(items)
		{}

		operator ArrayRef()
		{
			return m_Array;
		}

		T& operator[](int index)
		{
			if (index < 0)
				throw std::out_of_range(NAMEOF(index)" cannot be negative");
			if (index >= length)
				throw std::out_of_range(NAMEOF(index)" cannot be greater than the size of the array");
			return m_Array[index];
		}

		bool IsEmpty() const
		{
			return length == 0;
		}
		
		int GetLength() const
		{
			return (int)length;
		}

		int IndexOf(T item) const
		{
			for (int i = 0; i < length; ++i)
			{
				if (m_Array[i] == item)
					return i;
			}

			return -1;
		}

		int LastIndexOf(T item) const
		{
			for (int i = length - 1; i >= 0; --i)
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
	};
} }