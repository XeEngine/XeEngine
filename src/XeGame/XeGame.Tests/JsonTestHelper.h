#pragma once
#include <list>

#define STREAM_FROM_FIXEDARRAY(a) ObjPtr<IStream>(Xe::IO::IMemoryStream::From(a, lengthof(a)))

template <class T>
T& Add(std::list<T>& list)
{
	list.push_back(T());
	return *(std::prev(list.end()));
}

template <class T>
T& Add(std::vector<T>& list)
{
	list.push_back(T());
	return *(std::prev(list.end()));
}