#include <VXpch.h>
#include <Graphics/Primitives/GPGeometryShader.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GeometryShader.h>
#endif

namespace Vortex
{
	GPGeometryShader* GPGeometryShader::Create(std::string file, std::string defines)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11GeometryShader(file, defines);
		default:
			return nullptr;
		}
	}
}
