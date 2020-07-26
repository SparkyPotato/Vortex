#include <VXpch.h>
#include <Graphics/Primitives/GPFramebuffer.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>

namespace Vortex
{
	GPFramebuffer* GPFramebuffer::Create(GPTexture* texture)
	{
		switch (IGraphicsContext::Get()->GetAPI())
		{
		case  GraphicsAPI::DirectX11:
			return new DX11Framebuffer(texture);
		default:
			return nullptr;
		}
	}

	GPFramebuffer* GPFramebuffer::Create(IWindow* window)
	{
		switch (IGraphicsContext::Get()->GetAPI())
		{
		case  GraphicsAPI::DirectX11:
			return new DX11Framebuffer(window);
		default:
			return nullptr;
		}
	}
}
