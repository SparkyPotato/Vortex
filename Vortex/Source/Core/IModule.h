#pragma once

namespace Vortex
{
	class IModule
	{
	public:
		virtual ~IModule() {}

		/*
			Startup the module instance.
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
		virtual int RunTickLoop() = 0;

		/*
			Requests the module quit after finishing up whatever it is doing.
		*/
		virtual void Quit() = 0;
		/*
			Lets others stop the module from quitting.
		*/
		virtual void BlockQuit() = 0;
		virtual void AllowQuit() = 0;
	};
}
