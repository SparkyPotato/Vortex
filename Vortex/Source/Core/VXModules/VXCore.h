#pragma once
#include <Core/IModule.h>

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
		*/
		virtual int Startup() override;
		/*
			Shuts down the Vortex Core Module.
		*/
		virtual int Shutdown() override;
		/*
			Runs a single Tick of the Vortex Core Module.
		*/
		virtual void Tick(float deltaTime) override;
		/*
			Runs the Tick Loop of the Vortex Core Module.
		*/
		virtual void RunTickLoop() override;

		/*
			Requests the Vortex Core Module to quit. This will also cause the application to quit.
		*/
		virtual void Quit() override;

	private:
		bool m_IsTicking;
		std::mutex m_RunMutex;

		float m_DeltaTime;
		LARGE_INTEGER m_Frequency;
		LARGE_INTEGER m_LastTime, m_CurrentTime;
	};
}
