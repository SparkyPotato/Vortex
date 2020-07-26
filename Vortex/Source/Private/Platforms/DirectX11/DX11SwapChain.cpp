#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11SwapChain.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>

namespace Vortex
{
	DX11SwapChain::DX11SwapChain(IWindow* window)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);
		m_Window = reinterpret_cast<WWindow*>(window);
		Recreate();
	}

	DX11SwapChain::~DX11SwapChain()
	{
		IGraphicsContext::Get()->UnregisterPrimitive(this);

		delete m_BackBuffer;
		p_SwapChain->Release();
	}

	void DX11SwapChain::Bind()
	{
		ENG_WARN("Binding a swap chain does nothing.");
	}

	void DX11SwapChain::Recreate()
	{
		ENG_TRACE("Creating DirectX 11 Swap Chain.");

		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		if (p_SwapChain) p_SwapChain->Release();
		p_SwapChain = nullptr;

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferCount = 2;
		desc.BufferDesc.Width = 0;
		desc.BufferDesc.Height = 0;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.OutputWindow = m_Window->GetWindowHandle();
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.Windowed = TRUE;

		int hr = context->GetFactory()->CreateSwapChain
		(
			context->GetDevice(),
			&desc,
			&p_SwapChain
		);

		if (FAILED(hr))
			throw std::exception("Failed to create DirectX 11 Swap Chain.");

		if (m_BackBuffer) delete m_BackBuffer;
		ID3D11Texture2D* texture;
		p_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture);
		m_BackBuffer = new DX11Texture(texture);
		texture->Release();

		ENG_TRACE("Created DirectX 11 Swap Chain.");
	}

	GPTexture* DX11SwapChain::GetBackBuffer()
	{
		return m_BackBuffer;
	}

	void DX11SwapChain::Resize()
	{
		delete m_BackBuffer;

		HRESULT hr = p_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		ID3D11Texture2D* texture;
		p_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &texture);
		m_BackBuffer = new DX11Texture(texture);
		texture->Release();

		if (FAILED(hr))
			ENG_WARN("Failed to resize swap chain.");
	}

	void DX11SwapChain::Swap(int syncInterval)
	{
		HRESULT hr = p_SwapChain->Present(syncInterval, 0);

		if (FAILED(hr))
			ENG_WARN("Failed to swap.");

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			ENG_WARN("Device removed. Driver probably crashed. Recreating graphics device.");
			IGraphicsContext::Recreate();
		}
	}
}
