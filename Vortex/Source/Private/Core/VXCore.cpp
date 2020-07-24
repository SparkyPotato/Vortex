#include <VXpch.h>
#include <Core/Modules/VXCore.h>
#include <Core/Events/WindowEvent.h>

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

		m_Input = new VXInput();
		m_Input->Startup();

		// Creates the application and binds all the required Modules.
		m_App = CreateApplication();
		m_App->BindToModule(this);
		ENG_TRACE("Created Client application.");

		// Creates the window, using the application-defined properties.
		m_Window = IWindow::Create(m_App->GetWindowProperties());
		m_Window->SetEventCallback(std::bind(&VXCore::OnWindowEvent, this, std::placeholders::_1, std::placeholders::_2));

		// Starts the user-defined application.
		m_App->Start();
		ENG_TRACE("Started Client application.");

		ENG_TRACE("Started Vortex Core Module.");
		return 0;
	}

	int VXCore::Shutdown()
	{
		ENG_TRACE("Shutting down Vortex Core Module.");

		m_Input->Shutdown();
		delete m_Input;

		// Deletes the application, so the user doesn't have to worry about it.
		delete m_App;

		// Destroys the window.
		delete m_Window;

		ENG_TRACE("Shut down Vortex Core Module.");
		return 0;
	}

	void VXCore::Tick(float deltaTime)
	{
		m_Input->Tick(deltaTime);

		// Updates the application window, getting all window events.
		m_Window->Update();

		// Calls the application tick.
		m_App->Tick(deltaTime);
	}

	int VXCore::RunTickLoop()
	{
		ENG_TRACE("Starting Vortex Core Module Tick.");
		if (!m_IsTicking)
		{
			ENG_ERROR("Module has not been started!");
			return -1;
		}
		while (m_IsTicking)
		{
			if (m_WantsQuit && m_CanQuit)
				Quit();

			QueryPerformanceCounter(&m_LastTime);

			Tick(m_DeltaTime);

			// Measure frame delta.
			QueryPerformanceCounter(&m_CurrentTime);
			m_DeltaTime = (float) (m_CurrentTime.QuadPart - m_LastTime.QuadPart);
			m_DeltaTime /= m_Frequency.QuadPart;
		}

		return 0;
		ENG_TRACE("Ended Vortex Core Module Tick.");
	}

	void VXCore::Quit()
	{
		// Locks mutex so two threads cannot concurrently quit the application.
		m_QuitMutex.lock();

		// Makes sure no one else is keeping the Module running.
		if (m_CanQuit) m_IsTicking = false;
		else m_WantsQuit = true;

		m_QuitMutex.unlock();
	}

	void VXCore::BlockQuit()
	{
		// Locks mutex so two threads don't concurrently block and allow quitting.
		m_QuitMutex.lock();

		m_CanQuit = false;
		m_IsTicking = true;

		m_QuitMutex.unlock();
	}

	void VXCore::AllowQuit()
	{
		// Locks mutex so two threads don't concurrently block and allow quitting.
		m_QuitMutex.lock();

		m_CanQuit = true;

		m_QuitMutex.unlock();
	}

	void VXCore::OnWindowEvent(IWindow* window, IEvent& event)
	{
		EventDispatcher dispatcher = EventDispatcher(event);

		if (event.GetType() == EventType::WindowClose)
			Quit();

		dispatcher.Dispatch<KeyDownEvent>(std::bind(&VXInput::KDEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<KeyUpEvent>(std::bind(&VXInput::KUEvent, m_Input, std::placeholders::_1));

		dispatcher.Dispatch<MouseMoveEvent>(std::bind(&VXInput::MMEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonDownEvent>(std::bind(&VXInput::MDEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonUpEvent>(std::bind(&VXInput::MUEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonDoubleClickEvent>(std::bind(&VXInput::MDCEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrollEvent>(std::bind(&VXInput::MSEvent, m_Input, std::placeholders::_1));

		if (!event.IsHandled())
			m_App->OnEvent(event);
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
