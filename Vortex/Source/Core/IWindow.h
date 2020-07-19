#pragma once
#include <string>

namespace Vortex
{
	/*
		Class that defines the basic Window Interface.
		Implementation details vary per platform.
	*/
	class IWindow
	{
	public:
		/*
			Describes the properties of a window.
			Is queried from the application by the Vortex Core Module
		*/
		struct Properties 
		{
			std::string name;
			int width;
			int height;
		};
	};

	/*
		Implemented for each platform implementation so that the correct subclass is instantiated.
	*/
	extern IWindow* InstantiateWindow(const IWindow::Properties& properties);
}
