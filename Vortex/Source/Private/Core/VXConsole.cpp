#include <VXpch.h>
#include <Core/VXConsole.h>
#include <Core/Modules/VXCore.h>

namespace Vortex
{
	DEFINE_LOGGER(LogConsole, spdlog::level::trace);

	VXConsole* VXConsole::s_Console = nullptr;

	VXConsole::VXConsole()
	{

	}

	VXConsole::~VXConsole()
	{

	}

	void VXConsole::Init()
	{
		s_Console = new VXConsole;
	}

	void VXConsole::Shutdown()
	{
		delete s_Console;
	}

	void VXConsole::SubmitCommand(std::string commandString)
	{
		std::string delimiter = ".";
		std::string module = commandString.substr(0, commandString.find(delimiter));
		commandString.erase(0, commandString.find(delimiter) + delimiter.length());

		if (module == "core" || module == "c")
		{
			GCore->OnConsoleCommand({ commandString });
		}
		else if (module == "gui" || module == "g")
		{
			GCore->GetGui()->OnConsoleCommand({ commandString });
		}
		else if (module == "input" || module == "i")
		{
			GInput->OnConsoleCommand({ commandString });
		}
		else if (module == "renderer" || module == "r")
		{
			GRenderer->OnConsoleCommand({ commandString });
		}
		else
		{
			VX_ERROR(LogConsole, "'{0}' is not a valid module!", module);
		}
	}
}
