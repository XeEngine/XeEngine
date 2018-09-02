//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::Stack header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IStack.h>

namespace Xe
{
	namespace Collections
	{
		template <class T>
		class Stack : public IStack<T>
		{
			T* m_content;
			svar m_count;
			svar m_capacity;

			void _SetCapacity(svar capacity)
			{
				m_content = new T[capacity];
				m_count = 0;
				m_capacity = capacity;
			}
		public:
			Stack(const ICollection<T>* collecton)
			{
				_SetCapacity(collecton->Count());
				collecton->CopyTo(m_content, 0, m_capacity, 0);
			}
			Stack(svar capacity)
			{
				_SetCapacity(capacity);
			}

			//////////////////////////////////////////////////////////////////////////
			// ICollection

			bool Empty() const
			{
				return Count() == 0;
			}
			svar Count() const
			{
				return m_count;
			}
			bool Contains(const T value) const
			{
				for (svar i = 0; i < Count(); i++)
				{
					if (m_content[i] == value)
						return true;
				}
			}
			bool CopyTo(T* array, svar index, svar count, svar start) const
			{
				if (Count() - start < count)
					return false;

				T* src = m_content + start;
				T* dst = array + index;
				for(svar i = 0; i < count; i++)
					*dst++ = *src++;
				return true;
			}

			//////////////////////////////////////////////////////////////////////////
			// IStack

			void Clear()
			{
				m_capacity = 0;
			}
			bool Push(T value)
			{
				if (m_count >= m_capacity)
					return false;
				m_content[m_count++] = value;
				return true;
			}
			bool Pop()
			{
				if (m_count <= 0)
					return false;
				m_count--;
				return true;
			}
			bool Peek(T& value)
			{
				if (m_count <= 0)
					return false;
				value = m_content[m_count - 1];
				return true;
			}
		};
	}
}
