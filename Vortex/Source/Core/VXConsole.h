#pragma once

namespace Vortex
{
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

		bool SubmitCommand(std::string commandString);

	private:
		VXConsole();
		~VXConsole();

		static VXConsole* s_Console;
	};
}
