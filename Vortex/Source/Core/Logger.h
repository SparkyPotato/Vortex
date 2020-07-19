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

// Engine Log macros.
#define ENG_TRACE(...) Vortex::Logger::GetEngineLogger()->trace(__VA_ARGS__);
#define ENG_INFO(...)  Vortex::Logger::GetEngineLogger()->info(__VA_ARGS__);
#define ENG_WARN(...)  Vortex::Logger::GetEngineLogger()->warn(__VA_ARGS__);
#define ENG_ERROR(...) Vortex::Logger::GetEngineLogger()->error(__VA_ARGS__);

// Client Log macros.
#define VX_TRACE(...)  Vortex::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define VX_INFO(...)   Vortex::Logger::GetClientLogger()->info(__VA_ARGS__);
#define VX_WARN(...)   Vortex::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define VX_ERROR(...)  Vortex::Logger::GetClientLogger()->error(__VA_ARGS__);
