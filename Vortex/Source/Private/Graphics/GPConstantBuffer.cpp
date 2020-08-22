#include <VXpch.h>
#include <Graphics/Primitives/GPConstantBuffer.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11ConstantBuffer.h>
#endif

namespace Vortex
{
	GPConstantBuffer* GPConstantBuffer::Create(void* data, int size, int target)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11ConstantBuffer(data, size, target);
		default:
			return nullptr;
		}
	}
}
