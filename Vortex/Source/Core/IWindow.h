#pragma once
#include <string>
#include <Core/IEvent.h>

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
		*/
		struct Properties 
		{
			Properties()
			{}
			Properties(std::string windowName, int windowWidth, int windowHeight)
				: name(windowName), width(windowWidth), height(windowHeight)
			{}

			std::string name = "Vortex Engine Window";
			int width = 1280;
			int height = 720;
			int x = 100;
			int y = 100;

			bool IsActive = false;

			bool IsMinimized = false;
			bool IsMaximized = false;
			bool IsFullscreen = false;
		};

		typedef std::function<void(IWindow*, IEvent&)> EventCallback;

		virtual void SetEventCallback(EventCallback callback) = 0;
		virtual void Update() = 0;

		/*
			Implemented for each platform implementation so that the correct subclass is instantiated.
		*/
		static IWindow* Create(const IWindow::Properties& properties);
	};
}
