#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <Graphics/Primitives/GPTexture.h>

namespace Vortex
{
	class Window;

	/*
		A swap chain. Is created by every window, and swapped when IWindow::Update is called.
	*/
	class GPSwapChain : public GraphicsPrimitive
	{
	public:
		virtual ~GPSwapChain() {}

		/*
			Creates a swap chain on the specified window.
		*/
		static GPSwapChain* Create(Window* window);

		/*
			Binding a swap chain does nothing yet!
		*/
		virtual void Bind() = 0;
		/*
			Recreates the swap chain.
		*/
		virtual void Recreate() = 0;

		/*
			Returns the back buffer of the swap chain so you can render onto it.
			DO NOT create a framebuffer from the back buffer. 
			Create the framebuffer from the window that owns the swap chain instead.
		*/
		virtual GPTexture* GetBackBuffer() = 0;

		/*
			Resizes the swap chain to the dimensions of the window.
		*/
		virtual void Resize() = 0;
		/*
			Swaps the buffers with the specified sync interval.
			Should be 0 for uncapped, and 1 to match the framerate of the monitor.
		*/
		virtual void Swap(int syncInterval) = 0;
	};
}
