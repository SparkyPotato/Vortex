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
			Properties(std::string windowName, int windowWidth, int windowHeight, int minimumWidth, int minimumHeight)
				: name(windowName), width(windowWidth), minWidth(minimumWidth), height(windowHeight), minHeight(minimumHeight)
			{}

			// The name of the window as shown in the title bar.
			std::string name = "Vortex Engine Window";

			// Current width and height of the window. Is updated whenever the window is resized.
			int width = 1280;
			int height = 720;
			// Minimum width and height of the window that is allowed.
			int minWidth = 800;
			int minHeight = 600;

			// Position of the window in screen-space. Is updated whenever the window is moved.
			int x = 100;
			int y = 100;

			// Does the window have focus.
			bool IsActive = false;

			bool IsMinimized = false;
			bool IsMaximized = false;
			bool IsFullscreen = false;

			// Is the window currently moving or being resized?
			bool IsMoving = false;
			bool IsResizing = false;
		};

		typedef std::function<void(IWindow*, IEvent&)> EventCallback;

		/*
			Sets the callback for window events.
			If none is set, the window will not send events, but the properties will still change and will be accessible.
		*/
		virtual void SetEventCallback(EventCallback callback) = 0;
		/*
			Presents the next frame and gets all window events.
		*/
		virtual void Update() = 0;

		/*
			Moves the window to the top and activates it.
		*/
		virtual void Activate() = 0;
		/*
			Sets the name of the window.
		*/
		virtual void SetName(std::string name) = 0;
		/*
			Sets the size of the window, without moving it.
		*/
		virtual void SetSize(int width, int height) = 0;
		/*
			Sets the position of the window in screen-space.
		*/
		virtual void SetPosition(int x, int y) = 0;

		/*
			Implemented for each platform implementation so that the correct subclass is instantiated.
		*/
		static IWindow* Create(const IWindow::Properties& properties);
	};
}
