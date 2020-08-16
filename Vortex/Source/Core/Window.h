#pragma once
#include <string>
#include <Core/Event.h>
#include <Graphics/Primitives/GPSwapChain.h>

DECLARE_LOGGER(LogWindow);

namespace Vortex
{
	class GPFramebuffer;

	/*
		Class that defines the basic window interface.
		Implementation details vary per platform.
	*/
	class Window
	{
	public:
		/*
			Describes the properties of a window.
		*/
		struct Properties 
		{
			Properties()
			{}
			Properties(std::string windowName, int windowWidth, int windowHeight, bool vSync = false)
				: name(windowName), width(windowWidth), height(windowHeight)
			{
				if (vSync)
					syncInterval = 1;
				else
					syncInterval = 0;
			}
			Properties(std::string windowName, int windowWidth, int windowHeight, int minimumWidth = 800, int minimumHeight = 600, bool vSync = false)
				: name(windowName), width(windowWidth), minWidth(minimumWidth), height(windowHeight), minHeight(minimumHeight)
			{
				if (vSync)
					syncInterval = 1;
				else
					syncInterval = 0;
			}

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

			// Does the window have focus?
			bool isActive = false;

			// State of the window.
			bool isMinimized = false;
			bool isMaximized = false;
			bool isFullscreen = false;

			// Is the window currently moving or being resized?
			bool isMoving = false;
			bool isResizing = false;
			
			// VSync interval. If VSync is on this should usually be set to 1.
			int syncInterval = 0;
		};

		virtual ~Window() {}

		/*
			Implemented for each platform implementation so that the correct subclass is instantiated.
		*/
		static Window* Create(const Window::Properties& properties);

		typedef std::function<void(Window*, Event&)> EventCallback;

		/*
			Sets the callback for window events.
			If none is set, the window will not send events, but the properties will still change and will be accessible.
		*/
		virtual void SetEventCallback(EventCallback callback) = 0;
		/*
			Presents the next frame and gets all window events.
		*/
		virtual void Update() = 0;

		virtual const Properties& GetProperties() = 0;

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
			Returns the swap chain owned by the window.
		*/
		virtual GPSwapChain* GetSwapChain() = 0;
		/*
			Gets the framebuffer of the window.
		*/
		virtual GPFramebuffer* GetFramebuffer() = 0;
	};
}

extern Vortex::Window* GWindow;
