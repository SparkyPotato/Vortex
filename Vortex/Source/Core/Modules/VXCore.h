#pragma once
#include <Core/Module.h>
#include <Core/Modules/VXInput.h>
#include <Core/Modules/VXGui.h>
#include <Core/Application.h>
#include <Core/Layers/LayerStack.h>

namespace Vortex
{
	/*
		The Core Vortex Module. It starts all other Modules and deals with Window Management.
	*/
	class VXCore : public Module
	{
	public:
		VXCore();
		~VXCore();
		/*
			Starts the Vortex Core Module.
			Not putting this in a constructor means that we don't have to recreate it when running the application
			in the editor.
		*/
		virtual void Startup() override;
		/*
			Shuts down the Vortex Core Module.
			Not putting this in a destructor means that we don't have to destroy the Module to reset the application
		*/
		virtual void Shutdown() override;
		/*
			Runs a single Tick of the Vortex Core Module.
		*/
		virtual void Tick(float deltaTime) override;
		/*
			Runs the Tick Loop of the Vortex Core Module.
		*/
		void RunTickLoop();

		/*
			Requests the Vortex Core Module to quit. This will also cause the application to quit.
		*/
		virtual void Quit() override;
		/*
			Makes sure the application won't quit until AllowQuit() is called.
		*/
		virtual void BlockQuit() override;
		/*
			Allows the application to quit after a BlockQuit() call.
		*/
		virtual void AllowQuit() override;

		/*
			Returns the last frame delta.
		*/
		float GetLastDeltaTime() { return m_DeltaTime; }

		/*
			Returns the application owned by the Vortex Core. You shouldn't be needing this.
		*/
		Application* GetApplication() { return m_App; }

		/*
			Returns the Vortex Input Module.
		*/
		VXInput* GetInput() { return m_Input; }

		/*
			Returns the layer stack.
		*/
		LayerStack* GetLayerStack() { return m_LayerStack; }

		/*
			Returns the window.
		*/
		Window* GetWindow() { return m_Window; }

	private:
		/*
			Called back on every window event. 
			Passes on event to respective modules and then to the application.
		*/
		void OnWindowEvent(Window* window, Event& event);

		// The Vortex Input Module.
		VXInput* m_Input = nullptr;
		// The Vortex GUI Module.
		VXGui* m_Gui = nullptr;

		// A pointer to the user-defined application.
		Application* m_App = nullptr;
		// A pointer to the application layer stack.
		LayerStack* m_LayerStack = nullptr;
		// A pointer to the main application window.
		Window* m_Window = nullptr;

		// Is the application running?
		bool m_IsTicking = false;
		bool m_CanQuit = true;
		bool m_WantsQuit = false;
		std::mutex m_QuitMutex;

		// The last frame delta.
		float m_DeltaTime = 0.f;
		LARGE_INTEGER m_Frequency;
		LARGE_INTEGER m_LastTime, m_CurrentTime;
	};
}
