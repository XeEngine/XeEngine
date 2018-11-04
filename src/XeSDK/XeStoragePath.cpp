#include "pch.h"
#include <XeSDK/XeStoragePath.h>

namespace Xe { namespace Storage { namespace Path {
    bool IsValid(ctstring path) {
        while (true) {
            switch (*path++) {
                case '\0':
                    return true;
                case '?':
                case '%':
                case '*':
                case '|':
                case '>':
                case '<':
                    return false;
            }
        }
    }

	ctstring GetExtension(ctstring fileName)
	{
		int index = String(fileName).LastIndexOf('.');
		return index >= 0 ? fileName + index : "";
	}
} } }