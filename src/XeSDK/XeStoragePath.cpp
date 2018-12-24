#include "pch.h"
#include <XeSDK/XeStoragePath.h>
#include <XeSDK/XeMath.h>

namespace Xe { namespace Storage { namespace Path {

	bool InternalIsPathSeparator(char ch)
	{
		return ch == WindowsDirectorySeparator ||
			ch == UnixDirectorySeparator;
	}

	int InternalGetIndexOfDirectorySeparator(const StringSpan& path)
	{
		return Math::Max(
			path.IndexOf(WindowsDirectorySeparator),
			path.IndexOf(UnixDirectorySeparator));
	}

	int InternalGetIndexOfLastDirectorySeparator(const StringSpan& path)
	{
		return Math::Max(
			path.LastIndexOf(WindowsDirectorySeparator),
			path.LastIndexOf(UnixDirectorySeparator));
	}

	int InternalGetIndexOfExtension(const StringSpan& path)
	{
		auto index = path.LastIndexOf('.');
		if (index < 0)
			return path.GetLength();

		const auto substr = path.Substring(index);
		if (InternalGetIndexOfLastDirectorySeparator(substr) >= 0)
			return path.GetLength();

		return index;
	}

	int InternalGetNestedPathIndex(const StringSpan& path, int depth)
	{
		if (depth > 0)
		{
			auto index = InternalGetIndexOfDirectorySeparator(path);
			if (index >= 0)
			{
				if (index < path.GetLength())
				{
					const auto substr = path.Substring(index + 1);
					const auto i = InternalGetNestedPathIndex(substr, depth - 1);
					return index + i + 1;
				}

				return index;
			}

			return path.GetLength() + 1;
		}

		return 0;
	}

	StringSpan InternalGetNestedPath(const StringSpan& path, int depth)
	{
		auto index = InternalGetNestedPathIndex(path, depth);
		return path.Substring(0, index - 1);
	}

	String ChangeExtension(const StringSpan& path, const StringSpan& extension)
	{
		const auto indexExtension = InternalGetIndexOfExtension(path);
		const auto pathWithoutExtension = String(path.Substring(0, indexExtension));

		if (extension.GetLength() == 0 || extension[0] != '.')
		{
			return pathWithoutExtension + "." + extension;
		}

		return pathWithoutExtension +extension;
	}

	String Combine(const StringSpan& path1, const StringSpan& path2)
	{
		if (path1.IsEmptyOrWhitespace())
			return path2;

		if (path2.IsEmptyOrWhitespace())
			return path1;

		const char dirSeparator[]{ UnixDirectorySeparator, '\0' };
		auto str1 = String(path1);

		if (!InternalIsPathSeparator(path1[path1.GetLength() - 1]) &&
			!InternalIsPathSeparator(path2[0]))
		{
			return str1 + dirSeparator + path2;
		}

		return str1 + path2;
	}

	StringSpan GetDirectoryName(const StringSpan& path)
	{
		auto index = InternalGetIndexOfLastDirectorySeparator(path);

		if (index <= 0)
		{
			if (index < 0)
				return StringSpan::Empty;

			if (path.GetLength() > 1)
				return path.Substring(0, 1);
		}

		return path.Substring(0, index);
	}

	StringSpan GetExtension(const StringSpan& path)
	{
		return path.Substring(InternalGetIndexOfExtension(path));
	}

	StringSpan GetFileName(const StringSpan& path)
	{
		auto index = InternalGetIndexOfLastDirectorySeparator(path) + 1;
		if (index >= path.GetLength())
			return StringSpan::Empty;

		return path.Substring(index);
	}

	StringSpan GetFileNameWithoutExtension(const StringSpan& path)
	{
		auto index = InternalGetIndexOfLastDirectorySeparator(path) + 1;
		if (index >= path.GetLength())
			return StringSpan::Empty;

		auto retPath = path.Substring(index);

		auto extIndex = retPath.LastIndexOf('.');
		return extIndex >= 0 ? retPath.Substring(0, extIndex) : retPath;
	}

	StringSpan GetPathRoot(const StringSpan& path)
	{
		const char pathSeparators[]
		{
			WindowsDirectorySeparator,
			UnixDirectorySeparator
		};

		auto containsVolumeDir = path.GetLength() >= 2 &&
			String::IsAlpha(path[0]) &&
			path[1] == ':';


		const auto strSeparator = StringSpan(pathSeparators);

		auto rootIndex = InternalGetIndexOfDirectorySeparator(path);
		if (rootIndex < 0 || rootIndex > 0)
		{
			if (containsVolumeDir)
				return path.Substring(0, rootIndex == 2 ? 3 : 2);

			return StringSpan::Empty;
		}

		// Check if it's an UNC path
		if (path.GetLength() > 1 && InternalIsPathSeparator(path[1]))
		{
			const int UncSeparatorIndex = 4;
			if (path.GetLength() >= UncSeparatorIndex)
			{
				auto uncSubPath = path.Substring(UncSeparatorIndex);

				if (uncSubPath.GetLength() > 0)
				{
					auto index = 0;
					auto includePathSeparator = false;
					switch (path[2])
					{
					case '.': // Windows physical device
						index = InternalGetNestedPathIndex(uncSubPath, 1);
						includePathSeparator = true;
						break;
					case '?': // Windows UNC path
						index = InternalGetNestedPathIndex(uncSubPath, 3);
						includePathSeparator = false;
						break;
					default: // Windows server path
						index = InternalGetNestedPathIndex(uncSubPath, 2);
						includePathSeparator = false;
						break;
					}

					if (includePathSeparator == false &&
						InternalIsPathSeparator(uncSubPath[index - 1]))
						index--;

					return path.Substring(0, UncSeparatorIndex + index);
				}
			}

			// Not well formed UNC path
			return path;
		}

		// Unix path style
		return path.Substring(0, 1);
	}

	bool HasExtension(const StringSpan& path)
	{
		return !GetExtension(path).IsEmpty();
	}

	bool IsPathRooted(const StringSpan& path)
	{
		return !GetPathRoot(path).IsEmpty();
	}

	bool IsValid(const StringSpan& path)
	{
		return path.IndexOfAny("?%*|<>\0") < 0;
	}
} } }