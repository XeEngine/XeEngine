#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	//! \brief Smart pointer that manages the references of an IObject
	template <typename T>
	class ObjPtr {
		T* m_obj;

	public:
		//! \brief Does not hold any reference
		ObjPtr() : m_obj(nullptr) {}
		
		//! \brief Assign an IObject without to add a reference
		ObjPtr(T* obj) : m_obj(obj) {}

		//! \brief Assign an ObjPtr adding a reference
		ObjPtr(const ObjPtr& objptr) :
			m_obj(objptr.m_obj)
		{
			if (m_obj) m_obj->AddRef();
		}

		//! \brief Assign an ObjPtr adding a reference
		ObjPtr(ObjPtr&& objptr) noexcept :
			m_obj(objptr.m_obj)
		{
			m_obj = std::exchange(objptr.m_obj, nullptr);
		}

		//! \brief Remove a reference from the holding IObject, if exists
		~ObjPtr()
		{
			if (m_obj) m_obj->Release();
		}

		//! \brief Get the raw pointer; does not add any reference
		T* Get() const { return m_obj; }

		//! \brief Cast to the raw pointer; does not add any reference
		operator T*() { return m_obj; }

		//! \brief Get a pointer to the raw pointer
		T** operator&() { return &m_obj; }

		//! \brief Resolve the raw pointer
		T& operator*() const { return *m_obj; }

		//! \brief Use the raw pointer
		T* operator->() const { return m_obj; }

		//! \brief Use the helper operator () for IDelegate
		template <typename TArgs>
		void operator()(TArgs arg) { m_obj(arg); }

		//! \brief Check if there is an IObject alive
		operator bool() const { return !!m_obj; }

		//! \brief Check if there is not an IObject alive
		bool operator!() const { return !m_obj; }

		ObjPtr<T>& operator=(const ObjPtr<T>& objptr)
		{
			T* obj = objptr.m_obj;
			if (obj) obj->AddRef();
			if (m_obj) m_obj->Release();
			m_obj = obj;
			return *this;
		}

		ObjPtr<T>& operator=(ObjPtr<T>&& objptr) noexcept
		{
			m_obj = std::exchange(objptr.m_obj, nullptr);
			return *this;
		}

		ObjPtr<T>& operator=(T* obj)
		{
			if (obj) obj->AddRef();
			if (m_obj) m_obj->Release();
			m_obj = obj;
			return *this;
		}

		bool operator==(const T* o)
		{
			return m_obj == o;
		}

		bool operator==(const ObjPtr<T>& o)
		{
			return m_obj == o.m_obj;
		}

		template <typename U>
		bool operator==(const ObjPtr<U>& o)
		{
			return m_obj == o.m_obj;
		}

		bool operator!=(const T* o)
		{
			return m_obj != o;
		}

		bool operator!=(const ObjPtr<T>& o)
		{
			return m_obj != o.m_obj;
		}

		template <typename U>
		bool operator!=(const ObjPtr<U>& o)
		{
			return m_obj != o.m_obj;
		}
	};
}