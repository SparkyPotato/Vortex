#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Vortex
{
	template <typename Mutex>
	class EditorSink : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		enum class LogLevel
		{
			Trace = 0,
			Info = 1,
			Warn = 2,
			Error = 3
		};

		struct Log
		{
			Log(std::string msg, LogLevel lvl)
				: message(msg), level(lvl)
			{}

			std::string message;
			LogLevel level;
		};

		EditorSink() {}

		const std::vector<Log>& GetLog() { return m_Logs; }
		void Clear() { m_Logs.clear(); }

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			base_sink<Mutex>::formatter_->format(msg, formatted);
			
			// Extremely crappy code to find the error message.
			LogLevel logLevel = LogLevel::Trace;
			std::string string = fmt::to_string(formatted);

			if (*(string.c_str() + 36) == 'i')
				logLevel = LogLevel::Info;
			else if (*(string.c_str() + 36) == 'w')
				logLevel = LogLevel::Warn;
			else if (*(string.c_str() + 36) == 'e')
				logLevel = LogLevel::Error;

			m_Logs.emplace_back(string, logLevel);
		}

		void flush_() override
		{
			
		}

	private:
		std::vector<Log> m_Logs;
	};

	using EditorSink_mt = EditorSink<std::mutex>;
	using EditorSink_st = EditorSink<spdlog::details::null_mutex>;

	class Logger
	{
	public:
		/*
			Initializes the Logger. Is called in the Entry Point so don't you worry about it.
		*/
		static void Init();

		static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_EngineLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
		static std::shared_ptr<EditorSink_mt> GetEditorSink() { return s_EditorSink; }

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		static std::shared_ptr<EditorSink_mt> s_EditorSink;
	};
}

// Engine Log macros. Don't use in your application.
#define ENG_TRACE(...) Vortex::Logger::GetEngineLogger()->trace(__VA_ARGS__);
#define ENG_INFO(...)  Vortex::Logger::GetEngineLogger()->info(__VA_ARGS__);
#define ENG_WARN(...)  Vortex::Logger::GetEngineLogger()->warn(__VA_ARGS__);
#define ENG_ERROR(...) Vortex::Logger::GetEngineLogger()->error(__VA_ARGS__);

// Client Log macros. Use in your application.
#define VX_TRACE(...)  Vortex::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define VX_INFO(...)   Vortex::Logger::GetClientLogger()->info(__VA_ARGS__);
#define VX_WARN(...)   Vortex::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define VX_ERROR(...)  Vortex::Logger::GetClientLogger()->error(__VA_ARGS__);
