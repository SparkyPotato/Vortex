#include <VXpch.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	DX11GraphicsContext::DX11GraphicsContext()
	{
		ENG_TRACE("Creating DirectX 11 Graphics Context.");

		D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_1;
		D3D_FEATURE_LEVEL levelGot;
		D3D11_CREATE_DEVICE_FLAG createFlags = static_cast<D3D11_CREATE_DEVICE_FLAG>(NULL);

		// Create the device on the debug layer if we're debugging the engine.
		#ifdef CFG_DEBUGENG
		createFlags = D3D11_CREATE_DEVICE_DEBUG;
		#endif

		HRESULT hr = D3D11CreateDevice
		(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createFlags,
			&level,
			1,
			D3D11_SDK_VERSION,
			&p_Device,
			&levelGot,
			&p_Context
		);

		// If we failed to create the Device, or didn't get it with feature level 11.1, throw an exception.
		if (FAILED(hr) || levelGot != D3D_FEATURE_LEVEL_11_1)
			throw std::exception("Failed to create DirectX 11.1 Context.");

		hr = CreateDXGIFactory
		(
			__uuidof(IDXGIFactory),
			(void**) &p_Factory
		);

		// If we failed to create the DXGI Factory, throw an exception.
		if (FAILED(hr))
			throw std::exception("Failed to create DXGI 1.1 Factory.");

		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ENG_TRACE("Created DirectX 11 Context.");
	}

	DX11GraphicsContext::~DX11GraphicsContext()
	{
		// Make sure we delete everything.
		p_Context->ClearState();
		p_Context->Flush();

		p_Factory->Release();
		p_Factory = nullptr;
		p_Context->Release();
		p_Context = nullptr;

		// If we're debugging the engine, print live objects.
		PrintDebugInfo();

		p_Device->Release();
		p_Device = nullptr;
	}

	void DX11GraphicsContext::Draw(int drawSize)
	{
		p_Context->DrawIndexed(drawSize, 0, 0);
	}

	void DX11GraphicsContext::PrintDebugInfo()
	{
		#ifdef CFG_DEBUGENG
		ID3D11Debug* debug;
		// Get the debug device from the device.
		p_Device->QueryInterface(__uuidof(ID3D11Debug), (void**) &debug);

		// Report all live objects with all details.
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

		debug->Release();
		#endif
	}
}

#endif
