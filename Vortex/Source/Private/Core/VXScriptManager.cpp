#include <VXpch.h>
#include <Core/Modules/VXScriptManager.h>
#include <Core/VXConsole.h>

Vortex::VXScriptManager* GScriptManager;

DEFINE_LOGGER(LogScriptManager);

namespace Vortex
{

	VXScriptManager::VXScriptManager()
	{

	}

	VXScriptManager::~VXScriptManager()
	{

	}

	void VXScriptManager::Startup()
	{
		CREATE_LOGGER(LogScriptManager, spdlog::level::trace);
	}

	void VXScriptManager::Shutdown()
	{

	}

	void VXScriptManager::Tick(float deltaTime)
	{

	}

	void VXScriptManager::Quit()
	{

	}

	void VXScriptManager::OnConsoleCommand(ConsoleCommand command)
	{

	}
}
