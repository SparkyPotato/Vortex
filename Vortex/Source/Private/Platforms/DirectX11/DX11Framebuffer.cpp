#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Core/IWindow.h>

namespace Vortex
{
	DX11Framebuffer::DX11Framebuffer(GPTexture* texture)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);

		m_Texture = texture;

		Create(m_Texture);
	}

	DX11Framebuffer::DX11Framebuffer(IWindow* window)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);

		m_Window = window;
		m_Window->SetFramebuffer(this);

		GPTexture* texture = m_Window->GetSwapChain()->GetBackBuffer();
		Create(texture);
		delete texture;
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
		IGraphicsContext::Get()->UnregisterPrimitive(this);

		if (m_Texture) delete m_Texture;
	}

	void DX11Framebuffer::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		context->GetContext()->OMSetRenderTargets(1, p_RenderTarget.GetAddressOf(), p_DepthStencil.Get());
		context->GetContext()->RSSetViewports(1, &m_Viewport);
	}

	void DX11Framebuffer::Recreate()
	{
		
	}

	void DX11Framebuffer::Resize(int width, int height)
	{
		if (m_Window)
		{
			ENG_ERROR("Cannot force size onto window. Use Resize() instead.");
			return;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		p_DepthStencil->Release();
		ID3D11Texture2D* depthStencil;
		D3D11_TEXTURE2D_DESC descDepth = { 0 };
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		context->GetDevice()->CreateTexture2D(&descDepth, NULL, &depthStencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC dvDesc;
		ZeroMemory(&dvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dvDesc.Texture2D.MipSlice = 0;

		context->GetDevice()->CreateDepthStencilView(depthStencil, &dvDesc, &p_DepthStencil);
		depthStencil->Release();

		p_RenderTarget->Release();
		m_Texture->Resize(width, height);

		context->GetDevice()->CreateRenderTargetView(reinterpret_cast<DX11Texture*>(m_Texture)->GetTexture(), NULL, &p_RenderTarget);

		m_Viewport.Width = (float) width;
		m_Viewport.Height = (float) height;
	}

	void DX11Framebuffer::Resize()
	{
		if (m_Texture)
		{
			ENG_ERROR("Cannot freely resize texture-based framebuffer. Use Resize(width, height) instead.");
			return;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		p_RenderTarget.Reset();
		p_DepthStencil.Reset();
		m_Window->GetSwapChain()->Resize();

		DX11Texture* texture = reinterpret_cast<DX11Texture*>(m_Window->GetSwapChain()->GetBackBuffer());

		ID3D11Texture2D* depthStencil;
		D3D11_TEXTURE2D_DESC descDepth = { 0 };
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = texture->GetWidth();
		descDepth.Height = texture->GetHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		context->GetDevice()->CreateTexture2D(&descDepth, NULL, &depthStencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC dvDesc;
		ZeroMemory(&dvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dvDesc.Texture2D.MipSlice = 0;

		context->GetDevice()->CreateDepthStencilView(depthStencil, &dvDesc, &p_DepthStencil);
		depthStencil->Release();

		context->GetDevice()->CreateRenderTargetView(texture->GetTexture(), NULL, &p_RenderTarget);

		m_Viewport.Width = (float) texture->GetWidth();
		m_Viewport.Height = (float) texture->GetHeight();

		delete texture;
	}

	void DX11Framebuffer::Clear(float r, float g, float b, float a)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		float color[4] = { r, g, b, a };

		context->GetContext()->ClearRenderTargetView(p_RenderTarget.Get(), color);
	}

	void DX11Framebuffer::Create(GPTexture* texture)
	{
		ENG_TRACE("Creating DirectX 11 Framebuffer.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());
		DX11Texture* text = reinterpret_cast<DX11Texture*>(texture);

		context->GetDevice()->CreateRenderTargetView(text->GetTexture(), NULL, &p_RenderTarget);

		D3D11_DEPTH_STENCIL_DESC depthDesc = { 0 };
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

		ID3D11DepthStencilState* depthState;
		context->GetDevice()->CreateDepthStencilState(&depthDesc, &depthState);
		context->GetContext()->OMSetDepthStencilState(depthState, 1);

		ID3D11Texture2D* depthStencil;
		D3D11_TEXTURE2D_DESC descDepth = { 0 };
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = text->GetWidth();
		descDepth.Height = text->GetHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		context->GetDevice()->CreateTexture2D(&descDepth, NULL, &depthStencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC dvDesc;
		ZeroMemory(&dvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dvDesc.Texture2D.MipSlice = 0;

		context->GetDevice()->CreateDepthStencilView(depthStencil, &dvDesc, &p_DepthStencil);

		m_Viewport.Width = (float) text->GetWidth();
		m_Viewport.Height = (float) text->GetHeight();
		m_Viewport.MinDepth = 0;
		m_Viewport.MaxDepth = 1;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;

		ENG_TRACE("Created DirectX 11 Framebuffer.");
	}
}
