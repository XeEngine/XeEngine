//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::ListFixed header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IList.h>

namespace Xe
{
	namespace Collections
	{
		//! \brief a list of items
		template <class T>
		class ListFixed : public IList<T>
		{
		protected:
			T* m_content;
			T* m_contentCur;
			svar m_curLength;
			svar m_maxLength;
		public:
			typedef typename IList<T>::Position Position;

			ListFixed(svar length)
			{
				if (length > 0)
				{
					m_content = new T[length];
					m_contentCur = m_content;
				}

				m_curLength = 0;
				m_maxLength = length;
			}
			~ListFixed()
			{
				delete[] m_content;
			}

			//////////////////////////////////////////////////////////////////////////
			// ICollection

			bool Empty() const
			{
				return Count() == 0;
			}
			svar Count() const
			{
				return m_curLength;
			}
			bool Contains(const T& value) const
			{
				return IndexOf(value) != nullptr;
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
			// IEnumerable

			bool IsFirst(const Position position) const
			{
				return position == m_content;
			}
			bool IsEnd(const Position position) const
			{
				if (Count() == 0)
					return true;
				return position == m_contentCur;
			}
			Position Next(const Position position) const
			{
				if (IsEnd(position) == false)
					return (T*)position + 1;
				return position;
			}
			Position Begin() const
			{
				return m_content;
			}
			Position End() const
			{
				return m_contentCur;
			}

			//////////////////////////////////////////////////////////////////////////
			// IEnumerableReverse

			Position Prev(const Position position) const
			{
				if (IsFirst(position) == false)
					return (T*)position - 1;
				return position;
			}

			//////////////////////////////////////////////////////////////////////////
			// IList

			bool Add(const T& value)
			{
				if (Count() >= m_maxLength)
					return false;
				*m_contentCur++ = value;
				m_curLength++;
				return true;
			}
			void Clear()
			{
				m_contentCur = m_content;
				m_curLength = 0;
			}
			Position IndexOf(const T& value) const
			{
				for(T* index = m_content; index < m_contentCur; index++)
				{
					if (*index == value)
						return index;
				}
				return nullptr;
			}
			bool Insert(const Position position, const T& value)
			{
				if (position < m_content || position > m_contentCur ||
					Count() >= m_maxLength)
					return false;

				for (T* index = m_contentCur; index > position; index--)
					*index = index[-1];
				*(T*)position = value;
				m_contentCur++;
				m_curLength++;
				return true;
			}
			bool Remove(const T& value)
			{
				Position position = IndexOf(value);
				if (position == nullptr)
					return false;
				RemoveAt(position);
				return true;
			}
			void RemoveAt(const Position position)
			{
				if (position >= m_content && position < m_contentCur)
				{
					if (position != m_contentCur)
					{
						for (T* index = (T*)position; index < m_contentCur; index++)
							*index = index[1];
					}
					m_contentCur--;
					m_curLength--;
				}
			}
			T Read(const Position position) const
			{
				return *(T*)position;
			}
			void Write(const Position position, const T& value)
			{
				*(T*)position = value;
			}

			T& operator [] (const Position position)
			{
				return *(T*)position;
			}
		};
	}
}
