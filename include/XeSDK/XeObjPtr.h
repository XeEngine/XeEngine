#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	//! \brief Manage automatically references of an IObject
	template <typename T>
	class ObjPtr {
		T* m_obj;
	public:
		ObjPtr() : m_obj(nullptr) {}
		ObjPtr(T* obj) : m_obj(obj) {
		}
		ObjPtr(const ObjPtr& objptr) : m_obj(objptr.m_obj) {
			if (m_obj) m_obj->AddRef();
		}
		~ObjPtr() {
			m_obj->Release();
		}

		//! \brief Get the associated object
		T* Get() const { return m_obj; }

		//! \brief Get the ID of current ObjPtr
		UID Id() const { T::ID; }

		//! \brief Get the object as U
		//! \param[out] objptr that will hold the specified IObject
		//! \return true if the operation was successed, else false.
		/** \details When As is called, on the associated IObject will be
		 * called a query with the same ID of U. If the query fails then
		 * false is returned and objptr will contain a null reference.
		 */
		template <typename U>
		bool As(ObjPtr<U>& objptr) {
			if (m_obj != nullptr)
				return m_obj->Query((IObject**)&objptr.m_obj, U::ID);
			return false;
		}

		T& operator*() const { return *m_obj; }
		T* operator->() const { return m_obj; }
		bool operator!() const { return !m_obj; }
		ObjPtr<T>& operator=(const ObjPtr<T>& objptr) {
			T* obj = objptr.m_obj;
			if (obj) obj->AddRef();
			if (m_obj) m_obj->Release();
			m_obj = obj;
			return *this;
		}
		ObjPtr<T>& operator=(T* obj) {
			if (obj) obj->AddRef();
			if (m_obj) m_obj->Release();
			m_obj = obj;
			return *this;
		}
		bool operator==(const T* o) {
			return m_obj == o;
		}
		bool operator==(const ObjPtr<T>& o) {
			return m_obj == o.m_obj;
		}
		template <typename U>
		bool operator==(const ObjPtr<U>& o) {
			return m_obj == o.m_obj;
		}
		bool operator!=(const T* o) {
			return m_obj != o;
		}
		bool operator!=(const ObjPtr<T>& o) {
			return m_obj != o.m_obj;
		}
		template <typename U>
		bool operator!=(const ObjPtr<U>& o) {
			return m_obj != o.m_obj;
		}
	};
}