#include <VXpch.h>
#include <Graphics/Primitives/GPSwapChain.h>
#include <Graphics/IGraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11SwapChain.h>
#endif

namespace Vortex
{
	GPSwapChain* GPSwapChain::Create(IWindow* window)
	{
		switch (IGraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11SwapChain(window);
		default:
			return nullptr;
		}
	}
}
