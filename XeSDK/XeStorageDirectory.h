#pragma once
#include <XeSDK/IStorageDirectory.h>
#include <XeSDK/XeError.h>

namespace Xe {
	namespace Storage {
		namespace Directory {
			bool Create(ctstring path);
			bool Exists(ctstring path);
			RESULT Delete(ctstring path);
			RESULT Move(ctstring oldpath, ctstring newpath);
			RESULT Open(IDirectory** ppDirectory, ctstring path);


			Xe::RESULT GetCurrent(tstring path, uvar length);
			Xe::RESULT SetCurrent(ctstring path);
		}
	}
}