#pragma once
#include <XeSDK/XeDef.h>

namespace Xe { namespace Crypto {
	constexpr u32 Fnv1a32(const char data[], u32 hash, int index, int remains) {
		return remains == 0 ? hash :
			Fnv1a32(data, (hash ^ data[index]) * 16777619U, index + 1, remains - 1);
	}

	template <int length>
	constexpr u32 Fnv1a32(const char(&data)[length]) {
		return Crc32(data, length - 1);
	}

	constexpr u32 Fnv1a32(const char data[], int length) {
		return Fnv1a32(data, 2166136261U, 0, length);
	}

	constexpr u64 Fnv1a64(const char data[], u64 hash, int index, int remains) {
		return remains == 0 ? hash :
			Fnv1a64(data, (hash ^ data[index]) * 1099511628211ULL, index + 1, remains - 1);
	}

	template <int length>
	constexpr u64 Fnv1a64(const char(&data)[length]) {
		return Crc32(data, length - 1);
	}

	constexpr u64 Fnv1a64(const char data[], int length) {
		return Fnv1a64(data, 14695981039346656037ULL, 0, length);
	}
} }