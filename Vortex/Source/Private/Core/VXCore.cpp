#include <VXpch.h>
#include <Core/Modules/VXCore.h>

namespace Vortex
{
	int VXCore::Startup()
	{
		ENG_TRACE("Starting Vortex Core Module.");

		// Make sure we can't start the engine twice.
		if (m_IsTicking)
		{
			ENG_ERROR("Vortex Core Module is already started!");
			return -1;
		}

		// Start the tick, and does frame delta calculation setup.
		m_IsTicking = true;
		QueryPerformanceFrequency(&m_Frequency);
		m_DeltaTime = 0;

		// Creates the application and binds all the required Modules.
		m_App = CreateApplication();
		m_App->BindToModule(this);
		ENG_TRACE("Created Client application.");

		// Starts the user-defined application.
		m_App->Start();
		ENG_TRACE("Started Client application");

		// Creates the window according to the application's specification.
		auto properties = m_App->GetWindowProperties();
		m_Window = InstantiateWindow(properties);
		ENG_TRACE("Created window: \"{0}\" ({1}, {2})", properties.name, properties.width, properties.height);

		ENG_TRACE("Started Vortex Core Module.");
		return 0;
	}

	int VXCore::Shutdown()
	{
		ENG_TRACE("Shutting down Vortex Core Module.");

		// Deletes the application, so the user doesn't have to worry about it.
		delete m_App;

		// Destroys the window.
		delete m_Window;

		ENG_TRACE("Shut down Vortex Core Module.");
		return 0;
	}

	void VXCore::Tick(float deltaTime)
	{
		// Calls the application tick.
		m_App->Tick(deltaTime);
	}

	void VXCore::RunTickLoop()
	{
		ENG_TRACE("Starting Vortex Core Module Tick.");
		while (m_IsTicking)
		{
			QueryPerformanceCounter(&m_LastTime);

			Tick(m_DeltaTime);

			// Changes the frame delta value to the new one.
			QueryPerformanceCounter(&m_CurrentTime);
			m_DeltaTime = (float) (m_CurrentTime.QuadPart - m_LastTime.QuadPart);
			m_DeltaTime /= m_Frequency.QuadPart;
		}
		ENG_TRACE("Ended Vortex Core Module Tick.");
	}

	void VXCore::Quit()
	{
		// Makes sure no other threads are keeping the Module running.
		m_RunMutex.lock();
		m_IsTicking = false;
		m_RunMutex.unlock();
	}

	VXCore::VXCore()
	{
		// Makes sure that we aren't going to run the tick loop without calling VXCore::Startup.
		m_IsTicking = false;
	}

	VXCore::~VXCore()
	{

	}
}
