#include "sepch.h"

#include "core/Log.h"

#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

//helper to get current time
std::string GetTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto t_c = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
#ifdef _WIN32
	localtime_s(&tm, &t_c);
#else
	localtime_r(&t_c, &tm);
#endif
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
	return oss.str();
}

sengine::Logger::Logger(const std::string& name, const std::string& filename)
{
	m_logger = std::make_shared<spdlog::logger>(name);
	m_logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	if (!filename.empty())
	{
		std::filesystem::path exePath = std::filesystem::current_path();
		std::filesystem::path logPath = exePath / "logs" / filename;
		logPath += "_" + GetTimestamp() + ".log";
		m_logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath.string(), true));

	}
	m_logger->set_pattern("[%T] %n: %v%$");
	m_logger->set_level(spdlog::level::trace);
}