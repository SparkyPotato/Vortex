#include <VXpch.h>

/*
	This is the Entry Point of the Engine if we are on Windows.
	Only compiled if we are on Windows.
*/
#ifdef PLATFORM_WINDOWS
#include <Core/Modules/VXCore.h>
#include <Private/Platforms/Windows/WWindow.h>

/*
	If building as a Console App.
*/
int wmain(int argc, wchar_t** argv)
{
	using namespace Vortex;

	// Initialize the Logger, so we can start logging without depending on VXCore.
	try { Logger::Init(); }
	catch (...)
	{
		// Outputting to Visual Studio debugger because the logger creation failed.
		OutputDebugStringA("Failed to initialize Logger!");
		OutputDebugStringA("Sleeping for 10 seconds before exiting...");
		Sleep(10000);
		return -1;
	}
	
	// Register Window Class.
	try { WWindow::RegisterWindowClass(); }
	catch (std::exception& e)
	{
		ENG_ERROR("Failed to initialize Window Class!");
		ENG_ERROR("Error: {0}", e.what());
		ENG_ERROR("Sleeping for 10 seconds before exiting...");
		Sleep(10000);
		return -1;
	}

	VXCore* core = new VXCore();
	// Start the Vortex Core Module.
	try { core->Startup(); }
	catch (std::exception& e)
	{
		ENG_ERROR("Failed to start Vortex Core!");
		ENG_ERROR("Error: {0}", e.what());

		// If we have created the Graphics Context, make sure to clean everything up before exiting.
		if (IGraphicsContext::Get())
		{
			IGraphicsContext::Destroy();
		}

		ENG_ERROR("Sleeping for 10 seconds before exiting...");
		Sleep(10000);
		return -1;
	}

	// Run the blocking Tick Loop of the Vortex Core Module. App execution ends when out of this loop.
	try { core->RunTickLoop(); }
	catch (std::exception& e)
	{
		ENG_ERROR("Error while running Vortex Core Module Tick!");
		ENG_ERROR("Error: {0}", e.what());

		// If we haven't deleted the Graphics Context, make sure to clean everything up before exiting.
		if (IGraphicsContext::Get())
		{
			IGraphicsContext::Destroy();
		}

		ENG_ERROR("Sleeping for 10 seconds before exiting...");
		Sleep(10000);
		return -1;
	}

	// Shutdown the Vortex Core Module, and return the error code passed.
	try { core->Shutdown(); }
	catch (std::exception& e)
	{
		ENG_ERROR("Failed to shutdown Vortex Core!");
		ENG_ERROR("Error: {0}", e.what());

		// If we haven't deleted the Graphics Context, make sure to clean everything up before exiting.
		if (IGraphicsContext::Get())
		{
			IGraphicsContext::Destroy();
		}

		ENG_ERROR("Sleeping for 10 seconds before exiting...");
		Sleep(10000);
		return -1;
	}

	delete core;
	ENG_TRACE("Exiting...");
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
