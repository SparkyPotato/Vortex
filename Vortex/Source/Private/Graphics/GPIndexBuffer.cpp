#include <VXpch.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11IndexBuffer.h>
#endif

namespace Vortex
{
	GPIndexBuffer* GPIndexBuffer::Create(unsigned int* indices, int count)
	{
		switch (IGraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11IndexBuffer(indices, count);
		default:
			return nullptr;
		}
	}
}
