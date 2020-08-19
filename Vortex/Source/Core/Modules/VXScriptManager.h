#pragma once
#include <Core/Module.h>

DECLARE_LOGGER(LogScriptManager);

namespace Vortex
{
	class VXScriptManager : public Module
	{
	public:
		VXScriptManager();
		virtual ~VXScriptManager();

		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Tick(float deltaTime) override;
		virtual void Quit() override;

		virtual void OnConsoleCommand(ConsoleCommand command) override;
	};
}

extern Vortex::VXScriptManager* GScriptManager;
