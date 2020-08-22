#include <VXpch.h>
#include <Core/Modules/VXCore.h>
#include <Core/Events/WindowEvent.h>
#include <Graphics/GraphicsContext.h>
#include <Core/VXConsole.h>

Vortex::VXCore* GCore;

DEFINE_LOGGER(LogCore);
DEFINE_LOGGER(LogWindow);

namespace Vortex
{
	void VXCore::Startup()
	{
		CREATE_LOGGER(LogCore, spdlog::level::trace);

		VX_TRACE(LogCore, "Starting Vortex Core Module.");

		VXConsole::Init();

		// Make sure we can't start the engine twice.
		if (m_IsTicking)
			throw std::exception("Vortex Core already started.");

		// Start the tick, and does frame delta calculation setup.
		m_IsTicking = true;
		QueryPerformanceFrequency(&m_Frequency);
		m_RenderDeltaTime = m_TickDeltaTime = 0;

		// Create the Vortex Input Module.
		m_Input = new VXInput();
		m_Input->Startup();

		// Creates the layer stack.
		m_LayerStack = new LayerStack();

		// Creates the application and binds all the required Modules.
		m_App = CreateApplication();
		VX_TRACE(LogCore, "Created Client application.");

		// Create the Graphics Context.
		GraphicsContext::Create(GraphicsAPI::DirectX11);

		// Creates the window, using the application-defined properties.
		m_Window = Window::Create(m_App->GetWindowProperties());
		m_Window->SetEventCallback([&](Window* window, Event& event) { this->OnWindowEvent(window, event); });
		GWindow = m_Window;

		m_Gui = new VXGui();
		m_Gui->Startup();

		m_Renderer = new VXRenderer();
		m_Renderer->Startup();

		m_ScriptManager = new VXScriptManager();
		m_ScriptManager->Startup();

		::GInput = m_Input;
		::GLayerStack = m_LayerStack;
		::GRenderer = m_Renderer;
		::GScriptManager = m_ScriptManager;
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
		p_RenderThread->join();

		// Destroys the layer stack.
		delete m_LayerStack;
		GLayerStack = m_LayerStack = nullptr;

		// Deletes the application, so the user doesn't have to worry about it.
		delete m_App;
		m_App = nullptr;

		m_ScriptManager->Shutdown();
		delete m_ScriptManager;
		GScriptManager = m_ScriptManager = nullptr;

		m_Renderer->Shutdown();
		delete m_Renderer;
		GRenderer = m_Renderer = nullptr;

		// Deletes the Vortex GUI Module.
		m_Gui->Shutdown();
		delete m_Gui;
		m_Gui = nullptr;

		// Destroys the window.
		delete m_Window;
		GWindow = m_Window = nullptr;

		// Destroys Graphics Context.
		GraphicsContext::Destroy();

		// Deletes the Vortex Input Module.
		m_Input->Shutdown();
		delete m_Input;
		GInput = m_Input = nullptr;

		::GCore = nullptr;

		VX_TRACE(LogCore, "Shut down Vortex Core Module.");
	}

	void VXCore::Tick(float deltaTime)
	{
		// Ticks the Vortex Input module.
		m_Input->Tick(deltaTime);

		// Updates the application window, getting all window events.
		m_Window->GetEvents();

		ENG_PROFILESTART("Application Tick");
		// Calls the application tick.
		m_App->Tick(deltaTime);
		ENG_PROFILEEND();

		// Calls the layer stack tick.
		m_LayerStack->Tick(deltaTime);

		m_ScriptManager->Tick(deltaTime);
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
			m_ScriptManager->Quit();
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
		std::string delimiter = ".";
		std::string variable = command.command.substr(0, command.command.find(delimiter));
		command.command.erase(0, command.command.find(delimiter) + delimiter.length());

		if (variable == "restart")
		{
			VX_INFO(LogConsole, "Core: Restarting...");
			m_ShouldRestart = true;
		}
		else if (variable == "quit")
		{
			VX_INFO(LogConsole, "Core: Quitting...");
			Quit();
		}
		else if (variable == "help")
		{
			VX_INFO(LogConsole, "Core: Functions: ");
			VX_INFO(LogConsole, "Core:     quit - Quits the Vortex Engine.");
			VX_INFO(LogConsole, "Core:     restart - Restarts the Vortex Engine.");
		}
		else if (variable == "window")
		{
			m_Window->OnConsoleCommand({ command.command });
		}
		else
		{
			VX_ERROR(LogConsole, "'{0}' is not a valid command for module core!", variable);
		}
	}

	void VXCore::OnWindowEvent(Window* window, Event& event)
	{
		if (!m_IsTicking) return;

		ENG_PROFILE("Event Dispatching");
		auto dispatcher = EventDispatcher(event);

		// Dispatches all Input related events to the Vortex Input Module.
		dispatcher.Dispatch<KeyDownEvent>(BIND_EVENT(m_Input->KDEvent));
		dispatcher.Dispatch<KeyUpEvent>(BIND_EVENT(m_Input->KUEvent));

		dispatcher.Dispatch<MouseMoveEvent>(BIND_EVENT(m_Input->MMEvent));
		dispatcher.Dispatch<MouseButtonDownEvent>(BIND_EVENT(m_Input->MDEvent));
		dispatcher.Dispatch<MouseButtonUpEvent>(BIND_EVENT(m_Input->MUEvent));
		dispatcher.Dispatch<MouseButtonDoubleClickEvent>(BIND_EVENT(m_Input->MDCEvent));
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(m_Input->MSEvent));
		dispatcher.Dispatch<MouseRawEvent>(BIND_EVENT(m_Input->RMEvent));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(m_Renderer->ResizeOnWindow));

		if (event.GetType() == EventType::WindowDeactivate)
		{
			m_Input->ClearInputState();
		}

		m_LayerStack->PassEvent(event);

		// Passes the event to the app if it hasn't been handled already.
		if (!event.IsHandled() && m_App)
			m_App->OnEvent(event);

		// Quit the application if the window was closed. Doing it after everything so they have a chance to block the quit.
		if (event.GetType() == EventType::WindowClose)
			Quit();
	}

	void VXCore::RunTickLoop()
	{
		VX_TRACE(LogCore, "Starting Vortex Core Module Tick.");

		if (!m_IsTicking)
			throw std::exception("Module has not been started!");

		p_RenderThread = new std::thread(&VXCore::RunRenderLoop, this);

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
			QueryPerformanceCounter(&m_TickLastTime);

			if (m_MainThreadFrameCount - 5 > m_RenderThreadFrameCount)
			{
				if (m_RenderedGui)
				{
					QueryPerformanceCounter(&m_TickCurrentTime);
					m_TickDeltaTime = (float)(m_TickCurrentTime.QuadPart - m_TickLastTime.QuadPart);
					m_TickDeltaTime /= m_Frequency.QuadPart;

					continue;
				}
				m_Gui->Tick(m_TickDeltaTime);
				m_RenderedGui = true;

				QueryPerformanceCounter(&m_TickCurrentTime);
				m_TickDeltaTime = (float)(m_TickCurrentTime.QuadPart - m_TickLastTime.QuadPart);
				m_TickDeltaTime /= m_Frequency.QuadPart;

				m_MainThreadFrameCount++;
				continue;
			}
			else
			{
				Tick(m_TickDeltaTime);

				if (m_RenderedGui)
				{
					QueryPerformanceCounter(&m_TickCurrentTime);
					m_TickDeltaTime = (float)(m_TickCurrentTime.QuadPart - m_TickLastTime.QuadPart);
					m_TickDeltaTime /= m_Frequency.QuadPart;

					continue;
				}
				m_Gui->Tick(m_TickDeltaTime);
				m_RenderedGui = true;

				QueryPerformanceCounter(&m_TickCurrentTime);
				m_TickDeltaTime = (float)(m_TickCurrentTime.QuadPart - m_TickLastTime.QuadPart);
				m_TickDeltaTime /= m_Frequency.QuadPart;

				m_MainThreadFrameCount++;
			}
		}

		VX_TRACE(LogCore, "Ended Vortex Core Module Tick.");
	}

	void VXCore::RunRenderLoop()
	{
		VX_TRACE(LogCore, "Started Render Thread ({0}).", p_RenderThread->get_id());

		while (m_IsTicking)
		{
			// Get time before frame.
			QueryPerformanceCounter(&m_RenderLastTime);

			if (m_RenderThreadFrameCount > m_MainThreadFrameCount)
			{
				QueryPerformanceCounter(&m_RenderCurrentTime);
				m_RenderDeltaTime = (float)(m_RenderCurrentTime.QuadPart - m_RenderLastTime.QuadPart);
				m_RenderDeltaTime /= m_Frequency.QuadPart;

				continue;
			}

			m_Renderer->ResizeIfRequired();
			m_RenderedGui = false;

			m_Renderer->Tick(m_RenderDeltaTime);

			if (m_PostRenderFunction) m_PostRenderFunction(m_RenderDeltaTime);

			while (!m_RenderedGui) {}
			m_Gui->Draw();

			m_Window->GetSwapChain()->Swap(m_Window->GetProperties().syncInterval);

			QueryPerformanceCounter(&m_RenderCurrentTime);
			m_RenderDeltaTime = (float)(m_RenderCurrentTime.QuadPart - m_RenderLastTime.QuadPart);
			m_RenderDeltaTime /= m_Frequency.QuadPart;

			m_RenderThreadFrameCount++;
		}

		VX_TRACE(LogCore, "Ended Render Thread.");
	}

	VXCore::VXCore()
	{
		/*
			We are replacing constructors and destructors with Startup() and Shutdown() methods 
			so that we don 't have to keep reallocating memory for the Module if it is being started and stopped many times.
			This also helps in handling module restarting easily. We just wait for the tick to finish, and then call
			Shutdown() and Startup(). This way, a module can shut itself down and restart itself as well.
		*/
	}

	VXCore::~VXCore()
	{
		
	}
}
