#include <VXpch.h>
#include <Core/Logger.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Vortex
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
	std::shared_ptr<EditorSink_mt> Logger::s_EditorSink;

	void Logger::Init()
	{
		// Creating spdlog sinks.
		s_EditorSink = std::make_shared<EditorSink_mt>();

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.push_back(s_EditorSink);
		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Temp/Log.txt"));

		// Creating spdlog loggers.
		s_EngineLogger = std::make_shared<spdlog::logger>("Engine", begin(sinks), end(sinks));
		s_ClientLogger = std::make_shared<spdlog::logger>("Client", begin(sinks), end(sinks));

		if (s_EngineLogger == nullptr)
			throw std::exception("Failed to create Engine Logger.");
		if (s_ClientLogger == nullptr)
			throw std::exception("Failed to create Client Logger.");

		// Setting the log pattern.
		spdlog::set_pattern("[%I:%M:%S.%e] %n: (%l) %^%v%$");

		// Setting log level based on configuration.
		#ifdef CFG_DEBUGENG
		s_EngineLogger->set_level(spdlog::level::trace);
		#endif

		#ifdef CFG_DEBUGAPP
		s_ClientLogger->set_level(spdlog::level::trace);
		#endif

		#ifndef CFG_DEBUGENG
		s_EngineLogger->set_level(spdlog::level::warn);
		#endif

		#ifndef CFG_DEBUGAPP
		s_ClientLogger->set_level(spdlog::level::warn);
		#endif
	}
}
