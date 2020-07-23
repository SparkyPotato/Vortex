#pragma once
#include <Core/IModule.h>
#include <Core/IApplication.h>

namespace Vortex
{
	/*
		The Core Vortex Module. It starts all other Modules and deals with Window Management.
	*/
	class VXCore : public IModule
	{
	public:
		VXCore();
		~VXCore();
		/*
			Starts the Vortex Core Module.
			Not putting this in a constructor means that we don't have to recreate it when running the application
			in the editor.
		*/
		virtual int Startup() override;
		/*
			Shuts down the Vortex Core Module.
			Not putting this in a destructor means that we don't have to destroy the Module to reset the application
		*/
		virtual int Shutdown() override;
		/*
			Runs a single Tick of the Vortex Core Module.
		*/
		virtual void Tick(float deltaTime) override;
		/*
			Runs the Tick Loop of the Vortex Core Module.
		*/
		virtual int RunTickLoop() override;

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

	private:
		void OnWindowEvent(IWindow* window, IEvent& event);

		// A pointer to the user-defined application.
		IApplication* m_App;
		// A pointer to the main application window.
		IWindow* m_Window;

		// Is the application running?
		bool m_IsTicking = false;
		bool m_CanQuit = true;
		bool m_WantsQuit = false;
		std::mutex m_QuitMutex;

		// The last frame delta.
		float m_DeltaTime;
		LARGE_INTEGER m_Frequency;
		LARGE_INTEGER m_LastTime, m_CurrentTime;
	};
}
