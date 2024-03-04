#include "mkpch.h"
#include "Mashenka/Core/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h> // for console logging
#include <spdlog/sinks/basic_file_sink.h> // for file logging

namespace Mashenka
{
    // Static pointers for logger objects
    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;

    // Initialize the logger objects
    void Log::Init()
    {
        // Prepare two sinks: one for console logging and one for file logging
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>()); // console logging
        logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("Mashenka.log", true)); // file logging

        // Set the pattern for the loggers
        // %^ and %$ are used to set the color for the console logging
        // %T is the time, %n is the name of the logger, %v is the message
        // %l is the level of the message, which is the log level
        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        // set the core logger
        s_CoreLogger = CreateRef<spdlog::logger>("MASHENKA", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        // Set the client logger
        s_ClientLogger = CreateRef<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
    }
}
