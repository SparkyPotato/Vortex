#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>

namespace Vortex
{
	DX11Texture::DX11Texture(int width, int height)
	{
		ENG_TRACE("Creating texture.");

		if (width == 0 || height == 0)
			throw std::exception("Cannot create texture with 0 dimensions.");

		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(width, height, TextureUsage::ColorTexture);

		ENG_TRACE("Created texture.");
	}

	DX11Texture::DX11Texture(int width, int height, TextureUsage usage)
	{
		ENG_TRACE("Creating texture.");

		if (width == 0 || height == 0)
			throw std::exception("Cannot create texture with 0 dimensions.");

		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(width, height, usage);

		ENG_TRACE("Created texture.");
	}

	DX11Texture::DX11Texture(ID3D11Texture2D* texture)
	{
		ENG_TRACE("Creating texture.");

		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
		m_Width = desc.Width;
		m_Height = desc.Height;

		if (desc.BindFlags == (D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE))
			m_Usage = TextureUsage::ColorTexture;
		else if (desc.BindFlags == D3D11_BIND_RENDER_TARGET)
			m_Usage = TextureUsage::Framebuffer;
		else if (desc.BindFlags == D3D11_BIND_SHADER_RESOURCE)
			m_Usage = TextureUsage::ShaderResource;
		else
			throw std::exception("Tried to create texture with an illegal D3D11 Texture.");

		GraphicsContext::Get()->RegisterPrimitive(this);

		texture->AddRef();
		p_Texture = texture;

		ENG_TRACE("Created texture.");
	}

	DX11Texture::~DX11Texture()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);
		p_Texture->Release();
		if (p_ShaderResource) p_ShaderResource->Release();
	}

	void DX11Texture::Bind()
	{
		ENG_WARN("Binding a texture does nothing.");
	}

	void DX11Texture::Recreate()
	{
		if (p_Texture) p_Texture->Release();
		p_Texture = nullptr;

		if (p_ShaderResource) p_ShaderResource->Release();
		p_ShaderResource = nullptr;

		Create(m_Width, m_Height, m_Usage);
	}

	void DX11Texture::Resize(int width, int height)
	{
		p_Texture->Release();
		p_ShaderResource->Release();

		Create(width, height, m_Usage);

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());
		context->GetContext()->Flush();
	}

	void DX11Texture::Create(int width, int height, TextureUsage usage)
	{
		m_Width = width;
		m_Height = height;
		m_Usage = usage;

		int flag;
		DXGI_FORMAT format;
		switch (usage)
		{
		case TextureUsage::Framebuffer:
			flag = D3D11_BIND_RENDER_TARGET;
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case TextureUsage::ShaderResource:
			flag = D3D11_BIND_SHADER_RESOURCE;
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case TextureUsage::ColorTexture:
			flag = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.BindFlags = flag;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = NULL;

		context->GetDevice()->CreateTexture2D(&desc, NULL, &p_Texture);

		context->GetDevice()->CreateShaderResourceView(p_Texture, NULL, &p_ShaderResource);
	}
}
