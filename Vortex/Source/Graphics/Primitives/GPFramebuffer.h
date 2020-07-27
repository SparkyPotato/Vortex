#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <Core/IWindow.h>

namespace Vortex
{
	/*
		A framebuffer which contains the Render Target and Depth Stencil.
		Can be createed on a texture or a window.
	*/
	class GPFramebuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPFramebuffer() {}

		/*
			Bind the framebuffer to the pipeline.
		*/
		virtual void Bind() = 0;
		/*
			Recreates the framebuffer.
		*/
		virtual void Recreate() = 0;

		/*
			If the framebuffer was created on a window, resize it to the window dimensions.
			This is automatically called in IWindow.
			If the framebuffer was created on a texutre, please use Resize(int width, int height) instead.
			This is because you need to specify dimensions to resize to.
		*/
		virtual void Resize() = 0;
		/*
			If the framebuffer was created on a texture, resize it to the given dimensions.
			If the framebuffer was created on a window, please use Resize() instead.
			This is because you cannot force the window resolution from the framebuffer. 
			If you want to set the window size, call IWindow::SetSize(width, height) and then call Resize().
		*/
		virtual void Resize(int width, int height) = 0;

		/*
			Sets the new window for the framebuffer if it was created from a window.
			Deletes the texture if the framebuffer was created from a texture.
		*/
		virtual void SetWindow(IWindow* window) = 0;

		/*
			Clears the Render Target with the specified color.
			Clears the Depth Stencil to 1.f (infinity).
		*/
		virtual void Clear(float r, float g, float b, float a) = 0;

		/*
			Creates a framebuffer from a texture.
			The framebuffer now owns the texture, so it is deleted when the framebuffer is deleted.
		*/
		static GPFramebuffer* Create(GPTexture* texture);
		/*
			Creates a framebuffer from a window.
			The framebuffer does not own the window, but instead works together with it for resizing.
		*/
		static GPFramebuffer* Create(IWindow* window);
	};
}
