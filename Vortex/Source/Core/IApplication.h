#pragma once
#include <Core/IWindow.h>

namespace Vortex
{
	class VXCore;

	/*
		CreateApplication function prototype:-
		Vortex::IApplication* CreateApplication()
		{
			return new YourDerivedClass();
		}

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
			Gets the window properties desired by the application so that the window can be created.
		*/
		virtual const IWindow::Properties& GetWindowProperties() = 0;

		/*
			Binds the Modules to the Application, so you can use GCore to get a pointer to the Vortex Core Module.
		*/
		void BindToModule(VXCore* core) { GCore = core; }

		/*
			Called on application Startup.
		*/
		virtual void Start() = 0;
		/*
			Called every frame, with the frame delta of the previous frame (0 for the first frame).
		*/
		virtual void Tick(float deltaTime) = 0;

	protected:
		// Pointer to the Vortex Core Module that owns the application.
		VXCore* GCore;
	};
}

extern Vortex::IApplication* CreateApplication();
