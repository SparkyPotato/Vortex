#include <VXpch.h>
#include <Graphics/Primitives/GPVertexShader.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11VertexShader.h>
#endif

namespace Vortex
{
	GPVertexShader* GPVertexShader::Create(std::string file, std::string defines)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11VertexShader(file, defines);
		default:
			return nullptr;
		}
	}
}
