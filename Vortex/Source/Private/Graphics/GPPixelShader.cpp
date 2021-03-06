#include <VXpch.h>
#include <Graphics/Primitives/GPPixelShader.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11PixelShader.h>
#endif

namespace Vortex
{
	GPPixelShader* GPPixelShader::Create(std::string file, std::string defines)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11PixelShader(file, defines);
		default:
			return nullptr;
		}
	}
}
