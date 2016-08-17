#include "pch.h"
#include <XeSDK/XeString.h>
#ifdef PLATFORM_LIBC
#include <cstdio>
#include <stdarg.h>
#include <wchar.h>
#endif

tchar* Xe_PrintInteger(tchar* dst, svar value, svar base, svar fieldsize, svar padding)
{
	svar targetsize = Xe::String::Itoa(value, (tchar*)nullptr, base);
	if (fieldsize < targetsize)
		fieldsize = 0;
	else
		fieldsize -= targetsize;
	for (svar i = 0; i < fieldsize; i++)
		*dst++ = (tchar)padding;
	dst += Xe::String::Itoa(value, dst, base);
	return dst;
}
svar Xe::String::PrintFormatted(tchar* dst, svar length, ctstring format, ...)
{
#ifdef PLATFORM_LIBC

	va_list arglist;
	va_start(arglist, format);
#ifdef PLATFORM_UNICODE
#ifdef SECURE_CRT
	return vswprintf_s(dst, length, format, arglist);
#else
	return vswprintf(dst, length, format, arglist);
#endif
#else
#ifdef SECURE_CRT
	return vsnprintf_s(dst, length, length, format, arglist);
#else
	return vsnprintf(dst, length, format, arglist);
#endif
#endif

#else
	va_list arglist;
	va_start(arglist, format);

	tchar* origindst = dst;
	tchar* maxdst = dst + length - 1;
	while (dst < maxdst && *format != '\0')
	{
		tchar ch = *format++;
		if (ch != '%')
			*dst++ = ch;
		else
		{
			tchar ch = *format++;
			tchar padding = ch == '0' ? '0' : ' ';
			svar fieldsize = 0;
			while (ch >= '0' && ch <= '9')
			{
				fieldsize = fieldsize * 10 + ch - '0';
				ch = *format++;
			}
			if (ch == 'l' || ch == 'L')
				ch = *format++;
			switch (ch)
			{
			case 'c':
				*dst++ = (tchar)va_arg(arglist, const svar);
				break;
			case 's':
			{
				ctstring str = va_arg(arglist, ctstring);

				while (*str && dst < maxdst)
				{
					*dst++ = *str++;
				}
			}
			break;
			case 'S':
			{
				/*const IObject* obj = va_arg(arglist, const IObject*);
				const Collections::IStringable* stringable =
				dynamic_cast<const Collections::IStringable*>(obj);

				if (stringable != nullptr)
				{
				const Xe::String& string = stringable->ToString();
				ctstring str = string.GetData();
				while (*(tchar*)str && dst < maxdst)
				{
				*dst++ = *(tchar*)str++;
				}
				}*/
			}
			break;
			case 'b':
				dst = Xe_PrintInteger(dst, va_arg(arglist, const svar), 2, fieldsize, padding);
				break;
			case 'd':
			case 'i':
				dst = Xe_PrintInteger(dst, va_arg(arglist, const svar), 10, fieldsize, padding);
				break;
			case 'o':
				dst = Xe_PrintInteger(dst, va_arg(arglist, const svar), 8, fieldsize, padding);
				break;
			case 'p':
				fieldsize = sizeof(svar) * 2;
			case 'x':
			case 'X':
				dst = Xe_PrintInteger(dst, va_arg(arglist, const svar), 16, fieldsize, padding);
				break;
			}
		}
	}
	*dst++ = '\0';

	va_end(arglist);
	return (dst - origindst) / sizeof(tchar);
#endif
}

svar Xe::String::PrintArguments(char *dst, uvar length, const char *format, ...) {
	va_list args;
	va_start(args, format);
	return PrintArguments(dst, length, format, args);
}
svar Xe::String::PrintArguments(char *dst, uvar length, const char *format, void *args) {
	if (dst == nullptr)
		length = 0;
	return vsnprintf(dst, length, format, *(va_list*)&args);
}
svar Xe::String::PrintArguments(wchar_t *dst, uvar length, const wchar_t *format, ...) {
	va_list args;
	va_start(args, format);
	return PrintArguments(dst, length, format, args);
}
svar Xe::String::PrintArguments(wchar_t *dst, uvar length, const wchar_t *format, void *args) {
	if (dst == nullptr)
		length = 0;
	return _vsnwprintf(dst, length, format, *(va_list*)&args);
}
Xe::String Xe::String::PrintArguments(const char *format, ...) {
	va_list args;
	va_start(args, format);
	return PrintArguments(format, args);
}
Xe::String Xe::String::PrintArguments(const char *format, void *args) {
	int len = vsnprintf(nullptr, 0, format, *(va_list*)&args);
	char *buf = new char[len + 1];
	vsnprintf(buf, len + 1, format, *(va_list*)&args);
	String str(buf, len + 1);
	delete[] buf;
	return str;
}
Xe::String Xe::String::PrintArguments(const wchar_t *format, ...) {
	va_list args;
	va_start(args, format);
	return PrintArguments(format, args);
}
Xe::String Xe::String::PrintArguments(const wchar_t *format, void *args) {
	int len = _vsnwprintf(nullptr, 0, format, *(va_list*)&args);
	wchar_t *buf = new wchar_t[len + 1];
	buf[len] = '\0';
	_vsnwprintf(buf, len, format, *(va_list*)&args);
	String str(buf, len);
	delete[] buf;
	return str;
}