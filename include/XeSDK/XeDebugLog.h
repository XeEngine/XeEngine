#pragma once
#include <XeSDK/XeDef.h>

#if defined(DEBUGLOG)
#define ASSERT(_Expression) (void)( (!!(_Expression)) || (Xe::Debug::Log::Print(Xe::Debug::Log::Priority_Assertion, Xe::Debug::Log::Type_Generic, _XE_WIDE(__FILE__), __LINE__, _XE_WIDE(#_Expression)), 0) )
#define LOG(p, t, msg, ...) Xe::Debug::Log::Print(p, t, _XE_WIDE(__FILE__), __LINE__, msg, ##__VA_ARGS__)
#else
#define ASSERT
#define LOG ((void)0);
#endif

namespace Xe {
	namespace Debug {
		class Log {
		public:
			enum Priority {
				Priority_Diagnostics,
				Priority_Info,
				Priority_Warning,
				Priority_Error,
				Priority_Critical,
				Priority_Assertion,
			};
			enum Type {
				Type_Generic,
				Type_Assertion,
				Type_Module,
				Type_System,
				SurfaceType_Memory,
				Type_IO,
				Type_Graphics,
				Type_Font,
				Type_Sound,
			};

			typedef void(__stdcall * PFNLOGOUT) (Priority priority, ctstring text);

			static void SetLevelFilter(Priority priority);
			static void SetTypeFilter(Type type, bool enabled);
			static PFNLOGOUT GetDefaultLogOut();
			static void SetLogOut(PFNLOGOUT logout);
			static void Print(Priority priority, Type type, ctstring file, int line, ctstring msgfmt, ...);
		};
	}
}