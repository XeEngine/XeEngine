#pragma once

#define ASSERT_POSITIVE(x) \
	if (x < 0) { throw std::invalid_argument("\""#x"\" cannot be a negative number"); }

#define ASSERT_CPOSITION(x, l) { \
	ASSERT_POSITIVE(x); \
	if (x >= l) { throw std::invalid_argument("\""#x"\" is greater than the length of the string"); } }

#define ASSERT_CPOSITION_ALT(x, l) { \
	ASSERT_POSITIVE(x); \
	if (x > l) { throw std::invalid_argument("\""#x"\" is greater than the length of the string"); } }

#define ASSERT_POSITION(x) ASSERT_CPOSITION(x, m_Length)