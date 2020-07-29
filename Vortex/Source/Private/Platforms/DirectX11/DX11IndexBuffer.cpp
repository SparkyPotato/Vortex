#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11IndexBuffer.h>

namespace Vortex
{
	DX11IndexBuffer::DX11IndexBuffer(unsigned int* indices, int count)
	{
		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(indices, count);
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);

		m_Buffer->Release();
	}

	void DX11IndexBuffer::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->GetContext()->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Recreate()
	{

	}

	void DX11IndexBuffer::Create(unsigned int* indices, int count)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(unsigned int) * count;
		desc.CPUAccessFlags = NULL;
		desc.StructureByteStride = sizeof(unsigned int);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.MiscFlags = NULL;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = indices;

		context->GetDevice()->CreateBuffer(&desc, &data, &m_Buffer);
	}
}
