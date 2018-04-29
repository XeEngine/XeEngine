//////////////////////////////////////////////////////////////////////////
/* XeEngine
* Xe::IBase header
* Developed by Xeeynamo (Luciano Ciccariello)
* This file is part of XeEngine. Please do not share.
*////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/XeDef.h>

namespace Xe
{
	typedef u64 UID;

	//! \brief base interface
	/** \details This is not a real interface,
	* in fact it implements the basis of
	* AddRef and Release.
	*/
	class IObject
	{
		uvar i_count;
	public:
		//! \brief interface id
		static const UID ID = 0x0000000000000001ULL;

		//! \brief reset the reference count
		IObject();

		virtual ~IObject() {}

		//! \brief add a reference
		//! \return current reference count
		//! \details for every added reference, i_count increments by 1
		uvar AddRef();

		//! \brief remove a reference
		//! \return current reference count
		/** \details for every release, i_count decrements by 1; when
		* the reference count is 0, the resource is deallocated from
		* memory
		*/
		//! \warning don't use this if you declared the object into the stack
		uvar Release();

		//! \brief query the interface
		//! \param id interface identifier to check
		//! \return the class pointer if the query is satisfied, else nullptr
		virtual bool Query(IObject **obj, UID id);
	};
}
