#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <Graphics/Primitives/GPTexture.h>

namespace Vortex
{
	class IWindow;

	class GPSwapChain : public GraphicsPrimitive
	{
	public:
		virtual ~GPSwapChain() {}

		static GPSwapChain* Create(IWindow* window);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;

		virtual GPTexture* GetBackBuffer() = 0;

		virtual void Resize() = 0;
		virtual void Swap(int syncInterval) = 0;
	};
}
