#include <VXpch.h>

/*
	This is the Entry Point of the Engine if we are on Windows.
	This file is only compiled if we are on Windows.
*/
#ifdef PLATFORM_WINDOWS

/*
	If building as a Console App.
*/
int wmain(int argc, wchar_t** argv)
{
	using namespace Vortex;

	// Initialize the Logger, so we can start logging without depending on VXCore.
	Logger::Init();

	// Start the Vortex Core Module.
	VXCore* core = new VXCore();
	int errorCode = core->Startup();
	if (errorCode != 0)
	{
		ENG_ERROR("Failed to start Vortex Core Module! Error: {0}. Exiting...", errorCode);
		return errorCode;
	}

	// Run the blocking Tick Loop of the Vortex Core Module. App execution ends when out of this loop.
	core->RunTickLoop();

	// Shutdown the Vortex Core Module, and return the error code passed.
	errorCode = core->Shutdown();
	delete core;
	if (errorCode != 0)
	{
		ENG_ERROR("Failed to shut down Vortex Core Module! Error: {0}.", errorCode);
	}
	else
	{
		ENG_TRACE("Exiting...");
	}
	return errorCode;
}

/*
	If building as a Windowed App.
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// We just parse the Command Line arguments and call wmain().
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	return wmain(argc, argv);
}

#endif
