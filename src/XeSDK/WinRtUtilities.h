#pragma once

template <typename T, unsigned Count>
auto GetActivationFactory(WCHAR const (&classId)[Count]) -> Microsoft::WRL::ComPtr<T>
{
	HSTRING_HEADER header;
	HSTRING string;
	HR(WindowsCreateStringReference(classId,
		Count - 1,
		&header,
		&string));
	Microsoft::WRL::ComPtr<T> result;
	HR(RoGetActivationFactory(string,
		__uuidof(T),
		reinterpret_cast<void **>(result.GetAddressOf())));
	return result;
}