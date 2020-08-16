#include <VXpch.h>
#include <Core/Modules/VXCore.h>
#include <Core/Events/WindowEvent.h>
#include <Graphics/GraphicsContext.h>
#include <Core/VXConsole.h>

Vortex::VXCore* GCore;

namespace Vortex
{
	CREATE_LOGGER_LOCAL(LogCore, spdlog::level::trace);

	DEFINE_LOGGER(LogWindow, spdlog::level::trace);

	void VXCore::Startup()
	{
		VX_TRACE(LogCore, "Starting Vortex Core Module.");

		// Make sure we can't start the engine twice.
		if (m_IsTicking)
			throw std::exception("Vortex Core already started.");

		// Start the tick, and does frame delta calculation setup.
		m_IsTicking = true;
		QueryPerformanceFrequency(&m_Frequency);
		m_DeltaTime = 0;

		// Create the Vortex Input Module.
		m_Input = new VXInput();
		m_Input->Startup();

		// Creates the layer stack.
		m_LayerStack = new LayerStack();
		GLayerStack = m_LayerStack;

		// Creates the application and binds all the required Modules.
		m_App = CreateApplication();
		VX_TRACE(LogCore, "Created Client application.");

		// Create the Graphics Context.
		GraphicsContext::Create(GraphicsAPI::DirectX11);

		// Creates the window, using the application-defined properties.
		m_Window = Window::Create(m_App->GetWindowProperties());
		m_Window->SetEventCallback(std::bind(&VXCore::OnWindowEvent, this, std::placeholders::_1, std::placeholders::_2));
		GWindow = m_Window;

		m_Gui = new VXGui();
		m_Gui->Startup();

		m_Renderer = new VXRenderer();
		m_Renderer->Startup();

		::GCore = this;

		// Starts the user-defined application.
		m_App->Start();
		VX_TRACE(LogCore, "Started Client application.");

		VX_TRACE(LogCore, "Started Vortex Core Module.");
	}

	void VXCore::Shutdown()
	{
		VX_TRACE(LogCore, "Shutting down Vortex Core Module.");

		m_IsTicking = false;

		// Deletes the application, so the user doesn't have to worry about it.
		delete m_App;
		m_App = nullptr;

		m_Renderer->Shutdown();
		delete m_Renderer;

		// Deletes the Vortex GUI Module.
		m_Gui->Shutdown();
		delete m_Gui;

		// Destroys the layer stack.
		delete m_LayerStack;

		// Destroys the window.
		delete m_Window;

		// Destroys Graphics Context.
		GraphicsContext::Destroy();

		// Deletes the Vortex Input Module.
		m_Input->Shutdown();
		delete m_Input;

		::GCore = nullptr;

		VX_TRACE(LogCore, "Shut down Vortex Core Module.");
	}

	void VXCore::Tick(float deltaTime)
	{
		// Ticks the Vortex Input module.
		m_Input->Tick(deltaTime);

		// Updates the application window, getting all window events.
		m_Window->Update();

		ENG_PROFILESTART("Application Tick");
		// Calls the application tick.
		m_App->Tick(deltaTime);
		ENG_PROFILEEND();

		// Calls the layer stack tick.
		m_LayerStack->Tick(deltaTime);

		m_Renderer->Tick(deltaTime);

		m_Gui->Tick(deltaTime);
	}

	void VXCore::RunTickLoop()
	{
		VX_TRACE(LogCore, "Starting Vortex Core Module Tick.");

		if (!m_IsTicking)
			throw std::exception("Module has not been started!");

		while (m_IsTicking)
		{
			// Quit application if required.
			if (m_WantsQuit && m_CanQuit)
				Quit();

			if (m_ShouldRestart)
			{
				Quit();
				Shutdown();
				Startup();
				m_ShouldRestart = false;
			}

			// Get time before frame.
			QueryPerformanceCounter(&m_LastTime);

			Tick(m_DeltaTime);

			// Measure frame delta.
			QueryPerformanceCounter(&m_CurrentTime);
			m_DeltaTime = (float) (m_CurrentTime.QuadPart - m_LastTime.QuadPart);
			m_DeltaTime /= m_Frequency.QuadPart;
		}

		VX_TRACE(LogCore, "Ended Vortex Core Module Tick.");
	}

	void VXCore::Quit()
	{
		// Locks mutex so two threads cannot concurrently quit the application.
		m_QuitMutex.lock();

		// Makes sure no one else is keeping the Module running.
		if (m_CanQuit)
		{
			m_Gui->Quit();
			m_Input->Quit();
			m_Renderer->Quit();
			m_App->Quit();

			m_IsTicking = false;
		}
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

	void VXCore::OnConsoleCommand(ConsoleCommand command)
	{
		if (command.command == "restart")
		{
			m_ShouldRestart = true;
		}
		else if (command.command == "quit")
		{
			Quit();
		}
	}

	void VXCore::OnWindowEvent(Window* window, Event& event)
	{
		ENG_PROFILE("Event Dispatching");
		auto dispatcher = EventDispatcher(event);

		// Dispatches all Input related events to the Vortex Input Module.
		dispatcher.Dispatch<KeyDownEvent>(std::bind(&VXInput::KDEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<KeyUpEvent>(std::bind(&VXInput::KUEvent, m_Input, std::placeholders::_1));

		dispatcher.Dispatch<MouseMoveEvent>(std::bind(&VXInput::MMEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonDownEvent>(std::bind(&VXInput::MDEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonUpEvent>(std::bind(&VXInput::MUEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonDoubleClickEvent>(std::bind(&VXInput::MDCEvent, m_Input, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrollEvent>(std::bind(&VXInput::MSEvent, m_Input, std::placeholders::_1));

		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&VXRenderer::ResizeOnWindow, m_Renderer, std::placeholders::_1));

		if (event.GetType() == EventType::WindowDeactivate)
		{
			GInput->ClearInputState();
		}

		m_LayerStack->PassEvent(event);

		// Passes the event to the app if it hasn't been handled already.
		if (!event.IsHandled() && m_App)
			m_App->OnEvent(event);

		// Quit the application if the window was closed. Doing it after everything so they have a chance to block the quit.
		if (event.GetType() == EventType::WindowClose)
			Quit();
	}

	VXCore::VXCore()
	{
		/*
			We are replacing constructors and destructors with Startup() and Shutdown() methods 
			so that we don 't have to keep reallocating memory for the Module if it is being started and stopped many times.
		*/
	}

	VXCore::~VXCore()
	{
		
	}
}
