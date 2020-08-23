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

		VX_TRACE(LogGraphicsAPI, "Setting up blend state.");

		D3D11_RENDER_TARGET_BLEND_DESC renderDesc;
		ZeroMemory(&renderDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		renderDesc.BlendEnable = true;
		renderDesc.SrcBlend = D3D11_BLEND_ONE;
		renderDesc.DestBlend = D3D11_BLEND_SRC_COLOR;
		renderDesc.BlendOp = D3D11_BLEND_OP_ADD;
		renderDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderDesc.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		renderDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0] = renderDesc;

		ID3D11BlendState* blendState = nullptr;
		p_Device->CreateBlendState(&blendDesc, &blendState);
		p_Context->OMSetBlendState(blendState, NULL, 0xffffffff);
		blendState->Release();

		VX_TRACE(LogGraphicsAPI, "Setup blend state.");

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
		case Topology::PointList:
			p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
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

	void DX11GraphicsContext::Draw(unsigned int drawSize)
	{
		Lock();
		p_Context->Draw(drawSize, 0);
		Unlock();
	}

	void DX11GraphicsContext::DrawIndexed(unsigned int drawSize)
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
		if (!m_CurrentVertexShader || !m_CurrentVertexBuffer) return;

		int size = m_CurrentVertexBuffer->GetLayout().GetElementCount();
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[size];
		std::string* semantics = new std::string[size];

		int i = 0;
		for (auto element : m_CurrentVertexBuffer->GetLayout().GetElements())
		{
			switch (element.dataType)
			{
			case ShaderDataType::col1:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R8_UNORM;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				break;
			case ShaderDataType::col2:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R8G8_UNORM;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				break;
			case ShaderDataType::col4:
				semantics[i] = element.semantic;
				desc[i].SemanticName = semantics[i].c_str();
				desc[i].SemanticIndex = 0;
				desc[i].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc[i].InputSlot = 0;
				desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc[i].InstanceDataStepRate = 0;
				break;
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

		Lock();
		ID3D11InputLayout* layout;
		p_Device->CreateInputLayout(desc, size, m_CurrentVertexShader->GetBlob()->GetBufferPointer(),
			m_CurrentVertexShader->GetBlob()->GetBufferSize(), &layout);
		p_Context->IASetInputLayout(layout);
		layout->Release();
		Unlock();

		delete[] desc;
		delete[] semantics;
	}
}

#endif
