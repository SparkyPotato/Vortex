#include <VXpch.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>
#include <Private/Platforms/DirectX11/DX11VertexBuffer.h>
#include <Private/Platforms/DirectX11/DX11VertexShader.h>

namespace Vortex
{
	DX11GraphicsContext::DX11GraphicsContext()
	{
		VX_TRACE(LogGraphicsAPI, "Creating DirectX 11 Graphics Context.");

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

		VX_TRACE(LogGraphicsAPI, "Created DirectX 11 Context.");
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

	void DX11GraphicsContext::SetTopology(Topology topology)
	{
		switch (topology)
		{
		case Topology::LineList:
			p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case Topology::LineStrip:
			p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			break;
		case Topology::TriangleList:
			p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case Topology::TriangleStrip:
			p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			break;
		}
	}

	void DX11GraphicsContext::Draw(int drawSize)
	{
		Lock();
		p_Context->DrawIndexed(drawSize, 0, 0);
		Unlock();
	}

	void DX11GraphicsContext::Lock()
	{
		m_ContextMutex.lock();
	}

	void DX11GraphicsContext::Unlock()
	{
		m_ContextMutex.unlock();
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

	void DX11GraphicsContext::SetVertexBuffer(DX11VertexBuffer* buffer)
	{
		if (m_CurrentVertexBuffer)
		{
			if (!buffer)
			{
				m_CurrentVertexBuffer = buffer;
				return;
			}

			if (buffer->GetLayout() != m_CurrentVertexBuffer->GetLayout())
			{
				m_CurrentVertexBuffer = buffer;
				GenerateInputLayout();
			}
		}
		else
		{
			m_CurrentVertexBuffer = buffer;
			GenerateInputLayout();
		}
	}

	void DX11GraphicsContext::SetVertexShader(DX11VertexShader* shader)
	{
		m_CurrentVertexShader = shader;
	}

	void DX11GraphicsContext::GenerateInputLayout()
	{
		int size = m_CurrentVertexBuffer->GetLayout().GetElementCount();
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[size];
		std::string* semantics = new std::string[size];

		int i = 0;
		for (auto element : m_CurrentVertexBuffer->GetLayout().GetElements())
		{
			switch (element.dataType)
			{
			case ShaderDataType::float1:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R32_FLOAT;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				break;
			case ShaderDataType::float2:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R32G32_FLOAT;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				i++;
				break;
			case ShaderDataType::float3:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				i++;
				break;
			case ShaderDataType::float4:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				i++;
				break;
			case ShaderDataType::float3x3:
				for (int j = 0; j < 3; j++)
				{
					semantics[i] = element.semantic;
					desc[i].SemanticName = semantics[i].c_str();
					desc[i].SemanticIndex = j;
					desc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
					desc[i].InputSlot = 0;
					desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
					desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					desc[i].InstanceDataStepRate = 0;
					i++;
				}
				break;
			case ShaderDataType::float4x4:
				for (int j = 0; j < 4; j++)
				{
					semantics[i] = element.semantic;
					desc[i].SemanticName = semantics[i].c_str();
					desc[i].SemanticIndex = j;
					desc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					desc[i].InputSlot = 0;
					desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
					desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					desc[i].InstanceDataStepRate = 0;
					i++;
				}
				break;
			}
		}

		ID3D11InputLayout* layout;
		p_Device->CreateInputLayout(desc, size, m_CurrentVertexShader->GetBlob()->GetBufferPointer(),
			m_CurrentVertexShader->GetBlob()->GetBufferSize(), &layout);
		p_Context->IASetInputLayout(layout);
		layout->Release();

		delete[] desc;
		delete[] semantics;
	}
}

#endif
