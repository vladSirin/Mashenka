#include "mkpch.h"
#include "Log.h"

namespace Mashenka
{
    // Static pointers for logger objects
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    // Initialize the logger objects
    void Log::Init()
    {
        // "%^[%T] %n: %v%$" means that each log message will start with the time (%T),
        // followed by the logger's name (%n), a colon, and then the log message (%v).
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("MASHENKA");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}
