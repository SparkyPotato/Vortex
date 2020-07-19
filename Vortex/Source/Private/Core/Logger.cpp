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

		// Setting the log pattern.
		spdlog::set_pattern("[%I:%M:%S.%e] %n: (%l) %^%v%$");

		// Setting log level based on configuration.
		#ifdef CFG_DEBUGALL

		s_EngineLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_level(spdlog::level::trace);

		#elif CFG_DEBUGENG

		s_EngineLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_level(spdlog::level::warn);

		#elif CFG_DEBUGAPP

		s_EngineLogger->set_level(spdlog::level::warn);
		s_ClientLogger->set_level(spdlog::level::trace);

		#else

		s_EngineLogger->set_level(spdlog::level::warn);
		s_ClientLogger->set_level(spdlog::level::warn);

		#endif
	}
}
