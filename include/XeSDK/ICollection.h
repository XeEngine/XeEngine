//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::ICollection header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IObject.h>

namespace Xe
{
	namespace Collections
	{
		template <class T>
		class ICollection : public IObject
		{
		public:
			//! \brief interface id
			static const UID ID = 0x827bf93f1ba0481dL;

			//! \brief check if the content is empty or not
			//! \return true if it's empty
			virtual bool Empty() const = 0;

			//! \brief get the number of items inside the current item collection
			//! \return how items there are
			virtual svar Count() const = 0;

			//! \brief check if the structure contains the specified value
			//! \param[in] value to check
			//! \return true if the item was found
			virtual bool Contains(const T& value) const = 0;

			//! \brief copy the items to an array
			//! \param[in] array that will contains the items
			//! \param[in] index of array where to start the copy
			//! \param[in] count number of items to copy
			//! \param[in] start index of the structure
			//! \return true if the content was copied with success
			virtual bool CopyTo(T* array, svar index, svar count, svar start) const = 0;

			svar operator () ()
			{
				return Count();
			}
		};
	}
}
