#pragma once
#include <Graphics/Primitives/GPConstantBuffer.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11ConstantBuffer : public GPConstantBuffer
	{
	public:
		DX11ConstantBuffer(void* data, int size, ConstantBufferTarget target);
		virtual ~DX11ConstantBuffer();

		virtual void Bind() override;
		virtual void Recreate() override;

		virtual void Set(void* newData) override;

	private:
		ID3D11Buffer* m_Buffer;

		ConstantBufferTarget m_Target;
		int m_Size;
	};
}
