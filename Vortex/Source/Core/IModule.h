#pragma once

namespace Vortex
{
	class IModule
	{
	public:
		virtual ~IModule() {}

		/*
			Startup the Module instance.
		*/
		virtual int Startup() = 0;
		/*
			Shutdown the module instance.
		*/
		virtual int Shutdown() = 0;
		/*
			Tick the module instance.
		*/
		virtual void Tick(float deltaTime) = 0;
		/*
			Run a blocking Tick Loop of the module instance.
		*/
		virtual void RunTickLoop() = 0;

		/*
			Requests the Module quit after finishing up whatever it is doing. Blocks execution until the Module has quit.
		*/
		virtual void Quit() = 0;
	};
}
