#pragma once

namespace Vortex
{
	/*
		Base class of all Vortex Modules. 
	*/
	class IModule
	{
	public:
		// Virtual destructor so we can have a list of Modules and make sure they're deleted properly.
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
			Requests the module quit after finishing up whatever it is doing.
		*/
		virtual void Quit() = 0;
		/*
			Lets others stop and allow the module from quitting.
		*/
		virtual void BlockQuit() = 0;
		virtual void AllowQuit() = 0;
	};
}
