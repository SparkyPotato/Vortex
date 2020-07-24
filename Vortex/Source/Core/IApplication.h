#pragma once
#include <Core/IWindow.h>

namespace Vortex
{
	class VXCore;

	/*
		CreateApplication function prototype:-
		Vortex::IApplication* CreateApplication() { return new YourDerivedClass(); }

		Deletion of the application is handled by the Vortex Core Module.
	*/

	/*
		Defines the application and its functionality.
		Create a derived class, and a CreateApplication function.
	*/
	class IApplication
	{
	public:
		virtual ~IApplication() {}
		/*
			Binds the modules to the application, so you can use GCore to get a pointer to the Vortex Core Module.
		*/
		void BindToModule(VXCore* core) { GCore = core; }
		/*
			Gets the properties for the main window to be created.
		*/
		virtual IWindow::Properties GetWindowProperties() { return IWindow::Properties(); }

		/*
			Called on application startup.
		*/
		virtual void Start() = 0;
		/*
			Called every frame, with the frame delta of the previous frame (0 for the first frame).
		*/
		virtual void Tick(float deltaTime) = 0;

		/*
			Called with all unhandled events in the Vortex Core. Doesn't have to be overriden.
		*/
		virtual void OnEvent(IEvent& event) {};

	protected:
		// Pointer to the Vortex Core Module that owns the application.
		VXCore* GCore;
	};
}

/*
	Defined in the client to create the application.
*/
extern Vortex::IApplication* CreateApplication();
