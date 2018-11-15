cmake_minimum_required (VERSION 3.7)

set(TEST_NAME XeSDKTest)
add_executable (${TEST_NAME}
	main.cpp
#	XeStringTest.cpp
)

target_link_libraries(${TEST_NAME}
	XeSDK
	${GTEST_BOTH_LIBRARIES}
	pthread)

add_test (NAME ${TEST_NAME} COMMAND ${TEST_NAME})
