#pragma once
#ifdef PLATFORM_WINDOWS
#include <Graphics/IGraphicsContext.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

namespace Vortex
{
	class DX11GraphicsContext : public IGraphicsContext
	{
	public:
		DX11GraphicsContext();
		virtual ~DX11GraphicsContext();

		ID3D11Device* GetDevice() { return p_Device; }
		ID3D11DeviceContext* GetContext() { return p_Context; }
		IDXGIFactory1* GetFactory() { return p_Factory; }

	private:
		ID3D11Device* p_Device;
		ID3D11DeviceContext* p_Context;
		IDXGIFactory1* p_Factory;
	};
}

#endif