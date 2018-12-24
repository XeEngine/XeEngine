#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeString.h>

namespace Xe { namespace Storage { namespace Path {

	const char WindowsDirectorySeparator = '\\';
	const char UnixDirectorySeparator = '/';

	String ChangeExtension(const StringSpan& path, const StringSpan& extension);

	String Combine(const StringSpan& path1, const StringSpan& path2);

	StringSpan GetDirectoryName(const StringSpan& path);

	StringSpan GetExtension(const StringSpan& path);

	StringSpan GetFileName(const StringSpan& path);

	StringSpan GetFileNameWithoutExtension(const StringSpan& path);

	//! \brief Get the rightmost part of the path that represents the root 
	StringSpan GetPathRoot(const StringSpan& path);

	bool HasExtension(const StringSpan& path);

	bool IsPathRooted(const StringSpan& path);

	bool IsValid(const StringSpan& path);
} } }