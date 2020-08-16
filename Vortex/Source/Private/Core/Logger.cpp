#include <VXpch.h>
#include <Core/Logger.h>
#include <spdlog/sinks/basic_file_sink.h>

DEFINE_LOGGER(LogDefault);

namespace Vortex
{
	// Creating spdlog sinks.
	std::shared_ptr<EditorSink_mt> Logger::s_EditorSink = std::make_shared<EditorSink_mt>();
	std::vector<spdlog::sink_ptr> Logger::s_Sinks = { Logger::GetEditorSink(), std::make_shared<spdlog::sinks::basic_file_sink_mt>("Temp/Log.txt", true) };

	void Logger::Init()
	{
		CREATE_LOGGER(LogDefault, spdlog::level::trace);

		// Setting the log pattern.
		spdlog::set_pattern("%I:%M:%S.%e [%n] %l: %^%v%$");
	}

	std::shared_ptr<spdlog::logger> Logger::CreateLogger(std::string name, spdlog::level::level_enum logLevel)
	{
		std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(name, s_Sinks.begin(), s_Sinks.end());
		logger->set_level(logLevel);
		logger->set_pattern("%I:%M:%S.%e [%n] %l: %^%v%$");

		return logger;
	}
}
