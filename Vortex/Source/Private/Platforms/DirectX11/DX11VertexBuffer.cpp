#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11VertexBuffer.h>

namespace Vortex
{
	DX11VertexBuffer::DX11VertexBuffer(void* vertices, unsigned int count, const VertexLayout& layout, BufferAccessType accessType)
		: m_Layout(layout), m_Size(count), m_AccessType(accessType)
	{
		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(vertices);
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());
		if (context->GetVertexBuffer() == this)
		{
			context->SetVertexBuffer(nullptr);
		}

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

	void DX11VertexBuffer::Unbind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		UINT stride = m_Layout.GetStride();
		UINT offset = 0;

		context->Lock();
		context->GetContext()->IASetVertexBuffers(0, 1, nullptr, &stride, &offset);
		context->Unlock();

		context->SetVertexBuffer(nullptr);
	}

	void DX11VertexBuffer::Recreate()
	{
		// Does nothing yet.
	}

	void DX11VertexBuffer::Set(void* vertices, unsigned int count)
	{
		if (m_AccessType == BufferAccessType::Static)
		{
			VX_ERROR(LogGraphicsAPI, "Cannot set static vertex buffer!");
			return;
		}
		if (count > m_Size)
		{
			VX_ERROR(LogGraphicsAPI, "Cannot increase size of vertex buffer!");
			return;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());
		context->Lock();

		D3D11_MAPPED_SUBRESOURCE sub;
		context->GetContext()->Map(m_Buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
		memcpy(sub.pData, vertices, m_Layout.GetStride() * count);
		context->GetContext()->Unmap(m_Buffer, NULL);

		context->Unlock();
	}

	void DX11VertexBuffer::Create(void* vertices)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		UINT cpuAccess = NULL;
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
		if (m_AccessType == BufferAccessType::Dynamic)
		{
			cpuAccess = D3D11_CPU_ACCESS_WRITE;
			usage = D3D11_USAGE_DYNAMIC;
		}

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = m_Layout.GetStride() * m_Size;
		desc.CPUAccessFlags = cpuAccess;
		desc.StructureByteStride = m_Layout.GetStride();
		desc.Usage = usage;
		desc.MiscFlags = NULL;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertices;

		context->GetDevice()->CreateBuffer(&desc, &data, &m_Buffer);
	}
}
