#include <VXpch.h>
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/IGraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11VertexBuffer.h>
#endif

namespace Vortex 
{
	GPVertexBuffer* GPVertexBuffer::Create(void* vertices, int count, const VertexLayout& layout)
	{
		switch (IGraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11VertexBuffer(vertices, count, layout);
		default:
			return nullptr;
		}
	}
}