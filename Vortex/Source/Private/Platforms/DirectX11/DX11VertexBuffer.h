#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11VertexBuffer : public GPVertexBuffer
	{
	public:
		DX11VertexBuffer(void* vertices, unsigned int count, const VertexLayout& layout, BufferAccessType accessType);
		DX11VertexBuffer(unsigned int count, const VertexLayout& layout);
		virtual ~DX11VertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Recreate() override;
		virtual void Set(void* vertices, unsigned int count) override;

		unsigned int GetSize() override { return m_Size; }

		virtual const VertexLayout& GetLayout() override { return m_Layout; }

	private:
		void Create(void* vertices);

		ID3D11Buffer* m_Buffer;
		VertexLayout m_Layout;
		BufferAccessType m_AccessType;
		unsigned int m_Size;
	};
}
