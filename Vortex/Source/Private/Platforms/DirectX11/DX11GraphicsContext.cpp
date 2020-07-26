#include <VXpch.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	DX11GraphicsContext::DX11GraphicsContext()
	{
		ENG_TRACE("Creating DirectX 11 Graphics Context.");

		D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_1;
		HRESULT hr = D3D11CreateDevice
		(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			&level,
			1,
			D3D11_SDK_VERSION,
			&p_Device,
			NULL,
			&p_Context
		);
		if (FAILED(hr))
			throw std::exception("Failed to create DirectX 11.1 Context.");

		hr = CreateDXGIFactory
		(
			__uuidof(IDXGIFactory),
			(void**) &p_Factory
		);
		if (FAILED(hr))
			throw std::exception("Failed to create DXGI 1.1 Factory.");

		ENG_TRACE("Created DirectX 11 Context.");
	}

	DX11GraphicsContext::~DX11GraphicsContext()
	{
		p_Context->ClearState();
		p_Context->Flush();

		p_Factory->Release();
		p_Factory = nullptr;
		p_Context->Release();
		p_Context = nullptr;

		PrintDebugInfo();

		p_Device->Release();
		p_Device = nullptr;
	}

	void DX11GraphicsContext::PrintDebugInfo()
	{
		ID3D11Debug* debug;
		p_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

		debug->Release();
	}
}

#endif
