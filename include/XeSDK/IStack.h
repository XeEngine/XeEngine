//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IStack header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/ICollection.h>

namespace Xe { namespace Collections {
	//! \brief stack of items
	template <class T>
	interface IStack : public ICollection<T>
	{
		//! \brief interface id
		static const UID ID = 0x6eaa6fc5c7384208ULL;

		//! \brief remove all items
		virtual void Clear() = 0;

		//! \brief add an element to the top of the stack
		//! \param[in] value to insert
		//! \return true if the item was inserted with success
		bool Push(T value);

		//! \brief remove the element from the top of the stack
		//! \return true if the item was removed with success
		bool Pop();

		//! \brief get the element from the top of the stack
		//! \param[out] value read
		//! \return true if the value was got with success
		bool Peek(T& value);
	};
} }