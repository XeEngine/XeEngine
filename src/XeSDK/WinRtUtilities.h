#pragma once

#define HR(x) (ASSERT((x) == S_OK))

template <typename T, unsigned Count>
auto GetActivationFactory(WCHAR const (&classId)[Count]) -> ComPtr<T>
{
	HSTRING_HEADER header;
	HSTRING string;
	HR(WindowsCreateStringReference(classId,
		Count - 1,
		&header,
		&string));
	ComPtr<T> result;
	HR(RoGetActivationFactory(string,
		__uuidof(T),
		reinterpret_cast<void **>(result.GetAddressOf())));
	return result;
}