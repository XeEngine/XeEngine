#pragma once
#include <XeSDK/XeDef.h>

// create an error message compatible with XeEngine
// |E|C|HHHHHH|LLLLLLLL|CCCCCCCCCCCCCCCC
// E = 1 if it's an error, 0 if it isn't
// C = 0 for Xe* products, 1 for custom
// H = high module
// L = low module
// C = message code
#define XE_CREATE_RESULT(iserror, iscustom, himodule, lomodule, code, name) \
	static const RESULT name = (RESULT)((s32)(((iserror) != 0) << 31) | (((iscustom) != 0) << 30) \
	| (((himodule) & 0x3F) << 24) | (((lomodule) & 0xFF) << 16) | ((code) & 0xFFFF))

#define HM_RESERVED		0
#define HM_SDK			1
#define HM_WIDGET		2
#define HM_EDITOR		3
#define HM_SCRIPT		4
#define HM_SYNTH		5

#define LM_GENERAL				0x00
#define LM_SYSTEM				0x01
#define LM_NET					0x08
#define LM_DEVICE				0x10
#define LM_DEVICE_WINDOW		0x11
#define LM_DEVICE_VIDEO			0x12
#define LM_DEVICE_INPUT			0x13
#define LM_DEVICE_AUDIO			0x14
#define LM_STORAGEFILE			0x20
#define LM_STORAGEDIR			0x21
#define LM_GRAPHICS				0x30
#define LM_GRAPHICS_IMAGE		0x31
#define LM_GRAPHICS_SURFACE		0x32
#define LM_PARSER				0x80

#define ISERROR(r)	((r) < 0)

namespace Xe
{
	typedef s32 RESULT;

	//! \brief contains error messages
	namespace Error
	{
		//! \brief success
		XE_CREATE_RESULT(0, 0, HM_SDK, LM_GENERAL, 0x0000, OK);
		//! \brief generic error
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0000, GENERIC_ERROR);
		//! \brief function not implemented yet
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0001, NOT_IMPLEMENTED);
		//! \brief function not supported from platform
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0002, NOT_SUPPORTED);
		//! \brief invalid handle
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0003, INVALID_HANDLE);
		//! \brief when a required valid pointer is set to nullptr
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0004, NULL_POINTER);
		//! \brief invalid data
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0005, INVALID_DATA);
		//! \brief when RAM is out of memory
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0006, OUT_OF_MEMORY);
		//! \brief when a required of input/output failed
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0007, IO_ERROR);
		//! \brief when an invalid parameter is passed to a function
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_GENERAL, 0x0008, INVALID_PARAMETER);
	}
}