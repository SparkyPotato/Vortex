#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <Core/IWindow.h>

namespace Vortex
{
	DX11Framebuffer::DX11Framebuffer(GPTexture* texture)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);

		m_Texture = reinterpret_cast<DX11Texture*>(texture);

		Create(m_Texture);
	}

	DX11Framebuffer::DX11Framebuffer(IWindow* window)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);

		m_Window = window;
		m_Window->SetFramebuffer(this);

		m_Texture = reinterpret_cast<DX11Texture*>(m_Window->GetSwapChain()->GetBackBuffer());
		Create(m_Texture);
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
		IGraphicsContext::Get()->UnregisterPrimitive(this);

		p_RenderTarget->Release();
		p_DepthStencil->Release();

		if (m_Window) m_Window->SetFramebuffer(nullptr);
		if (!m_Window) delete m_Texture;
	}

	void DX11Framebuffer::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		context->GetContext()->OMSetRenderTargets(1, &p_RenderTarget, p_DepthStencil);
		context->GetContext()->RSSetViewports(1, &m_Viewport);
	}

	void DX11Framebuffer::Recreate()
	{
		Create(m_Texture);
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

		context->GetContext()->Flush();
	}

	void DX11Framebuffer::Resize()
	{
		if (!m_Window)
		{
			ENG_ERROR("Cannot freely resize texture-based framebuffer. Use Resize(width, height) instead.");
			return;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		p_RenderTarget->Release();
		m_Window->GetSwapChain()->Resize();

		m_Texture = reinterpret_cast<DX11Texture*>(m_Window->GetSwapChain()->GetBackBuffer());

		p_DepthStencil->Release();
		ID3D11Texture2D* depthStencil;
		D3D11_TEXTURE2D_DESC descDepth = { 0 };
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = m_Texture->GetWidth();
		descDepth.Height = m_Texture->GetHeight();
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

		context->GetDevice()->CreateRenderTargetView(m_Texture->GetTexture(), NULL, &p_RenderTarget);

		m_Viewport.Width = (float) m_Texture->GetWidth();
		m_Viewport.Height = (float) m_Texture->GetHeight();

		context->GetContext()->Flush();
	}

	void DX11Framebuffer::SetWindow(IWindow* window)
	{
		if (m_Window)
			m_Window->SetFramebuffer(nullptr);
		else
			delete m_Texture;

		m_Window = window;

		if (m_Window)
		{
			m_Window->SetFramebuffer(this);
			m_Texture = reinterpret_cast<DX11Texture*>(m_Window->GetSwapChain()->GetBackBuffer());
			Create(m_Texture);
		}
	}

	void DX11Framebuffer::Clear(float r, float g, float b, float a)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		float color[4] = { r, g, b, a };

		context->GetContext()->ClearRenderTargetView(p_RenderTarget, color);
	}

	void DX11Framebuffer::Create(DX11Texture* texture)
	{
		ENG_TRACE("Creating DirectX 11 Framebuffer.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		context->GetDevice()->CreateRenderTargetView(texture->GetTexture(), NULL, &p_RenderTarget);

		D3D11_DEPTH_STENCIL_DESC depthDesc = { 0 };
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

		ID3D11DepthStencilState* depthState;
		context->GetDevice()->CreateDepthStencilState(&depthDesc, &depthState);
		context->GetContext()->OMSetDepthStencilState(depthState, 1);
		depthState->Release();

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

		m_Viewport.Width = (float) texture->GetWidth();
		m_Viewport.Height = (float) texture->GetHeight();
		m_Viewport.MinDepth = 0;
		m_Viewport.MaxDepth = 1;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;

		ENG_TRACE("Created DirectX 11 Framebuffer.");
	}
}
