#pragma once
#include "Mashenka/Core/Base.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// ReSharper disable once CppUnusedIncludeDirective
#include "spdlog/fmt/ostr.h" // for operator overload

/*
 * 1. **Introduction to SPDLOG**: SPDLOG is a fast, header-only/compiled, C++ logging library.
 * It is used for logging messages in a game engine.
 * It is a third-party library that can be integrated into the game engine.
 * 
 * 2. **Adding SPDLOG to the Project**: The SPDLOG library can be added to the project as a Git submodule.
 * This allows the library to be updated easily when new changes are made to the library.
 * The library is stored in a folder called 'vendor' inside the Hazel project.
 *
 * 3. **Setting Up the Logger**: The logger is set up by including the SPDLOG library in the project and
 * creating a wrapper class called 'Log'.
 * This class contains two static shared pointers to SPDLOG loggers,
 * one for the core engine (coreLogger) and one for the client (clientLogger).
 * An initialization function (Init) is also created to set up these loggers.
 */
namespace Mashenka
{
    class Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_ClientLogger;
        static Ref<spdlog::logger> s_CoreLogger;
    };
}

/*
 *4. **Creating Log Macros**: Macros are created for different log levels (trace, info, warn, error, fatal)
 *for both the core engine and the client.
 *These macros make it easier to log messages at different levels.
 *They also allow for the possibility of stripping out log messages in distribution builds for performance reasons.
 *
 * 5. **Using the Logger**: The logger can be used by calling the log macros with the message to be logged.
 * For example, HZ_CORE_WARN("Initialized Log") would log a warning message saying "Initialized Log"
 * from the core engine.
 */
// __VA_ARGS__ is a special identifier that represents all arguments passed to the macro.
//Core Log Macros
#define MK_CORE_CRITICAL(...)   ::Mashenka::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define MK_CORE_ERROR(...)      ::Mashenka::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MK_CORE_WARN(...)       ::Mashenka::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MK_CORE_INFO(...)       ::Mashenka::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MK_CORE_TRACE(...)      ::Mashenka::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define MK_CRITICAL(...)        ::Mashenka::Log::GetClientLogger()->critical(__VA_ARGS__)
#define MK_ERROR(...)           ::Mashenka::Log::GetClientLogger()->error(__VA_ARGS__)
#define MK_WARN(...)            ::Mashenka::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MK_INFO(...)            ::Mashenka::Log::GetClientLogger()->info(__VA_ARGS__)
#define MK_TRACE(...)           ::Mashenka::Log::GetClientLogger()->trace(__VA_ARGS__)
