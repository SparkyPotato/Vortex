#pragma once
#include <Core/Window.h>
#include <Core/VXConsole.h>
#include <imgui.h>

namespace Vortex
{
	class VXCore;
	class VXInput;
	class LayerStack;
	class Window;

	/*
		CreateApplication function prototype:-
		Vortex::Application* CreateApplication() { return new YourDerivedClass; }

		Deletion of the application is handled by the Vortex Core Module.
	*/

	/*
		Defines the application and its functionality.
		Create a derived class, and a CreateApplication function.
	*/
	class Application
	{
	public:
		virtual ~Application() {}
		/*
			Gets the properties for the main window to be created.
		*/
		virtual Window::Properties GetWindowProperties() { return Window::Properties(); }

		/*
			Called on application startup.
		*/
		virtual void Start() = 0;

		/*
			Called on application quit.
		*/
		virtual void Quit() = 0;

		/*
			Called every frame, with the frame delta of the previous frame (0 for the first frame).
		*/
		virtual void Tick(float deltaTime) = 0;

		/*
			Called every frame when ImGui is being rendered. ImGui functions will only work here. Doesn't have to be overridden.
		*/
		virtual void OnGuiRender() {}

		/*
			Called with all unhandled events in the Vortex Core. Doesn't get any event handled by layers. Doesn't have to be overridden.
		*/
		virtual void OnEvent(Event& event) {};

		/*
			Called to run application-custom console commands.
		*/
		virtual void OnConsoleCommand(ConsoleCommand command)
		{
			VX_ERROR(LogConsole, "Application has no custom commands.");
		}
	};
}

/*
	Defined in the client to create the application.
*/
extern Vortex::Application* CreateApplication();
