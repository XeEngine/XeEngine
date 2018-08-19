//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IQueue header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/ICollection.h>

namespace Xe { namespace Collections {
	//! \brief stack of items
	template <class T>
	interface IQueue : public ICollection<T>
	{
		//! \brief interface id
		static const UID ID = 0x6eaa6fc5c7384208L;

		//! \brief remove all items
		virtual void Clear() = 0;

		//! \brief add an element to the end of the queue
		//! \param[in] value to insert
		//! \return true if the item was inserted with success
		bool Enqueue(T value);

		//! \brief remove the element from the beginning of the queue
		//! \return true if the item was removed with success
		bool Dequeue();

		//! \brief get the element from the beginning of the queue
		//! \param[out] value read
		//! \return true if the value was got with success
		bool Peek(T& value);
	};
} }