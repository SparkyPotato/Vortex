#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace Vortex
{
	/*
		A structure to hold all log data.
	*/
	struct Log
	{
		Log(std::string msg, spdlog::level::level_enum lvl, std::string log)
			: message(msg), level(lvl), logger(log)
		{}

		std::string message;
		spdlog::level::level_enum level;
		std::string logger;
	};

	/*
		Custom spdlog sink for the Vortex Editor.
	*/
	template <typename Mutex>
	class EditorSink : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		EditorSink() {}

		/*
			Gets all the logs.
		*/
		const std::vector<Log>& GetLog() { return m_Logs; }

		/*
			Shrinks the log to the last `size` messages, if it is greater than `size`
		*/
		void Shrink(int size)
		{
			if ((int) m_Logs.size() > size)
			{
				m_Logs.erase(m_Logs.begin(), m_Logs.end() - size - 1);
			}
		}

		/*
			Clears all the logs.
		*/
		void Clear() { m_Logs.clear(); }

		void SetLogCallback(std::function<void(Log&)> callback) { m_Callback = callback; }

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			base_sink<Mutex>::formatter_->format(msg, formatted);

			// Pushes the log into the log vector.
			m_Logs.emplace_back(fmt::to_string(formatted), msg.level, msg.logger_name.data());

			if (m_Callback) m_Callback(*(m_Logs.end() - 1));
		}

		void flush_() override
		{
			/*
				Flush doesn't do anything because we are using an immediate-mode GUI,
				and the log vector is read every frame.

				If we start using a retained-mode GUI, flush should tell the GUI to update the log.
			*/

		}

	private:
		// Stores all the logs.
		std::vector<Log> m_Logs;
		std::function<void(Log&)> m_Callback = nullptr;
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

		static std::shared_ptr<EditorSink_mt> GetEditorSink() { return s_EditorSink; }

		static std::shared_ptr<spdlog::logger> CreateLogger(std::string name, spdlog::level::level_enum logLevel);

	private:
		static std::vector<spdlog::sink_ptr> s_Sinks;
		static std::shared_ptr<EditorSink_mt> s_EditorSink;
	};
}

#define DECLARE_LOGGER(loggerName) \
extern std::shared_ptr<spdlog::logger> Logger##loggerName

#define DEFINE_LOGGER(loggerName) \
std::shared_ptr<spdlog::logger> Logger##loggerName = nullptr

#define CREATE_LOGGER(loggerName, logLevel) \
::Logger##loggerName = Vortex::Logger::CreateLogger(#loggerName, logLevel)

// Log Macros. 'logger' is a logger created by CREATE_LOGGER(loggerName, logLevel).
#define VX_TRACE(logger, ...)  ::Logger##logger->trace(__VA_ARGS__)
#define VX_DEBUG(logger, ...)  ::Logger##logger->debug(__VA_ARGS__)
#define VX_INFO(logger, ...)   ::Logger##logger->info(__VA_ARGS__)
#define VX_WARN(logger, ...)   ::Logger##logger->warn(__VA_ARGS__)
#define VX_ERROR(logger, ...)  ::Logger##logger->error(__VA_ARGS__)

DECLARE_LOGGER(LogDefault);
