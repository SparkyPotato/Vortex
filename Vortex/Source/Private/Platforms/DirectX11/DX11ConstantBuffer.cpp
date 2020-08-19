#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11ConstantBuffer.h>

namespace Vortex
{
	DX11ConstantBuffer::DX11ConstantBuffer(void* data, int size, ConstantBufferTarget target)
		: m_Size(size), m_Target(target)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->RegisterPrimitive(this);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = size;
		desc.StructureByteStride = size;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.MiscFlags = NULL;

		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = data;

		context->GetDevice()->CreateBuffer(&desc, &subData, &m_Buffer);
	}

	DX11ConstantBuffer::~DX11ConstantBuffer()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);

		m_Buffer->Release();
	}

	void DX11ConstantBuffer::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->Lock();

		switch (m_Target)
		{
		case ConstantBufferTarget::VertexShader:
			context->GetContext()->VSSetConstantBuffers(0, 1, &m_Buffer);
			break;
		case ConstantBufferTarget::PixelShader:
			context->GetContext()->PSSetConstantBuffers(0, 1, &m_Buffer);
			break;
		}

		context->Unlock();
	}

	void DX11ConstantBuffer::Recreate()
	{

	}

	void DX11ConstantBuffer::Set(void* newData)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->Lock();

		D3D11_MAPPED_SUBRESOURCE sub;
		context->GetContext()->Map(m_Buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
		memcpy(sub.pData, newData, m_Size);
		context->GetContext()->Unmap(m_Buffer, NULL);

		context->Unlock();
	}
}
