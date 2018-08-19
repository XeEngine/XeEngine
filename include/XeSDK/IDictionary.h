//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IDictionary header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/ICollection.h>

namespace Xe { namespace Collections {
	template <class T, class K>
	interface IDictionary : public ICollection<T>
	{
		//! \brief interface id
		static const UID ID = 0x562c6725d6814c43L;

		//! \brief add an item
		//! \param[in] key of the value
		//! \param[in] value to add
		//! !
		virtual void Add(const K& key, const T& value) = 0;

		//! \brief remove all items
		virtual void Clear() = 0;

		//! \brief remove an element
		//! \param[in] key of the value
		virtual void Remove(const K& key) = 0;

		//! \brief check if the specified key is inside the structure
		//! \param[in] key to search
		//! \return true if the item was found
		virtual bool Contains(const K& key) = 0;

		//! \brief get the value from the specified key
		//! \param[in] key to search
		//! \param[out] value read
		//! \return true if the item was read
		virtual bool GetValue(const K& key, T& value) = 0;

		//! \brief get the index of the specified key
		//! \param[in] key to search
		//! \return key's index; if < 0, the key was not found
		virtual svar IndexOf(const K& key) = 0;

		//! \brief read the item inside the dictionary from the specified index
		//! \param[in] index of dictionary
		//! \param[in] key of value to be sure of the integrity of dictionary
		//! \param[out] value read
		//! \return true if the value was read with success
		virtual bool Read(svar index, const K& key, T& value) = 0;

		//! \brief write the item inside the dictionary to the specified index
		//! \param[in] index of dictionary
		//! \param[in] key of value to be sure of the integrity of dictionary
		//! \param[out] value to write
		//! \return true if the value was written with success
		virtual bool Write(svar index, const K& key, const T& value) = 0;
	};
} }