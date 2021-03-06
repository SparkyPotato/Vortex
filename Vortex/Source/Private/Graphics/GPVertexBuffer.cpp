#include <VXpch.h>
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/GraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11VertexBuffer.h>
#endif

namespace Vortex 
{
	GPVertexBuffer* GPVertexBuffer::Create(void* vertices, unsigned int count, const VertexLayout& layout, BufferAccessType accessType)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11VertexBuffer(vertices, count, layout, accessType);
		default:
			return nullptr;
		}
	}

	GPVertexBuffer* GPVertexBuffer::Create(unsigned int size, VertexLayout& layout)
	{
		switch (GraphicsContext::Get()->GetAPI())
		{
		case GraphicsAPI::DirectX11:
			return new DX11VertexBuffer(size, layout);
		default:
			return nullptr;
		}
	}
}
