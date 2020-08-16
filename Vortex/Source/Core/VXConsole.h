#pragma once

namespace Vortex
{
	DECLARE_LOGGER_EXTERN(LogConsole);

	struct ConsoleCommand
	{
		std::string command;
	};

	class VXConsole
	{
	public:
		static void Init();
		static void Shutdown();

		static VXConsole* Get() { return s_Console; }

		void SubmitCommand(std::string commandString);

	private:
		VXConsole();
		~VXConsole();

		static VXConsole* s_Console;
	};
}
