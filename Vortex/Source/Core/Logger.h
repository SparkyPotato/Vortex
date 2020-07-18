#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Vortex
{
	class Logger
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_EngineLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define ENG_TRACE(...) Logger::GetEngineLogger()->trace(__VA_ARGS__);
#define ENG_INFO(...)  Logger::GetEngineLogger()->info(__VA_ARGS__);
#define ENG_WARN(...)  Logger::GetEngineLogger()->warn(__VA_ARGS__);
#define ENG_ERROR(...) Logger::GetEngineLogger()->error(__VA_ARGS__);

#define VX_TRACE(...)  Logger::GetClientLogger()->trace(__VA_ARGS__);
#define VX_INFO(...)   Logger::GetClientLogger()->info(__VA_ARGS__);
#define VX_WARN(...)   Logger::GetClientLogger()->warn(__VA_ARGS__);
#define VX_ERROR(...)  Logger::GetClientLogger()->error(__VA_ARGS__);
