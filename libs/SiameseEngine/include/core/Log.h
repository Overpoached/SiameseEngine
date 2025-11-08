#pragma once

#include <spdlog/spdlog.h>

namespace sengine
{
    class Logger
    {
    public:
        //if a file name is provided, a file write sink will be created
        Logger(const std::string& name, const std::string& filename = "");
        ~Logger() = default;

        template <typename... Args>
        void Trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            m_logger->log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            m_logger->log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            m_logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            m_logger->log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            m_logger->log(spdlog::level::err, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            m_logger->log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };

    class LoggerService
    {
    public:
        static void Register(std::shared_ptr<Logger> logger) noexcept { s_instance = logger; }
        static bool IsRegistered() { return s_instance != nullptr; }
        static std::shared_ptr<Logger> Get() noexcept { return s_instance; }

    private:
        static inline std::shared_ptr<Logger> s_instance = nullptr;
    };
}

#define SENGINE_TRACE(...) sengine::LoggerService::Get()->Trace(__VA_ARGS__)
#define SENGINE_DEBUG(...) sengine::LoggerService::Get()->Debug(__VA_ARGS__)
#define SENGINE_INFO(...) sengine::LoggerService::Get()->Info(__VA_ARGS__)
#define SENGINE_WARN(...) sengine::LoggerService::Get()->Warn(__VA_ARGS__)
#define SENGINE_ERROR(...) sengine::LoggerService::Get()->Error(__VA_ARGS__)
#define SENGINE_CRITICAL(...) sengine::LoggerService::Get()->Critical(__VA_ARGS__)