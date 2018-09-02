//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IHashable header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IObject.h>

namespace Xe { namespace Collections {
	interface IHashable : public IObject
	{
		//! \brief get the hash from current object
		//! \return hash code
		virtual u64 GetHash() const = 0;

		//! \brief get the hash from current object
		//! \details same as GetHash
		//! \sa GetHash
		u64 operator () () const
		{
			return GetHash();
		}
	};
} }