//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IComparer header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IObject.h>

namespace Xe { namespace Collections {
	interface IComparable : public IObject
	{
		//! \brief interface id
		static const UID ID = 0xa614539dd682482fL;

		//! \brief compare the current object with another one
		//! \param[in] obj the object to compare
		/** \return less than zero if the current object precedes obj
			* zero if the current object occurs in the same position of obj
			* greater than zero if the current object follows obj
			*/
		virtual svar CompareTo(const IComparable& obj) const = 0;

		//! \brief compare two objects
		//! \details same as Compare(str) == 0
		bool operator == (const IComparable& obj) const
		{
			return CompareTo(obj) == 0;
		}
		//! \brief compare two objects
		//! \details same as Compare(str) != 0
		bool operator != (const IComparable& obj) const
		{
			return CompareTo(obj) != 0;
		}
		//! \brief compare two objects
		//! \details same as Compare(str) > 0
		bool operator > (const IComparable& obj) const
		{
			return CompareTo(obj) > 0;
		}
		//! \brief compare two objects
		//! \details same as Compare(str) < 0
		bool operator < (const IComparable& obj) const
		{
			return CompareTo(obj) < 0;
		}
		//! \brief compare two objects
		//! \details same as Compare(str) >= 0
		bool operator >= (const IComparable& obj) const
		{
			return CompareTo(obj) >= 0;
		}
		//! \brief compare two objects
		//! \details same as Compare(str) <= 0
		bool operator <= (const IComparable& obj) const
		{
			return CompareTo(obj) <= 0;
		}
	};
} }