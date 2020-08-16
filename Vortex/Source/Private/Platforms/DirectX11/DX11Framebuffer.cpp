#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <Core/Window.h>

namespace Vortex
{
	DX11Framebuffer::DX11Framebuffer(GPTexture* texture)
	{
		GraphicsContext::Get()->RegisterPrimitive(this);

		m_Texture = reinterpret_cast<DX11Texture*>(texture);

		Create(m_Texture);
	}

	DX11Framebuffer::DX11Framebuffer(Window* window)
	{
		GraphicsContext::Get()->RegisterPrimitive(this);

		m_Window = window;

		m_Texture = reinterpret_cast<DX11Texture*>(m_Window->GetSwapChain()->GetBackBuffer());
		Create(m_Texture);
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);

		p_RenderTarget->Release();
		p_DepthStencil->Release();

		// If not on a window, delete the texture.
		if (!m_Window && m_Texture) delete m_Texture;

		m_Texture = nullptr;
	}

	void DX11Framebuffer::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->Lock();

		// Bind the Render Target and make sure the viewport dimensions are correct.
		context->GetContext()->RSSetViewports(1, &m_Viewport);
		context->GetContext()->OMSetRenderTargets(1, &p_RenderTarget, p_DepthStencil);

		context->Unlock();
	}

	void DX11Framebuffer::Recreate()
	{
		Create(m_Texture);
	}

	void DX11Framebuffer::Resize(int width, int height)
	{
		if (m_Window)
		{
			VX_ERROR(LogGraphicsAPI, "Cannot resize framebuffer on window. Use Resize() instead.");
			return;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		// Recreate Depth Stencil from width and height.
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

		// Recreate Render Target from width and height.
		p_RenderTarget->Release();
		// Resize the base texture.
		m_Texture->Resize(width, height);

		context->GetDevice()->CreateRenderTargetView(reinterpret_cast<DX11Texture*>(m_Texture)->GetTexture(), NULL, &p_RenderTarget);

		// Resize the viewport.
		m_Viewport.Width = (float) width;
		m_Viewport.Height = (float) height;

		// Flush all commands so that deletion is forced.
		// This is done since Resize is called many times on resizing.
		// If not done, the application starts gobbling up memory (From 20 MB up to 200 MB!).
		context->Lock();

		context->GetContext()->Flush();

		context->Unlock();
	}

	void DX11Framebuffer::Resize()
	{
		if (!m_Window)
		{
			VX_ERROR(LogGraphicsAPI, "Cannot freely resize texture-based framebuffer. Use Resize(width, height) instead.");
			return;
		}

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		// Remove references to the back buffer.
		p_RenderTarget->Release();
		// Resize back buffer.
		m_Window->GetSwapChain()->Resize();

		m_Texture = reinterpret_cast<DX11Texture*>(m_Window->GetSwapChain()->GetBackBuffer());

		// Recreate Depth Stencil.
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

		// Create Render Target.
		context->GetDevice()->CreateRenderTargetView(m_Texture->GetTexture(), NULL, &p_RenderTarget);

		// Resize the viewport.
		m_Viewport.Width = (float) m_Texture->GetWidth();
		m_Viewport.Height = (float) m_Texture->GetHeight();

		// Flush all commands so that deletion is forced.
		// This is done since Resize is called many times on resizing.
		// If not done, the application starts gobbling up memory (From 20 MB up to 200 MB!).
		context->Lock();

		context->GetContext()->Flush();

		context->Unlock();
	}

	void DX11Framebuffer::Clear(float r, float g, float b, float a)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		float color[4] = { r, g, b, a };

		context->Lock();

		// Clear Render Target to the color.
		context->GetContext()->ClearRenderTargetView(p_RenderTarget, color);
		// Set the Depth Stencil to infinity.
		context->GetContext()->ClearDepthStencilView(p_DepthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);

		context->Unlock();
	}

	void DX11Framebuffer::Create(DX11Texture* texture)
	{
		VX_TRACE(LogGraphicsAPI, "Creating DirectX 11 Framebuffer.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		// Create Render Target.
		context->GetDevice()->CreateRenderTargetView(texture->GetTexture(), NULL, &p_RenderTarget);

		// Create Depth Stencil.
		D3D11_DEPTH_STENCIL_DESC depthDesc = { 0 };
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

		ID3D11DepthStencilState* depthState;
		context->GetDevice()->CreateDepthStencilState(&depthDesc, &depthState);

		context->Lock();
		context->GetContext()->OMSetDepthStencilState(depthState, 1);
		context->Unlock();

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

		// Create the viewport.
		m_Viewport.Width = (float) texture->GetWidth();
		m_Viewport.Height = (float) texture->GetHeight();
		m_Viewport.MinDepth = 0;
		m_Viewport.MaxDepth = 1;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;

		VX_TRACE(LogGraphicsAPI, "Created DirectX 11 Framebuffer.");
	}
}
