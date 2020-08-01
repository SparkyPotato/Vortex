#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11VertexBuffer.h>

namespace Vortex
{
	DX11VertexBuffer::DX11VertexBuffer(void* vertices, int count, const VertexLayout& layout)
		: m_Layout(layout)
	{
		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(vertices, count);
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);

		m_Buffer->Release();
	}

	void DX11VertexBuffer::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		UINT stride = m_Layout.GetStride();
		UINT offset = 0;

		context->Lock();
		context->GetContext()->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);
		context->Unlock();

		context->SetVertexBuffer(this);
	}

	void DX11VertexBuffer::Recreate()
	{
		// Does nothing yet.
	}

	void DX11VertexBuffer::Create(void* vertices, int count)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = m_Layout.GetStride() * count;
		desc.CPUAccessFlags = NULL;
		desc.StructureByteStride = m_Layout.GetStride();
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.MiscFlags = NULL;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertices;

		context->GetDevice()->CreateBuffer(&desc, &data, &m_Buffer);
	}
}
