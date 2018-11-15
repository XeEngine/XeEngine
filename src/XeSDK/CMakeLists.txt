cmake_minimum_required(VERSION 3.7)

project (XeSDK)

include_directories(XeSDK PRIVATE .)

add_library(XeSDK STATIC
	XeCore.cpp
	XeMemory.cpp
	XeString.cpp
	XeStringSpan.cpp)

include (XeSDK.Shared.Unix/CMakeInclude.txt)
