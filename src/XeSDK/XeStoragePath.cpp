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
		return Xe::String::FindLastCharacter(fileName, '.');
	}
} } }