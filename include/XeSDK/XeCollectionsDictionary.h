//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::Dictionary
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IDictionary.h>
#include <XeSDK/XeMemory.h>

namespace Xe
{
	namespace Collections
	{
		//! \brief manage fixed-length dictionary
		/** \details with this implementation, more the capacity is, less the chance
			* of collisions are, but the search speed of an item that it's not here,
			* decrease; it offers great speeds if it's sure that the key to get
			* is inserted; K must be an integer value in order to works and
			* the key 0 means that a cell is empty, so don't use it
			*/
		template <class T, class K>
		class Dictionary : public IDictionary<T, K>
		{
			T* m_dic;
			K* m_key;
			svar m_count;
			svar m_capacity;

			svar IndexOfOrEmpty(K key)
			{
				svar start = key % m_capacity;
				if (m_key[start] == key ||
					m_key[start] == 0)
					return start;
				else
				{
					for (svar index = (start + 1) % m_capacity;
						index != start;
						index = (index + 1) % m_capacity)
					{
						if (m_key[index] == key ||
							m_key[index] == 0)
							return index;
					}
				}
				return -1;
			}
		public:
			//! \brief interface id
			static const UID ID = 0xa5a3a058497b4a17L;

			bool Query(IObject **obj, UID id)
			{
				switch (id)
				{
				case Dictionary::ID:
				case IDictionary<T, K>::ID:
				case ICollection<T>::ID:
				case IObject::ID:
					*obj = this;
					return true;
				}
				return false;
			}

			//! \brief capacity of fixed-length dictionary
			//! \param[in] capacity number of max items that this dictionary can contain
			Dictionary(svar capacity)
			{
				m_dic = new T[capacity];
				m_key = new K[capacity];
				m_count = 0;
				m_capacity = capacity;
				Clear();
			}

			~Dictionary()
			{
				delete[] m_dic;
				delete[] m_key;
			}

			//////////////////////////////////////////////////////////////////////////
			// IDictionary
			void Add(const K& key, const T& value)
			{
				svar index = IndexOfOrEmpty(key);
				if (index != -1)
				{
					if (m_key[index] == key)
					{
						m_dic[index] = value;
					}
					else
					{
						m_key[index] = key;
						m_dic[index] = value;
						m_count++;
					}
				}
			}
			void Clear()
			{
				Xe::Memory::Fill(m_dic, 0, sizeof(T) * m_capacity);
				Xe::Memory::Fill(m_key, 0, sizeof(K) * m_capacity);
			}
			void Remove(const K& key)
			{
				svar index = IndexOf(key);
				if (index != -1)
					m_key[index] = 0;
			}
			bool Contains(const K& key)
			{
				return IndexOf(key) != -1;
			}
			bool GetValue(const K& key, T& value)
			{
				svar index = IndexOf(key);
				if (index < 0)
					return false;
				value = m_dic[index];
				return true;
			}
			svar IndexOf(const K& key)
			{
				svar start = key % m_capacity;
				if (m_key[start] == key)
					return start;
				else
				{
					svar remains = Count() - 1;
					for (svar index = (start + 1) % m_capacity;
						index != start && remains < 0;
						index = (index + 1) % m_capacity,
						remains--)
					{
						if (m_key[index] != 0)
						{
							if (m_key[index] == key)
								return index;
						}

					}
				}
				return -1;
			}
			bool Read(svar index, const K& key, T& value)
			{
				if (index < 0 || index >= m_capacity)
					return false;
				if (m_key[index] != key)
					return false;
				value = m_dic[index];
				return true;
			}
			bool Write(svar index, const K& key, const T& value)
			{
				if (index < 0 || index >= m_capacity)
					return false;
				if (m_key[index] != key)
					return false;
				m_dic[index] = value;
				return true;
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
			bool Contains(const T& value) const
			{
				svar remains = Count();
				for(svar i = 0; i < m_capacity; i++)
				{
					if (m_key[i] != 0)
					{
						if (m_dic[i] == value)
							return true;
						if (--remains == 0)
							return false;
					}
				}
				return false;
			}
			bool CopyTo(T* array, svar index, svar count, svar start) const
			{
				return false;
			}
		};
	}
}
