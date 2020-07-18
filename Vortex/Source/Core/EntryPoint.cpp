#include <VXpch.h>
#include <Core/VXCore.h>

/*
	If we are on Windows, this is the Entry Point of the Engine.
*/
#ifdef PLATFORM_WINDOWS

#include <Windows.h>

int main(char** argv, int argc)
{
	using namespace Vortex;

	Logger::Init();
	VXCore::Startup();

	VXCore::GetCore()->RunCoreLoop();

	int errorLevel = VXCore::Shutdown();
	ENG_TRACE("Exiting...");
	return errorLevel;
}

#endif
