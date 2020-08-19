#include <VXpch.h>

/*
	This is the Entry Point of the Engine if we are on Windows.
	Only compiled if we are on Windows.
*/
#ifdef PLATFORM_WINDOWS
#include <Core/Modules/VXCore.h>
#include <Private/Platforms/Windows/WWindow.h>

DECLARE_LOGGER(LogEntry);
DEFINE_LOGGER(LogEntry);

/*
	If building as a Console App.
*/
int wmain(int argc, wchar_t** argv)
{
	using namespace Vortex;

	CREATE_LOGGER(LogEntry, spdlog::level::trace);
	CREATE_LOGGER(LogWindow, spdlog::level::trace);

	// Initialize the Logger, so we can start logging without depending on VXCore.
	try { Logger::Init(); }
	catch (...)
	{
		// Outputting to Visual Studio debugger because the logger creation failed.
		OutputDebugStringA("Failed to initialize Logger!");
		return -1;
	}

	Profiler::Init();
	
	// Register Window Class.
	try { WWindow::RegisterWindowClass(); }
	catch (std::exception& e)
	{
		VX_ERROR(LogEntry, "Failed to initialize Window Class!");
		VX_ERROR(LogEntry, "Error: {0}", e.what());
		return -1;
	}

	VXCore* core = new VXCore();
	// Start the Vortex Core Module.
	try { core->Startup(); }
	catch (std::exception& e)
	{
		VX_ERROR(LogEntry, "Failed to start Vortex Core!");
		VX_ERROR(LogEntry, "Error: {0}", e.what());

		// If we have created the Graphics Context, make sure to clean everything up before exiting.
		if (GraphicsContext::Get())
		{
			GraphicsContext::Destroy();
		}

		return -1;
	}

	// Run the blocking Tick Loop of the Vortex Core Module. App execution ends when out of this loop.
	try { core->RunTickLoop(); }
	catch (std::exception& e)
	{
		VX_ERROR(LogEntry, "Error while running Vortex Core Module Tick!");
		VX_ERROR(LogEntry, "Error: {0}", e.what());

		// If we haven't deleted the Graphics Context, make sure to clean everything up before exiting.
		if (GraphicsContext::Get())
		{
			GraphicsContext::Destroy();
		}

		return -1;
	}

	// Shutdown the Vortex Core Module, and return the error code passed.
	try { core->Shutdown(); }
	catch (std::exception& e)
	{
		VX_ERROR(LogEntry, "Failed to shutdown Vortex Core!");
		VX_ERROR(LogEntry, "Error: {0}", e.what());

		// If we haven't deleted the Graphics Context, make sure to clean everything up before exiting.
		if (GraphicsContext::Get())
		{
			GraphicsContext::Destroy();
		}

		return -1;
	}

	Profiler::Shutdown();

	delete core;
	VX_TRACE(LogEntry, "Exiting...");
	return 0;
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
