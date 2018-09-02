//////////////////////////////////////////////////////////////////////////
/* XeEngine
 * Xe::Collections::IEnumerable header
 * Developed by Xeeynamo (Luciano Ciccariello)
 * This file is part of XeEngine. Please do not share.
 *////////////////////////////////////////////////////////////////////////

#pragma once
#include <XeSDK/ICollection.h>

namespace Xe { namespace Collections {
	template <class T, class P>
	interface IEnumerable : public ICollection<T>
	{
		//! \brief used as position
		typedef P Position;

		//! \brief check if the current position is the first
		//! \return true if it is
		virtual bool IsFirst(const Position) const = 0;

		//! \brief check if the current position reaches the end
		//! \return true if it is
		virtual bool IsEnd(const Position) const = 0;

		//! \brief move forward by one position
		//! \return current position
		virtual Position Next(const Position) const = 0;

		//! \brief move backward by one position
		//! \return current position
		virtual Position Prev(const Position) const = 0;

		//! \brief move to the begin
		//! \return current position
		virtual Position Begin() const = 0;

		//! \brief move to the end
		//! \return current position
		virtual Position End() const = 0;
	};
} }
