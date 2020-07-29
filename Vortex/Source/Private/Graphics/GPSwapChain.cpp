#include <VXpch.h>
#include <Graphics/Primitives/GPSwapChain.h>
#include <Graphics/GraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11SwapChain.h>
#endif

namespace Vortex
{
	GPSwapChain* GPSwapChain::Create(Window* window)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11SwapChain(window);
		default:
			return nullptr;
		}
	}
}
