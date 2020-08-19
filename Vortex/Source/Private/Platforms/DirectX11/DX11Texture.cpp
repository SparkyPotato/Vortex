#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/WICTextureLoader.h>

namespace Vortex
{
	DX11Texture::DX11Texture(int width, int height)
	{
		VX_TRACE(LogGraphicsAPI, "Creating texture.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		if (width == 0 || height == 0)
			throw std::exception("Cannot create texture with 0 dimensions.");

		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(width, height, TextureUsage::ColorTexture);

		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		ID3D11SamplerState* sState;
		context->GetDevice()->CreateSamplerState(&sDesc, &sState);
		context->GetContext()->PSSetSamplers(0, 1, &sState);
		sState->Release();

		VX_TRACE(LogGraphicsAPI, "Created texture.");
	}

	DX11Texture::DX11Texture(int width, int height, TextureUsage usage)
	{
		VX_TRACE(LogGraphicsAPI, "Creating texture.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		if (width == 0 || height == 0)
			throw std::exception("Cannot create texture with 0 dimensions.");

		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(width, height, usage);

		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		ID3D11SamplerState* sState;
		context->GetDevice()->CreateSamplerState(&sDesc, &sState);
		context->GetContext()->PSSetSamplers(0, 1, &sState);
		sState->Release();

		VX_TRACE(LogGraphicsAPI, "Created texture.");
	}

	DX11Texture::DX11Texture(ID3D11Texture2D* texture)
	{
		VX_TRACE(LogGraphicsAPI, "Creating texture.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

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

		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		ID3D11SamplerState* sState;
		context->GetDevice()->CreateSamplerState(&sDesc, &sState);
		context->GetContext()->PSSetSamplers(0, 1, &sState);
		sState->Release();

		VX_TRACE(LogGraphicsAPI, "Created texture.");
	}

	DX11Texture::DX11Texture(std::string file)
	{
		VX_TRACE(LogGraphicsAPI, "Creating texture.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		LoadFromFile(file);

		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		ID3D11SamplerState* sState;
		context->GetDevice()->CreateSamplerState(&sDesc, &sState);
		context->GetContext()->PSSetSamplers(0, 1, &sState);
		sState->Release();

		VX_TRACE(LogGraphicsAPI, "Created texture.");
	}

	DX11Texture::~DX11Texture()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);
		p_Texture->Release();
		if (p_ShaderResource) p_ShaderResource->Release();
	}

	void DX11Texture::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->GetContext()->PSSetShaderResources(0, 1, &p_ShaderResource);
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

		context->Lock();
		context->GetContext()->Flush();
		context->Unlock();
	}

	void DX11Texture::LoadFromFile(std::string file)
	{
		if (p_Texture) p_Texture->Release(); 
		if (p_ShaderResource) p_ShaderResource->Release();

		p_Texture = nullptr;
		p_ShaderResource = nullptr;

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		int size_needed = MultiByteToWideChar(CP_UTF8, 0, file.c_str(), (int)file.size(), NULL, 0);
		std::wstring wfile(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, file.c_str(), (int)file.size(), &wfile[0], size_needed);

		HRESULT hr = CreateWICTextureFromFile
		(
			context->GetDevice(),
			context->GetContext(),
			wfile.c_str(),
			(ID3D11Resource**) &p_Texture,
			&p_ShaderResource
		);

		if (FAILED(hr))
		{
			VX_ERROR(LogGraphicsAPI, "Failed to load texture from file \"{0}\"", file);
			__debugbreak();
			throw std::exception("Failed to load texture from file!");
		}
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
		desc.CPUAccessFlags = NULL;
		desc.MiscFlags = NULL;

		context->GetDevice()->CreateTexture2D(&desc, NULL, &p_Texture);
		context->GetDevice()->CreateShaderResourceView(p_Texture, NULL, &p_ShaderResource);
	}
}
