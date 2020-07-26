#include <VXpch.h>
#include <Core/Logger.h>

namespace Vortex
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		// Creating spdlog loggers.
		s_EngineLogger = spdlog::stdout_color_mt("Engine");
		s_ClientLogger = spdlog::stdout_color_mt("Client");

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
