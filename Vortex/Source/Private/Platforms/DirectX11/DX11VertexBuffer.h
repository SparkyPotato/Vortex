#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11VertexBuffer : public GPVertexBuffer
	{
	public:
		DX11VertexBuffer(void* vertices, int count, const VertexLayout& layout);
		virtual ~DX11VertexBuffer();

		virtual void Bind() override;
		virtual void Recreate() override;

	private:
		void Create(void* vertices, int count);

		ID3D11Buffer* m_Buffer;
		VertexLayout m_Layout;
	};
}
