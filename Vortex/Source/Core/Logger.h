#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace Vortex
{
	enum class LoggerName
	{
		Engine,
		Client
	};

	struct Log
	{
		Log(std::string msg, spdlog::level::level_enum lvl, LoggerName loc)
			: message(msg), level(lvl), location(loc)
		{}

		std::string message;
		spdlog::level::level_enum level;
		LoggerName location;
	};

	template <typename Mutex>
	class EditorSink : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		EditorSink() {}

		const std::vector<Log>& GetLog() { return m_Logs; }
		void Shrink(int size)
		{
			if ((int) m_Logs.size() > size)
			{
				m_Logs.erase(m_Logs.begin(), m_Logs.end() - size - 1);
			}
		}
		void Clear() { m_Logs.clear(); }

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			base_sink<Mutex>::formatter_->format(msg, formatted);

			LoggerName logger = LoggerName::Engine;

			if (msg.logger_name == "Client")
				logger = LoggerName::Client;
			else
				logger = LoggerName::Engine;

			m_Logs.emplace_back(fmt::to_string(formatted), msg.level, logger);
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
#define ENG_TRACE(...) Vortex::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define ENG_DEBUG(...) Vortex::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define ENG_INFO(...)  Vortex::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define ENG_WARN(...)  Vortex::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define ENG_ERROR(...) Vortex::Logger::GetEngineLogger()->error(__VA_ARGS__)

// Client Log macros. Use in your application.
#define VX_TRACE(...)  Vortex::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define VS_DEBUG(...)  Vortex::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define VX_INFO(...)   Vortex::Logger::GetClientLogger()->info(__VA_ARGS__)
#define VX_WARN(...)   Vortex::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define VX_ERROR(...)  Vortex::Logger::GetClientLogger()->error(__VA_ARGS__)
