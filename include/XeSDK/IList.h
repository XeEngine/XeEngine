//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IList header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IEnumerable.h>

namespace Xe
{
	namespace Collections
	{
		//! \brief a list of items
		template <class T>
		class IList : public IEnumerable<T, void*>
		{
		public:
			//! \brief used as position
			typedef typename IEnumerable<T, void*>::Position Position;

			//! \brief interface id
			static const UID ID = 0x96c86c98cc214162L;

			//! \brief remove all items
			virtual void Clear() = 0;

			//! \brief add an element
			//! \param[in] value to add
			//! \return false if the item was not inserted due to an internal error
			virtual bool Add(const T& value) = 0;

			//! \brief get the index of the specified item
			//! \param[in] value to check
			//! \return index value; -1 if the value was not found
			virtual Position IndexOf(const T& value) const = 0;

			//! \brief insert an item to the specified index
			//! \param[in] index of the value
			//! \param[in] value to insert
			//! \return false if the item was not inserted due to an internal error
			virtual bool Insert(const Position index, const T& value) = 0;

			//! \brief remove the first occurrence of the specified item
			//! \param[in] value to remove
			//! \return false if the item to remove was not found
			virtual bool Remove(const T& value) = 0;

			//! \brief remove the item at the specified index
			//! \param[in] index where the item to remove is located
			//! \return false if the item was not removed due to invalid index
			virtual void RemoveAt(const Position index) = 0;

			//! \brief read a value from the specified position
			//! \param[in] position where to read the item
			//! \return the read element
			virtual T Read(const Position position) const = 0;

			//! \brief write a value to the specified position
			//! \param[in] position where to write the item
			//! \param[in] value the value to write
			virtual void Write(const Position position, const T& value) = 0;

			virtual T& operator [] (const Position position) = 0;
		};
	}
}
