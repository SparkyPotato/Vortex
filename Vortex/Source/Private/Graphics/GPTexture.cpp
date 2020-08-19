#include <VXpch.h>
#include <Graphics/Primitives/GPTexture.h>
#include <Graphics/GraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11Texture.h>
#endif

namespace Vortex
{
	GPTexture* GPTexture::Create(int width, int height)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11Texture(width, height);
		default:
			return nullptr;
		}
	}

	GPTexture* GPTexture::Create(int width, int height, TextureUsage usage)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11Texture(width, height, usage);
		default:
			return nullptr;
		}
	}

	GPTexture* GPTexture::Create(std::string file)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11Texture(file);
		default:
			return nullptr;
		}
	}
}
