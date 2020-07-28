#pragma once
#include <Core/IWindow.h>
#include <imgui.h>

namespace Vortex
{
	class VXCore;
	class VXInput;
	class LayerStack;
	class IWindow;

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
		void BindToModule(VXCore* core, VXInput* input, LayerStack* layerStack) 
		{ 
			GCore = core;
			GInput = input;
			GLayerStack = layerStack;
		}
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
			Called every frame when ImGui is being rendered. ImGui functions will only work here. Doesn't have to be overriden.
		*/
		virtual void OnGuiRender() {}

		/*
			Called with all unhandled events in the Vortex Core. Doesn't get any event handled by layers. Doesn't have to be overriden.
		*/
		virtual void OnEvent(IEvent& event) {};

	protected:
		// Pointer to the Vortex Core Module that owns the application.
		VXCore* GCore;
		// Pointer to Vortex Input Module.
		VXInput* GInput;
		// Pointer to the layer stack.
		LayerStack* GLayerStack;
	};
}

/*
	Defined in the client to create the application.
*/
extern Vortex::IApplication* CreateApplication();
