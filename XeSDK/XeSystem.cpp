#include "pch.h"
#include <XeSDK/XeSystem.h>

const Xe::System::Version& Xe::System::GetVersion() {
	static const Version version = { 0, 6, 1 };
	return version;
}
ctstring Xe::System::GetBuildDate() {
	static const tchar DATETIME_YYYYMMDD[] = {
		__DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], '-',
		__DATE__[0], __DATE__[1], __DATE__[2], '-',
		__DATE__[4], __DATE__[5], '_',
		__TIME__[0], __TIME__[1], '-',
		__TIME__[3], __TIME__[4], '-',
		__TIME__[6], __TIME__[7], '\0',
	};
	return DATETIME_YYYYMMDD;
}