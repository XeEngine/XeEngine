#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Storage { namespace Path {
	bool IsValid(ctstring path);

	ctstring GetExtension(ctstring fileName);
} } }