//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IComparer header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once

namespace Xe
{
	class String;
	namespace Collections
	{
		interface IStringable : public IObject
		{
			//! \brief get the object as a string
			//! \return object as string
			virtual Xe::String ToString() const = 0;
		};
	}
}