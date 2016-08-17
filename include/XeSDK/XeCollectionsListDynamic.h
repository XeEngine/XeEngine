//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::ListDynamic header
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
		class ListDynamic : public IList<T>
		{
		protected:
			struct Node
			{
				Node* next;
				Node* prev;
				T value;
			};

			Node* m_mainNode;
			svar m_curLength;
		public:
			typedef typename IList<T>::Position Position;

			//! \brief interface id
			static const UID ID = 0x9935ccc67c5d4013L;

			bool Query(IObject **obj, UID id)
			{
				switch (id)
				{
				case ListDynamic::ID:
				case IList<T>::ID:
				case IEnumerable<T, Position>::ID:
				case ICollection<T>::ID:
				case IObject::ID:
					*obj = this;
					return true;
				}
				return false;
			}

			ListDynamic()
			{
				m_mainNode = new Node;
				m_mainNode->next = m_mainNode->prev = m_mainNode;

				m_curLength = 0;
			}
			~ListDynamic()
			{
				Clear();
				delete m_mainNode;
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
				return IndexOf(value) != (Position)-1;
			}
			bool CopyTo(T* array, svar index, svar count, svar start) const
			{
				if (Count() - start < count)
					return false;

				Node* src = m_mainNode->next;
				T* dst = array + index;
				for(svar i = 0; i < count; i++)
				{
					*dst++ = src->value;
					src = src->next;
				}
				return true;
			}

			//////////////////////////////////////////////////////////////////////////
			// IEnumerable

			bool IsFirst(const Position position) const
			{
				return position == Begin();
			}
			bool IsEnd(const Position position) const
			{
				return position == End();
			}
			Position Next(const Position position) const
			{
				return ((Node*)position)->next;
			}
			Position Begin() const
			{
				return m_mainNode->next;
			}
			Position End() const
			{
				return m_mainNode;
			}

			//////////////////////////////////////////////////////////////////////////
			// IEnumerableReverse

			Position Prev(const Position position) const
			{
				return ((Node*)position)->prev;
			}

			//////////////////////////////////////////////////////////////////////////
			// IList

			bool Add(const T& value)
			{
				Node* tmp = m_mainNode->prev;
				tmp->next = m_mainNode->prev = new Node;
				tmp->next->prev = tmp;
				tmp = tmp->next;
				tmp->next = m_mainNode;
				tmp->value = value;
				m_curLength++;
				return true;
			}
			void Clear()
			{
				Node* tmp;
				while (m_mainNode->next != m_mainNode)
				{
					tmp = m_mainNode->next;
					m_mainNode->next = tmp->next;
					delete tmp;
				}
			}
			Position IndexOf(const T& value) const
			{
				Node* tmp = m_mainNode->next;
				for (svar i = 0; i < Count(); i++)
				{
					if (tmp->value == value)
						return (Position)tmp;
					tmp = tmp->next;
				}
				return (Position)-1;
			}
			bool Insert(const Position position, const T& value)
			{
				if (position == nullptr)
					return false;

				Node* curnode = (Node*)position;
				Node* newnode = new Node;

				newnode->next = curnode;
				newnode->prev = curnode->prev;
				curnode->prev->next = newnode;
				curnode->prev = newnode;
				newnode->value = value;
				m_curLength++;
				return true;
			}
			bool Remove(const T& value)
			{
				const Position position = IndexOf(value);
				if (position == (const Position)-1)
					return false;
				RemoveAt(position);
				return true;
			}
			void RemoveAt(const Position position)
			{
				if (position != nullptr)
				{
					Node* curnode = (Node*)position;
					curnode->prev->next = curnode->next;
					curnode->next->prev = curnode->prev;
					delete curnode;
					m_curLength--;
				}
			}
			T Read(const Position position) const
			{
				return ((const Node*)position)->value;
			}
			void Write(const Position position, const T& value)
			{
				((Node*)position)->value = value;
			}

			T& operator [] (const Position position)
			{
				return ((Node*)position)->value;
			}
		};
	}
}
