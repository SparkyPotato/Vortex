#pragma once
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11IndexBuffer : public GPIndexBuffer
	{
	public:
		DX11IndexBuffer(unsigned int* indices, int count);
		virtual ~DX11IndexBuffer();

		virtual void Bind() override;
		virtual void Recreate() override;

	private:
		void Create(unsigned int* indices, int count);

		ID3D11Buffer* m_Buffer;
	};
}
