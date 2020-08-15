#include <VXpch.h>
#include <Core/VXConsole.h>
#include <Core/Modules/VXCore.h>

namespace Vortex
{
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

	bool VXConsole::SubmitCommand(std::string commandString)
	{
		std::string delimiter = ".";
		std::string destination = commandString.substr(0, commandString.find(delimiter));
		commandString.erase(0, commandString.find(delimiter) + delimiter.length());

		if (destination == "core")
		{
			return GCore->OnConsoleCommand({ commandString });
		}
		else if (destination == "gui")
		{
			return GCore->GetGui()->OnConsoleCommand({ commandString });
		}
		else if (destination == "input")
		{
			return GInput->OnConsoleCommand({ commandString });
		}
		else if (destination == "renderer")
		{
			return GRenderer->OnConsoleCommand({ commandString });
		}
		else return false;
	}
}
