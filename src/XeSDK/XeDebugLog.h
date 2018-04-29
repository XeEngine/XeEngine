#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeLogger.h>

#if defined(DEBUG) || defined(DEVELOPMENT)
#define ASSERT(_Expression) (void)( (!!(_Expression)) || (Xe::Debug::Logger::Print(Xe::Debug::Logger::Priority_Assertion, Xe::Debug::Logger::Type_Generic, _XE_WIDE(__FILE__), __LINE__, _XE_WIDE(#_Expression)), 0) )
#define LOG(p, t, msg, ...) Xe::Debug::Log::Print(p, t, _XE_WIDE(__FILE__), __LINE__, msg, ##__VA_ARGS__)
#else
#define ASSERT
#define LOG ((void)0);
#endif

namespace Xe { namespace Debug {
    class Log {
    public:
        typedef Xe::Logger::LogLevel Level;

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

        static void Print(Priority priority, Type type, ctstring file, int line, ctstring msgfmt, ...);
    };
} }