#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Mashenka
{
    class MASHENKA_API Log
    {
    public:

        static void Init();

        inline static  std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}
        inline static  std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}
        
    private:
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };

}

//Core Log Macros
#define MK_CORE_FATAL(...)   ::Mashenka::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define MK_CORE_ERROR(...)   ::Mashenka::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MK_CORE_WARN(...)    ::Mashenka::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MK_CORE_INFO(...)    ::Mashenka::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MK_CORE_TRACE(...)   ::Mashenka::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define MK_FATAL(...)        ::Mashenka::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define MK_ERROR(...)        ::Mashenka::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MK_WARN(...)         ::Mashenka::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MK_INFO(...)         ::Mashenka::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MK_TRACE(...)        ::Mashenka::Log::GetCoreLogger()->trace(__VA_ARGS__)