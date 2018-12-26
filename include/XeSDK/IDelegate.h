#pragma once
#include <XeSDK/IObject.h>

namespace Xe
{
	template <class TSender, typename TArgs>
	struct EventArgs
	{
		TSender Sender;
		TArgs Arguments;
	};


	template <typename TArgs>
	interface IDelegate
	{
		virtual void operator()(TArgs args) = 0;
	};

	template <typename TArgs>
	interface IDelegateRef : public virtual IDelegate<TArgs>, public virtual Xe::IObject
	{
		virtual void operator()(TArgs args) = 0;
	};
}