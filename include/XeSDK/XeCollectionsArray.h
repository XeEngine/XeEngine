#pragma once
#include <XeSDK/ICollection.h>

namespace Xe {
	namespace Collections {
		template <class T>
		class Array<T> : public ICollection<T> {
			svar m_count;
			T* m_items;
		public:
			//! \brief interface id
			static const UID ID = 0xc7e96d1d1bbf41d3ULL;

			bool Query(IObject** obj, UID id) {
				switch (id) {
				case Array::ID:
				case ICollection::ID:
				case IObject::ID:
					AddRef();
					*obj = this;
					return true;
				}
				*obj = nullptr;
				return false;
			}

			Array(svar length) :
				m_count(length),
				m_items(new T[length]) { }
			~Array() {
				delete[] m_items;
			}

			T& operator[](int index) {
				return m_items[index];
			}

			// Implementation of ICollection
			bool Empty() const {
				return m_count == 0;
			}
			svar Count() const {
				return m_count;
			}
			bool Contains(const T& value) const {
				for (svar i = 0; i < m_count; i++)
					if (m_items[i] == value)
						return true;
				return false;
			}
			bool CopyTo(T* array, svar index, svar count, svar start) const {
				for (svar i = 0; i < count; i++) {
					array[index + i] = m_items[start + i];
				}
			}
		};
	}
}