#include "pch.h"
#include <XeSDK/IObject.h>

Xe::IObject::IObject()
{
	i_count = 1;
}
uvar Xe::IObject::AddRef()
{
	return ++i_count;
}
uvar Xe::IObject::Release()
{
	i_count--;
	if (i_count == 0)
	{
		delete this;
		return 0;
	}
	return i_count;
}