//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IHashable header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/IObject.h>

namespace Xe
{
	namespace Collections
	{
		class IHashable : public IObject
		{
		public:
			//! \brief interface id
			static const UID ID = 0x24e4ff4946a0464fL;

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
	}
}