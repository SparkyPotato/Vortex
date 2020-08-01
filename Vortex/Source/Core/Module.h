#pragma once

namespace Vortex
{
	/*
		Base class of all Vortex Modules. 
	*/
	class Module
	{
	public:
		/* 
			Virtual destructor so we can have a list of Modules and make sure they're deleted properly.
		*/
		virtual ~Module() {}

		/*
			Startup the module instance.
		*/
		virtual void Startup() = 0;
		/*
			Shutdown the module instance.
		*/
		virtual void Shutdown() = 0;
		/*
			Tick the module instance.
		*/
		virtual void Tick(float deltaTime) = 0;

		/*
			Requests the module quit after finishing up whatever it is doing.
		*/
		virtual void Quit() = 0;
	};
}
