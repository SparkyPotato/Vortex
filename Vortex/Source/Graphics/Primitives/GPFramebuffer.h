#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <Core/IWindow.h>

namespace Vortex
{
	class GPFramebuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPFramebuffer() {}

		virtual void Bind() = 0;
		virtual void Recreate() = 0;

		virtual void Resize() = 0;
		virtual void Resize(int width, int height) = 0;

		virtual void SetWindow(IWindow* window) = 0;

		virtual void Clear(float r, float g, float b, float a) = 0;

		static GPFramebuffer* Create(GPTexture* texture);
		static GPFramebuffer* Create(IWindow* window);
	};
}
